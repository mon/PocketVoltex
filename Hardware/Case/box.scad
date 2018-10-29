use <case.scad>;
use <board_primitives.scad>
use <board_shape.scad>

$fn=128;

// to easily automate exports
produce = 0;
if(produce == 0)
    render();
else if(produce == 1)
    bottom_padding();
else if(produce == 2)
    mainCuts();
else if(produce == 3)
    padding();

knobOffset = 83;
knobY = -83;
main_thickness = 42;
bottom_thickness = 5;
padding_thickness = 14;

encoderShaftHole = 5.5;

// to work out viewports for render
//echo(str($vpt[0],",",$vpt[1],",",$vpt[2],",",$vpr[0],",",$vpr[1],",",$vpr[2],",",$vpd));

// x, y, size
finger_holes = [
    [85,  -25, 20],
    [-85, -25, 20],
    [0,    5,   20],
    [0, -85,   20],
];

loosen = 3; // so things aren't impossible to remove
gap = 1; // for the top cover

size = [200, 140];

module mainCuts() {
    difference() {
        translate([0,-30])
        square(size, center = true);
        
        offset(delta = loosen)
        board();
        
        translate([-knobOffset, knobY])
            circle(d = 25 + loosen);
        translate([knobOffset,knobY])
            circle(d = 25 + loosen);

        // not needed any more
        //translate([0, 15])
        //square([80, 3], center = true);
        
        translate([0, 28])
        square([100, 13], center = true);
        
        for(f = finger_holes) {
            translate([f.x,f.y])
            circle(d=f[2]);
        }
    }
}

module padding() {
    difference() {
        offset(delta = -gap)
        board();
        
        encoders()
        circle(d=encoderShaftHole);
    }
}

module bottom_padding() {
    square(size, center = true);
}

module render() {
    full_stack(false);
    
    color([0.3,0.3,0.3,0.9])
    linear_extrude(main_thickness)
        mainCuts();
    
    color([0.3,0.3,0.3,0.9])
    // extra 1 for visualisation purposes
    translate([0,-30,-bottom_thickness-1])
    linear_extrude(bottom_thickness)
    bottom_padding();
    
    color([0.3,0.3,0.3,0.9])
    translate([0, 0, 150]) // raise it up to see the board
    translate([0, 0, 25.5])
    linear_extrude(padding_thickness)
    padding();
    
    translate([-knobOffset, knobY])
        encoder_knob();
    translate([knobOffset,knobY])
        encoder_knob();
}