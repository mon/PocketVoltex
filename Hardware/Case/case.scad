use <imports/cherry_mx.scad>
include <imports/mx_keycap.scad>
key_profile_index = 11;

$fn = 64;

// x, y, keycap width in units, isBlack
switches = [[0,0, 1],
            [-42.862,-37.592, 1.25],
            [ 42.862,-37.592, 1.25],
            [-14.287,-37.592, 1.25],
            [ 14.287,-37.592, 1.25],
            [-28.5,-75.692, 1.75, 1],
            [ 28.5,-75.692, 1.75, 1]];
bolts = [[30.025, 6.65],
         [71, -16.925],
         [71, 16.5],
         [42.732, -82.791],
         // Middle hole, remove if not needed
         //[0, -37.592]
         ];
// Not the encoders themselves, but the circle that surrounds them
encoders = [[-64,0.4],
            [ 64,0.4]];
encoderRadius = 13;

// encoder solder points hit the case unless we cut these out
// x, y, hole size
encoderHoles = [[-8, 0, 4],
                [ 8, 0, 4]];
// the 3 A/B/Gnd connections + room for their decoupling caps
// x (mirrored), y, hole size
encoderHull = [5, -11.2, 5];

// x, y, rotation
leds = [[-26.475, -13.625, 35],
        [-55.5, -37.25, 90],
        [-47, -57.25, 135],
        [-13.5, -65, 180]];
ledDims = [6, 3];
ledTopLength = 10;
                
macroHole = [0,-75.692];
macroDiam = 3;

usbWidth = 9;
usbPos = [-21.311, 10.3];
// bottom to top, last 2 are white/black sections of the 1 plate
plate_thickness = [3, 3, 1.6, 3, 1.5, 0.1];

fudge = 0.2;

// So you get a little clear acrylic edge and it looks nice
wallOverlap = 1;

boltDiam = 2;
boltFudge = 0.1;
boltSize = boltDiam + boltFudge;
spacerSize = boltSize + 1;

boltExpand = boltDiam;

// bolt + half on either side
wallStrength = 6;

// 0 = full spec, 1 = half spec, 2 = plain square
holetype = 2;

// For the animation
explodeFactor = 20;

module switch(holetype){
    //Hole size, from Cherry MX data sheet
    holesize=14;
    //height of switch clasp cutouts
    cutoutheight = 3;
    //width of switch clasp cutouts
    cutoutwidth = 1;
    translate([-holesize/2, -holesize/2])
	if (holetype == 0){
		union(){
			square([holesize,holesize]);

			translate([-cutoutwidth,1])
			square([holesize+2*cutoutwidth,cutoutheight]);

			translate([-cutoutwidth,holesize-1-cutoutheight])
			square([holesize+2*cutoutwidth,cutoutheight]);
		}
	} else if (holetype == 1){
		union(){
			square([holesize,holesize]);

			translate([-cutoutwidth,1])
			square([holesize+2*cutoutwidth,holesize-2]);
		}
	} else {
		square([holesize,holesize]);
	}
}

module switches_fx() {
    for(i = [5:6]) {
        translate([switches[i][0], switches[i][1]])
        switch(holetype);
    }
}

module switches() {
    for(sw = switches) {
        translate([sw[0], sw[1]])
        switch(holetype);
    }
}

module switch_models() {
    for(sw = switches) {
        translate([sw[0], sw[1]])
        cherry_mx_model();
    }
}

module switch_keycaps() {
    for(sw = switches) {
        color(sw[3] ? [0.1,0.1,0.1] : [1,1,1])
        translate([sw[0], sw[1], 11.5])
        scale([sw[2],1,1])
        key();
    }
}

module bolt_spacers() {
    for(bolt = bolts) {
        translate([bolt[0], bolt[1]])
        circle(d = spacerSize, center = true);
        
        // mirror
        translate([-bolt[0], bolt[1]])
        circle(d = spacerSize, center = true);
    }
}

