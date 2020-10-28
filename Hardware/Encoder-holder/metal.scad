include <imports/dimlines.scad>
use <utils.scad>

$fn = 128;

// 0.2 a little less for tight hold
encoder = [16+0.2, 16, 6.5 - 0.2];
encoder_diam = 26;
side_cut = 0;
side_cut_height = 1.5;
side_cut_flat = 0.5;
hole_cut = 9.3;

material_thick = 0.8;

wing_rounding = 2;
hole_size = 2.1;
hole_clearance = 1.5;
hole_offset = [2.75+0.3,0,0];
wing_length = encoder.x*2; // will be trimmed

DIM_LINE_WIDTH = 0.17;
DIM_FONTSCALE = DIM_LINE_WIDTH * 0.4;

echo(wing_length);

module encoder_model() {
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
}

module xycube(dims) {
    translate([0,0,dims.z/2])
    cube(dims, center=true);
}

module main_bit() {
    intersection() {
        difference() {
            union() {
                xycube(encoder + [material_thick*2, 0, material_thick]);
                
                //color("green")
                xycube([
                    wing_length*2 + encoder.x + material_thick*2,
                    encoder.y,
                    material_thick
                ]);
            }
            // offsets are for nice f5 render
            translate([0,0,-1])
            xycube(encoder + [0,1,1]);
            
            // cutouts
            translate([0,0,-0.1])
            xycube([
                encoder.x+side_cut_flat*2+material_thick*2,
                side_cut,
                side_cut_height-0.1]);
            cylinder(d=hole_cut, h=encoder.z*2);
            
            screw_off = [
                hole_offset.x + encoder.x/2 - 0.2,
                hole_offset.y,
                0   
            ];
            echo("Screw offset from center", screw_off);
            // screws
            reflect([1,0,0])
            reflect([0,1,0])
            translate(screw_off + [0,0,-0.1])
            cylinder(d = hole_size, h = material_thick*2);
            
            
            reflect([1,0,0])
            reflect([0,1,0])
            translate(screw_off + [0,0,material_thick])
            #cylinder(d = 3.8, h = material_thick*2);
        }
        
        cylinder(d = encoder_diam, h = encoder.z*2);
    }
}

module dims() {
    translate([-encoder.x/2,-encoder.y/2,0])
    rotate([90,0,0])
    dimensions(encoder.x);
    
    translate([encoder.x/4,-encoder.y/2,encoder.z])
    rotate([90,90,0])
    dimensions(encoder.z);
    
    translate([encoder.x/2+material_thick,-encoder.y/2,6])
    rotate([90,0,90])
    dimensions(encoder.y);
    
    translate([encoder.x/2+material_thick,-side_cut/2,1.5])
    rotate([90,0,90])
    dimensions(side_cut);
    
    translate([encoder.x/2+material_thick,side_cut/2+0.5,side_cut_height])
    rotate([90,90,90])
    dimensions(side_cut_height);
    
    translate([-hole_cut/2,0,encoder.z+material_thick])
    dimensions(hole_cut);
    
    leader_line(25, encoder_diam/2, 5, 5, text = str(encoder_diam), do_circle=true);
    
    screw_off = [
        hole_offset.x + encoder.x/2 + material_thick,
        hole_offset.y,
        0   
    ];
    
    translate(screw_off + [0,0,material_thick])
    leader_line(25, hole_size/2, 5, 4, text = str(hole_size), do_circle=true);
    
    translate([encoder.x/2 + material_thick,2,material_thick+encoder.z+0.1])
    dimensions(side_cut_flat, loc = DIM_OUTSIDE);
}

//color("gray")
//encoder_model();

//color("black")
//dims();

main_bit();