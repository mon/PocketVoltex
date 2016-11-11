$fn = 64;

switches = [[0,0],
            [-42.862,-37.592],
            [ 42.862,-37.592],
            [-14.287,-37.592],
            [ 14.287,-37.592],
            [-28.5,-75.692],
            [ 28.5,-75.692]];
bolts = [[-47.9, 8.05],
         [ 47.9, 8.05],
         [-72, -16.9],
         [ 72, -16.9],
         [-72, -49.9],
         [ 72, -49.9],
         [-45, -75.387],
         [ 45, -75.387]];
// Not the encoders themselves, but the circle that surrounds them
encoders = [[-64,0.4],
            [ 64,0.4]];
encoderRadius = 14.278;

// x, y, hole size
encoderHoles = [[71.4, 0.4, 3.2],
                [69, -10.1, 1.6]];

usbWidth = 9;
usbPos = [-21.311, 10.16];
// bottom to top
plate_thickness = [3, 3, 1.6, 3, 1.6];

fudge = 0.2;

// So you get a little clear acrylic edge and it looks nice
wallOverlap = 1;

boltDiam = 3;
boltFudge = fudge;
boltSize = boltDiam + boltFudge;

boltExpand = boltDiam/2;

// bolt + half on either side
wallStrength = boltDiam * 2;

// 0 = full spec, 1 = half spec, 2 = plain square
holetype = 0;

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

module switches() {
    for(sw = switches) {
        translate([sw[0], sw[1]])
        switch(holetype);
    }
}

module bolts() {
    for(bolt = bolts) {
        translate([bolt[0], bolt[1]])
        circle(d = boltSize, center = true);
    }
}

module bolts_expansion() {
    offset(r = boltExpand)
    bolts();
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

// don't do the same thing twice
module usb_half() {
    w = usbWidth / 2;
    h = wallStrength + fudge;
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
    import("BOARD_SHAPE.dxf");
}

module board_encoderhole() {
    difference() {
        board();
        encoders();
    }
}

module top_plate() {
    difference() {
        offset(delta = -wallOverlap)
        difference() {
            board();
            encoders();
        }
        switches();
        bolts();
    }
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
            bolts_expansion();
        }
        bolts();
    }
    import("RING_TOP_SUPPORTS.dxf");
}

module pcb() {
    difference() {
        board();
        bolts();
    }
}

module bottom_ring() {
    difference() {
        union() {
            difference() {
                board();
                offset(delta = -wallStrength)
                    board();
            };
            bolts_expansion();
        }
        bolts();
        encoderHoles();
        translate([usbPos[0], usbPos[1], 0])
        usb();
    }
    import("RING_BOT_SUPPORTS.dxf");
}

module bottom_plate() {
    difference() {
        board();
        bolts();
    }
}

// This is kinda disgusting
module full_stack() {
    /*color([1, 1, 1, 0.2])
    linear_extrude(plate_thickness[0])
        bottom_plate();*/
    translate([0, 0, plate_thickness[0]]) {
        color([1, 1, 1, 0.2])
        linear_extrude(plate_thickness[1])
            bottom_ring();
        translate([0, 0, plate_thickness[1]]) {
            color([1,1,1, 1])
            linear_extrude(plate_thickness[2])
                pcb();
            translate([0, 0, plate_thickness[2]]) {
                color([1, 1, 1, 0.2])
                linear_extrude(plate_thickness[3])
                    top_ring();
                translate([0, 0, plate_thickness[3]]) {
                    color([0,0,0, 0.5])
                    linear_extrude(plate_thickness[4])
                        top_plate();
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