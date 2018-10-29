include <board_primitives.scad>
use <board_shape.scad>
use <cutouts.scad>
use <artwork.scad>

use <imports/cherry_mx.scad>
include <imports/mx_keycap.scad>
key_profile_index = 11;

$fn = 256;

// For the animation
explode_factor = 20;

layer_thickness = [
    3, // bottom cover
    3.5, // bottom spacer
    1.6, // PCB
    2,   // top spacer
    1.5, // top reflector
    0.1, // artwork
    1.5, // top cover
    1.5, // knob spacer
    1.5, // knob cover
];

// to work out viewports for render
//echo(str($vpt[0],",",$vpt[1],",",$vpt[2],",",$vpr[0],",",$vpr[1],",",$vpr[2],",",$vpd));

module bottom_cover() {
    difference() {
        board();
        bolts_external();
    }
}

module bottom_spacer() {
    difference() {
        board();
        bolts_internal();
        switch_socket_holes();
        mcu_cutout();
        usb_cutout();
        encoder_pins();
    }
}

module pcb() {
    difference() {
        board();
        bolts_internal();
    }
}

module top_outline_offset() {
    offset(delta = -top_plate_shrink)
    top_outline();
}

module top_spacer() {
    difference() {
        top_outline();
        bolts_internal();
        switch_internal();
        led_cutouts();
        macro_internal();
        
    }
}

module top_spacer_engrave() {
    engraveInterval = 0.05;
    wallOverlap = 1;
    // go in intervals of 2 to avoid final implicit union()
    for(i = [0.05:engraveInterval*2:wallOverlap+engraveInterval]) {
        difference() {
            offset(delta=-i)
            top_outline();
            
            offset(delta=-(i+engraveInterval))
            top_outline();
        }
    }
}

module top_reflector() {
    difference() {
        top_outline_offset();
        bolts_external();
        switch_internal();
        macro_internal();
    }
}

module art_outline() {
    difference() {
        top_outline_offset();
        bolts_external();
        switch_artwork();
        macro_external();
    }
}

module top_cover() {
    difference() {
        top_outline_offset();
        bolts_external();
        switch_external();
        macro_external();
    }
}

module knob_spacer() {
    intersection() {
        top_outline_offset();
        
        encoders()
        circle(r=encoder_radius_board-top_plate_shrink);
    }
}

module knob_cover_panel() {
    intersection() {
        offset(delta = -top_plate_shrink)
        board();
        
        encoders()
        circle(r=encoder_radius_board-top_plate_shrink);
    }
}

module knob_cover_holes() {
    encoders() {
        circle(d=9.3);
        
        translate([0,-9])
        circle(d=3.3);
    }
}

module knob_cover() {
    difference() {
        // panel
        knob_cover_panel();
        
        // panel holes
        knob_cover_holes();
    }
}

module boop() {
    difference() {
        knob_cover_panel();
        board_half();
    }
}

module knob_cover_cnc() {
    difference() {
        union() {
            linear_extrude(1.5)
            difference() {
                knob_spacer();
                bolts_external();
            }
            
            translate([0,0,1.5])
            hull() {
                linear_extrude(0.01)
                boop();
                translate([0,0,1.5])
                linear_extrude(0.01)
                offset(r=-1.5)
                boop();
            }
        }
        linear_extrude(10){
            bolts_external();
            knob_cover_holes();
        }
        
        translate([0,0,-0.1])
        linear_extrude(5)
        board_half();
    }
}

module macro_model() {
    translate([0,0,2])
    rotate([90,0,0])
    import("imports/KSC521J.stl");
}

module stab_model() {
    color("gray")
    rotate([0,0,180])
    import("imports/mx_pcb_stab_open.stl");
}

module switch_models() {
    switches_all(colour = false)
    cherry_mx_model();
    
    fx(colour = false)
    stab_model();
}

module switch_keycaps() {
    translate([0,0,11.5])
    switches_all(apply_scaling = true)
    key();
}

module encoder_knob() {
    color([0.7,0.7,0.7])
    cylinder(20, d = 25);
}

module encoder_model(knob = true) {
    translate([0,0,5.5])
    rotate([90,0,0])
    import("imports/PEC16.stl");
    
    // Shaft
    encoder_shaft = 6;
    encoder_flat = 4.5;
    translate([0,0,6.5])
    linear_extrude(20)
    intersection() {
        circle(d = encoder_shaft);
        translate([0, (encoder_shaft-encoder_flat)/2])
        square([encoder_shaft, encoder_flat], center = true);
    }
    
    if(knob) {
        translate([0,0,13.5])
        encoder_knob();
    }
}

module animate_layer(index) {
    boom = 20 * quad_ease_inout($t);
    
    translate([0,0,boom*index + sum(layer_thickness, index)])
    children();
}

module extrude_plate(index) {
    animate_layer(index)
    linear_extrude(layer_thickness[index])
    children();
}

module full_stack(knobs = true) {
    color([1, 1, 1, 0.4])
    extrude_plate(0)
    bottom_cover();
    
    color([0.8, 0.8, 0.8, 0.2])
    extrude_plate(1)
    bottom_spacer();
    
    color([1,1,1, 1])
    extrude_plate(2)
    pcb();
    
    color([0.8, 0.8, 0.8, 0.2])
    extrude_plate(3)
    top_spacer();
    
    color([1,1,1, 1])
    extrude_plate(4)
    top_reflector();
    
    animate_layer(5)
    artwork();
    
    color([0.8, 0.8, 0.8, 0.4])
    extrude_plate(6)
    top_cover();
    
    // this isn't ready yet - need bevel
    /*color([0.8, 0.8, 0.8, 0.3])
    extrude_plate(7)
    knob_spacer();
    
    color([0.8, 0.8, 0.8, 0.4])
    extrude_plate(8)
    knob_cover();*/
    
    // some nice models
    animate_layer(2)
    translate([macro_pos.x, macro_pos.y, layer_thickness[2]])
    color("gray")
    macro_model();
    
    // start at layer 2 but rise up above layer 6
    translate([0,0,90 * quad_ease_inout($t)])
    animate_layer(2) {
        color([0.8,0.8,0.8,1])
        encoders()
        encoder_model(knobs);
        
        switch_models();
        switch_keycaps();
    }
}

// override in commandline
build = "render";
if(build == "render") {
    full_stack();
} else if(build == "bottom_plate") {
    bottom_cover();
} else if(build == "bottom_spacer") {
    bottom_spacer();
} else if(build == "top_spacer") {
    top_spacer();
} else if(build == "top_reflector") {
    top_reflector();
} else if(build == "top_spacer_engrave") {
    top_spacer_engrave();
} else if(build == "top_plate") {
    top_cover();
} else if(build == "art_cutout") {
    art_outline();
} else if(build == "knob_spacer") {
    knob_spacer();
} else if(build == "knob_cover") {
    knob_cover();
} else if(build == "knob_cover_cnc") {
    knob_cover_cnc();
}