module bolts() {
    for(bolt = bolts) {
        translate([bolt[0], bolt[1]])
        circle(d = boltSize, center = true);
        
        // mirror
        translate([-bolt[0], bolt[1]])
        circle(d = boltSize, center = true);
    }
}

// Only used to see if my distances are right
module bolts_expansion() {
    offset(r = boltExpand)
    bolt_spacers();
}

module encoders() {
    for(enc = encoders) {
        translate([enc[0], enc[1]])
        circle(r=encoderRadius, center = true);
    }
}

module encoder_holes() {
    for(enc = encoders) {
        translate([enc[0], enc[1]]) {
            // A/B/Gnd
            hull() {
                translate([encoderHull[0], encoderHull[1]])
                circle(d=encoderHull[2], center = true);
                translate([-encoderHull[0], encoderHull[1]])
                circle(d=encoderHull[2], center = true);
            }
            for(hole = encoderHoles) {
                translate([hole[0], hole[1]])
                    circle(d=hole[2], center = true);
            }
        }
    }
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
    encoderShaft = 6;
    encoderFlat = 4.5;
    translate([0,0,6.5])
    linear_extrude(20)
    intersection() {
        circle(d = encoderShaft);
        translate([0, (encoderShaft-encoderFlat)/2])
        square([encoderShaft, encoderFlat], center = true);
    }
    
    if(knob) {
        translate([0,0,13.5])
        encoder_knob();
    }
}

module encoder_model_full(knob = true) {
    for(enc = encoders) {
        translate([enc[0], enc[1]])
        encoder_model(knob);
    }
}

module led_holes() {
    led_holes_half();
    mirror([1,0,0])
    led_holes_half();
}

module led_holes_half() {
    for(led = leds) {
        translate([led[0], led[1]])
        rotate([0,0,led[2]]) {
            translate([-ledDims[0]/2, -ledDims[1]/2])
            square([ledDims[0], ledTopLength]);
        }
    }
}

module led_leg_holes() {
    led_leg_holes_half();
    mirror([1,0,0])
    led_leg_holes_half();
}

module led_leg_holes_half() {
    for(led = leds) {
        translate([led[0], led[1]])
        rotate([0,0,led[2]])
        hull() {
            offset = ledDims[1]/2 - ledDims[0]/2;
            translate([offset,0])
            circle(d=3, center = true);
            translate([-offset,0])
            circle(d=3, center = true);
        }
    }
}

// don't do the same thing twice
module usb_half() {
    w = usbWidth / 2;
    h = wallStrength + fudge*2;
    curveWidth = h / 2;
    
    // USB connector origin is at board edge
    translate([0, -h, 0]) {
        square([w, h]);
        
        translate([w, curveWidth, 0])
        difference() {
            square(curveWidth);
            translate([curveWidth, 0, 0])
                circle(r=curveWidth);
        }
    }
}

module usb() {
    usb_half();
    mirror()
        usb_half();
}

module board() {
    import("imports/BOARD_SHAPE.dxf");
}

module board_encoderhole() {
    difference() {
        board();
        encoders();
    }
}

module top_plate_outline() {
    offset(delta = -wallOverlap)
        difference() {
            board();
            encoders();
        }
}

module top_plate_holes() {
    switches();
    bolts();
    translate([macroHole[0], macroHole[1], 0])
    circle(d = macroDiam, center = true);
}

module top_plate() {
    difference() {
        top_plate_outline();
        top_plate_holes();
    }
}

module top_ring_outline() {
    board_encoderhole();
}

// go in intervals of 2 to avoid final implicit union()
// start at 0.3 because laser kerf
module top_ring_engrave() {
    engraveInterval = 0.05;
    wallOverlap = 1;
    for(i = [0.3:engraveInterval*2:wallOverlap+engraveInterval]) {
        difference() {
            offset(delta=-i)
            top_ring_outline();
            
            offset(delta=-(i+engraveInterval))
            top_ring_outline();
        }
    }
}

module top_ring() {
    difference() {
        top_ring_outline();
        switches();
        led_holes();
        translate(macroHole)
            square(10, center = true);
        bolt_spacers();
    }
}

