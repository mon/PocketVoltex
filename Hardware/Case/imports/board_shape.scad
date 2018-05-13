include <board_dimensions.scad>

$fn = 128;

module board_half() {
    // 1mm gap for the important bits
    top_gap = 10.16;
    encoder_side_gap = 11;
    // arbitrary
    height = 96.64;
    angled_xoff = 44.263;
    
    // calculated
    width = abs(encoders[0][0]) + encoder_side_gap;
    
    // main body
    translate([0, -height + top_gap])
    difference() {
        square([width, height]);
        
        translate([angled_xoff,0])
        rotate([0,0,-45])
        square([width, height]);
    }
    
    // encoder bits
    difference() {
        translate(encoders[1])
        circle(r=encoderRadiusBoard);
        
        translate([width, -height/2])
        square([width, height]);
    }
}

module board_shape() {
    board_half();
    mirror([1,0,0])
    board_half();
}

board_shape();