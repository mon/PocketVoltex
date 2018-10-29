include <board_primitives.scad>
use <utils.scad>
use <cutouts.scad>

encoder_side_gap = 11;
function board_width() = (abs(encoder_map[0][0]) + encoder_side_gap)*2;

module board_half() {
    // 1mm gap for the important bits
    top_gap = 10.16;
    
    // arbitrary
    height = 96.64;
    angled_xoff = 55;
    
    // calculated
    width = board_width()/2;
    
    // main body
    translate([0, -height + top_gap])
    difference() {
        square([width, height]);
        
        translate([angled_xoff,0])
        rotate([0,0,-35])
        square([width, height]);
    }
    
    // encoder bits
    difference() {
        translate(encoder_map[1])
        circle(r=encoder_radius_board);
        
        translate([width, -height/2])
        square([width, height]);
    }
}

module board() {
    reflect([1,0,0])
    board_half();
}

module top_outline() {
    difference() {
        board();
        encoder_cutouts(smooth = true);
    }
}

board();