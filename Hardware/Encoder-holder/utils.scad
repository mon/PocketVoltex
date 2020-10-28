
// like mirror, but leaves the original too
module reflect(axis) {
    children();
    
    mirror(axis)
    children();
}

module rounded_square(w, h, r, center=false) {
    translate([r,r] - (center ? [w/2,h/2] : [0,0]))
    minkowski() {
        square([w,h]-[r*2,r*2]);
        circle(r=r);
    }
}