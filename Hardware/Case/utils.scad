
// like mirror, but leaves the original too
module reflect(axis) {
    children();
    
    mirror(axis)
    children();
}

module rounded_square(w, h, r) {
    translate([r,r])
    minkowski() {
        square([w,h]-[r*2,r*2]);
        circle(r=r);
    }
}

module ring(outer_radius, thickness) {
    difference() {
        circle(r=outer_radius);
        circle(r=outer_radius-thickness);
    }
}

// to get consistent colours on our render
module light_from_above() {
    rotate([-40,0,0]) scale([1,1/cos(40),0.01])children();
} 

// sum from 0 to end, like python sum(v[:end])
function sum(v, end=-1, i=0, r=0) = (i<len(v) && (end < 0 || i < end)) ? sum(v, end, i+1, r+v[i]) : r;

function lookup3D(index, v) = [
    lookup(index, [for (el = v) [el[0], el[1].x]]),
    lookup(index, [for (el = v) [el[0], el[1].y]]),
    lookup(index, [for (el = v) [el[0], el[1].z]]),
];
    
function quad_ease_inout(t) = t<.5 ? 2*t*t : -1+(4-2*t)*t;