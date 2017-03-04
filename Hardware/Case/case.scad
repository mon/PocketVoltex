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
         [42.732, -82.791]];
// Not the encoders themselves, but the circle that surrounds them
encoders = [[-64,0.4],
            [ 64,0.4]];
encoderRadius = 13;

// encoder solder points hit the case unless we cut these out
// x, y, hole size
encoderHoles = [[71.6, 0.4, 3.4],
                [69, -10.1, 1.6]];
                
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

module encoderHoles() {
    encoderHoles_half();
    mirror([1,0,0])
    encoderHoles_half();
}

module encoderHoles_half() {
    for(enc = encoderHoles) {
        translate([enc[0], enc[1]])
        circle(d=enc[2], center = true);
    }
}

module encoder_model() {
    translate([0,0,5.5])
    rotate([90,0,0])
    import("imports/PEC16.stl");
    
    translate([0,0,13])
    cylinder(20, d = 25);
}

module encoder_model_full() {
    for(enc = encoders) {
        translate([enc[0], enc[1]])
        encoder_model();
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
    difference() {
        board_encoderhole();
        offset(delta = -wallStrength)
            board_encoderhole();
        // Room for FX/START
        switches();
    };
    import("imports/RING_TOP_SUPPORTS.dxf");
    bolts_expansion();
}

module top_ring() {
    difference() {
        union() {
            difference() {
                board_encoderhole();
                offset(delta = -wallStrength)
                    board_encoderhole();
                // Room for FX/START
                switches();
            };
            import("imports/RING_TOP_SUPPORTS.dxf");
            bolts_expansion();
        }
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
        union() {
            difference() {
                board();
                offset(delta = -wallStrength)
                    board();
            };
            import("imports/RING_BOT_SUPPORTS.dxf");
            bolts_expansion();
        }
        translate([usbPos[0], usbPos[1], 0])
        usb();
    }
}

module bottom_ring() {
    difference() {
        bottom_ring_outline();
        bolt_spacers();
        encoderHoles();
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
module full_stack() {
    // quadratic ease out
    t = $t -1;
    boom = -explodeFactor * (t*t*t*t - 1);
    
    color([1, 1, 1, 0.4])
    linear_extrude(plate_thickness[0])
        bottom_plate();
    translate([0, 0, plate_thickness[0] + boom]) {
        color([1, 1, 1, 0.4])
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
                    encoder_model_full();
                    
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

//board();
full_stack();
//linear_extrude(plate_thickness[0])
//    bottom_plate();
//linear_extrude(plate_thickness[1])
//    bottom_ring();
//linear_extrude(plate_thickness[3])
//    top_ring();
//linear_extrude(plate_thickness[4])
//    top_plate();