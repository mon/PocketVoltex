include <board_primitives.scad>
use <board_shape.scad>

// SWITCHES

module switch_socket_holes() {
    switches_all() {
        holesize = [19,11];
        translate([-holesize.x/2, -3])
        rounded_square(holesize.x, holesize.y, 2);
    }
    
    fx() {
        reflect([1,0,0])
        mirror([0,1,0]) { // my switches have upside down stabs

            translate([11.9,7])
            circle(d=4.5);
            
            hull() {
                translate([11.9,-8.24])
                circle(d=4);
                
                translate([11.9,-10])
                circle(d=4);
            }
        }
    }
}

a = 23.8; // mx datasheet, good for 2/2.25/2.75 units
main_box = [6.65,12.3];

module stabiliser_cutout_offset() {
    // my switches have upside down stabs
    mirror([0,1,0])
    // per datasheet
    translate([a/2,main_box.y/2 - 6.77])
    children();
}

module stabiliser_cutout_bare_minimum() {
    stabiliser_cutout_offset() {
        // main body of the stabs
        square(main_box, center=true);
        
        // cutout for the bar to pivot
        translate([0,-1.2])
        square([3, main_box.y], center=true);
        
        // linking gap to main switch body
        translate([-a/4,0])
        square([a/2, main_box.y - 1.6], center=true);
    }
}

module stabiliser_cutout_panel_bits() {
    // cutout for the side bits on a panel-mount
    stabiliser_cutout_offset()
    translate([0.875,3/2-0.5])
    square([main_box.x, 2.8], center=true);
}

module stabiliser_cutout_external() {
    stabiliser_cutout_bare_minimum();
    stabiliser_cutout_panel_bits();
}

module stabiliser_cutout_internal() {
    // since we want most of it
    stabiliser_cutout_external();
    
    // get rid of the fingers at the bottom
    //resize([20,0,0])
    //switch_hole(expansion=1);
    
    stabiliser_cutout_offset() {
        // space for the bar to move
        
        translate([-a/4,-2.5])
        square([a/2, main_box.y], center=true);
        
        // cut off ugly bottom bits
        translate([-a/4,-2.5])
        square([a/2, main_box.y], center=true);
        
        // also space for the stab itself
        translate([0,-0.5])
        square([7.4, 19], center=true);
    }
}

module stabiliser_cutout_artwork() {
    stabiliser_cutout_bare_minimum();
    // no panel bits, if you swap to panel mount,
    // cut a new thing yourself
    
    // linking gap to main switch body, but bigger
    // Less flappy bits, nicer cuts
    extra=0.3;
    stabiliser_cutout_offset() {
        translate([-a/4,-extra/2])
        square([a/2, main_box.y+extra], center=true);
    }
}

module switch_hole(expansion = false) {
    // from Cherry MX data sheet
    holesize = 14;
    
    translate([-holesize/2, -holesize/2])
    square([holesize,holesize]);
    
    // expansion for inner layers so the clips have something to grab
    if(expansion) {
        expand_hole = [5, 15];
        translate(-expand_hole/2)
        square(expand_hole);
    }
}

module switch_external() {
    switches_all()
    switch_hole();
    
    fx()
    reflect()
    stabiliser_cutout_external();
}

module switch_internal() {
    switches_all()
    switch_hole(expansion = true);
    
    fx()
    reflect()
    stabiliser_cutout_internal();
}

module switch_artwork() {
    switches_all()
    switch_hole(expansion = true);
    
    fx()
    reflect()
    stabiliser_cutout_artwork();
}

// ENCODERS

module encoder_pins() {
    encoders() {
        // A/B/Gnd
        hull() {
            translate([encoder_hull[0], encoder_hull[1]])
            circle(d=encoder_hull[2], center = true);
            translate([-encoder_hull[0], encoder_hull[1]])
            circle(d=encoder_hull[2], center = true);
        }
        for(hole = encoder_holes) {
            translate([hole[0], hole[1]])
                circle(d=hole[2], center = true);
        }
    }
}

module board_encoders_only() {
    difference() {
        board();
        encoder_cutouts();
    }
}

module enc_smooth() {
    r = 3;
    intersection() {
        // the rounded bits
        difference() {
            board_encoders_only();
            minkowski() {
                offset(delta=-r)
                board_encoders_only();
                
                circle(r=r);
            }
        }
        // only affect the encoders
        encoders()
        square(encoder_radius*2.5, center=true);
    }
}

module encoder_cutouts(smooth = false) {
    encoders()
    circle(r=encoder_radius, center = true);
    
    // smooth harsh edges so you don't cut yourself
    if(smooth) {
        enc_smooth();
    }
}

// MCU/USB HOLE

module mcu_cutout() {
    corners = [[-17, -40],[-16, -1]];
    polygon([[corners.x[0], corners.y[0]],
             [corners.x[0], corners.y[1]],
             [corners.x[1], corners.y[1]],
             [corners.x[1], corners.y[0]]]);
}

module usb_cutout() {
    wall_strength = 6; // bolt + half on either side
    w = usb_dims.x / 2;
    h = wall_strength + fudge*2;
    curve_width = h / 2;
    
    translate(usb_pos)
    reflect()
    translate([0, -h, 0]) { // USB connector origin is at board edge
        translate([0,-(usb_dims.y-h)])
        square([w, usb_dims.y]);
        
        translate([w, curve_width, 0])
        difference() {
            square(curve_width);
            translate([curve_width, 0, 0])
                circle(r=curve_width);
        }
    }
}

// LEDs

module led_cutouts() {
    leds()
    translate(-led_size/2)
    square(led_size);
}

// MACRO BUTTON

module macro_internal() {
    translate(macro_pos)
    square(10, center = true);
}

module macro_external() {
    translate(macro_pos)
    circle(d=macro_diam);
}