module pcb() {
    difference() {
        board();
        bolt_spacers();
    }
}

module artwork() {
    import("imports/Artwork.dxf");
}

module bottom_ring_outline() {
    difference() {
        board();
        translate(usbPos)
        usb();
    }
}

module mcu_hole() {
    polygon([[-13.3, 3.9],
             [-13.3, -8],
             [-20.15, -17],
             [-28.65, -17],
             [-35.5, -8],
             [-35.5, 3.9]]);
}

module bottom_ring() {
    difference() {
        bottom_ring_outline();
        bolt_spacers();
        encoder_holes();
        led_leg_holes();
        mcu_hole();
        // Room for FX
        switches();
    }
}

module bottom_plate_outline() {
    board();
}

module bottom_plate() {
    difference() {
        board();
        bolts();
    }
}

module art_trimmed() {
    intersection() {
        // Won't work cause I'm an idiot
        //offset(delta = -0.5)
        top_plate();
        artwork();
    }
}

// This is kinda disgusting
module full_stack(knobs = true) {
    // quadratic ease out
    t = $t -1;
    boom = -explodeFactor * (t*t*t*t - 1);
    
    color([1, 1, 1, 0.4])
    linear_extrude(plate_thickness[0])
        bottom_plate();
    translate([0, 0, plate_thickness[0] + boom]) {
        color([0.8, 0.8, 0.8, 0.4])
        linear_extrude(plate_thickness[1])
            bottom_ring();
        translate([0, 0, plate_thickness[1] + boom]) {
            color([1,1,1, 1])
            linear_extrude(plate_thickness[2])
                pcb();
            // not-as-pretty model of macro key
            translate([macroHole[0], macroHole[1], plate_thickness[2]])
            color([1,0.5,0])
            cylinder(h = 5.2, d = macroDiam);
            
            translate([0, 0, plate_thickness[2] + boom]) {
                // pretty models of stuff
                translate([0, 0, boom*2]) {
                    color([0.8,0.8,0.8,1])
                    encoder_model_full(knobs);
                    
                    switch_models();
                    switch_keycaps();
                };
                
                color([1, 1, 1, 0.4])
                linear_extrude(plate_thickness[3])
                    top_ring();
                translate([0, 0, plate_thickness[3] + boom]) {
                    color([1,1,1, 1])
                    linear_extrude(plate_thickness[4])
                        top_plate();
                    translate([0, 0, plate_thickness[4]]) {
                        color([0,0,0, 1])
                        linear_extrude(plate_thickness[5]) {
                            difference() {
                                top_plate();
                                art_trimmed();
                            }
                        }
                    }
                }
            }
        }
    }
    
}

fiducialPoints = [
    [-80, -100],
    [80, -100],
    [-80, 50],
    [80, 50]
];

module fiducials() {
    for(f = fiducialPoints) {
        translate(f)
        square(5, center=true);
    }
}
// override in commandline
build = "render";
enable_fiducials = 0;
if(build == "render") {
    full_stack();
} else if(build == "bottom_plate") {
    bottom_plate();
} else if(build == "bottom_spacer") {
    bottom_ring();
} else if(build == "top_spacer") {
    if(enable_fiducials)
        fiducials();
    top_ring();
} else if(build == "top_spacer_engrave") {
    if(enable_fiducials)
        fiducials();
    top_ring_engrave();
} else if(build == "top_plate") {
    if(enable_fiducials)
        fiducials();
    top_plate();
} else if(build == "top_plate_engrave") {
    if(enable_fiducials)
        fiducials();
    art_trimmed();
}

// for inspecting individual plates
//linear_extrude(plate_thickness[0])
//    bottom_plate();
//linear_extrude(plate_thickness[1])
//    bottom_ring();
//linear_extrude(plate_thickness[3])
//    top_ring();
//translate([0,0,plate_thickness[3]])
//linear_extrude(1)
//    top_ring_engrave();
//linear_extrude(plate_thickness[4])
//    top_plate();