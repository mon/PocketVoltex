use <case.scad>;

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

knobOffset = 74;
knobY = -83;
main_thickness = 37.5;
bottom_thickness = 5;
padding_thickness = 12;

encoders = [[-64,0.4],
            [ 64,0.4]];
encoderShaftHole = 5.5;

finger = 20;
fingerX = -75;
fingerY = -25;

squeeze = 0; // so things are held tight
gap = 1; // for the top cover

size = [180, 140];

module mainCuts() {
    difference() {
        translate([0,-30])
        square(size, center = true);
        
        offset(delta = -squeeze)
        board();
        
        translate([-knobOffset, knobY])
            circle(d = 25 - squeeze);
        translate([knobOffset,knobY])
            circle(d = 25 - squeeze);

        translate([0, 15])
        square([80, 3], center = true);
        
        translate([0, 28])
        square([100, 13], center = true);
        
        translate([fingerX,fingerY])
        circle(d=finger);
        translate([-fingerX,fingerY])
        circle(d=finger);
    }
}

module padding() {
    difference() {
        offset(delta = -gap)
        board();
        
        for(e = encoders) {
            translate(e)
            circle(d=encoderShaftHole);
        }
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
    translate([0, 0, 25.5])
    linear_extrude(padding_thickness)
    padding();
    
    translate([-knobOffset, knobY])
        encoder_knob();
    translate([knobOffset,knobY])
        encoder_knob();
}