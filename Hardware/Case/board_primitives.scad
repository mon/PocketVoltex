use <utils.scad>

black = [0.1,0.1,0.1];
blue = [0,0.6,1];
// x, y, keycap width in units, colour, text
bt_map = [[0,0, 1, blue, "START"],
           [-42.862,-37.592, 1.25, "white", "BT-A"],
           [ 42.862,-37.592, 1.25, "white", "BT-D"],
           [-14.287,-37.592, 1.25, "white", "BT-B"],
           [ 14.287,-37.592, 1.25, "white", "BT-C"]];
fx_map = [[-33.3375,-75.692, 2.25, black, "FX-L"],
           [ 33.3375,-75.692, 2.25, black, "FX-R"]];

bolt_map = [[37.4, 6.65],
         [80.5, -15.5],
         [80.5, 16.5],
         [52.732, -82.792]
         ];
encoder_map = [[-73,0.5],
            [ 73,0.5]];
// Not the encoders themselves, but the cutout that surrounds them
encoder_radius = 13;
// and for the board outline
encoder_radius_board = 22.222;

// encoder solder points hit the case unless we cut these out
// x, y, hole size
encoder_holes = [[-8, 0, 4],
                [ 8, 0, 4]];
// the 3 A/B/Gnd connections
// x (mirrored), y, hole size
encoder_hull = [5, -10.5, 3];

// x, y, rotation
led_map = [[47.1, -1.7, 0],
        [65, -30, 90],
        [57, -68, 45],
        [12, -80, 0]];
led_size = [7.5, 6.5];
                
macro_pos = [0,-75.692];
macro_diam = 4.8;

usb_dims = [11, 12];
usb_pos = [-26.52, 10.17];

// So you get a little clear acrylic edge and it looks nice
top_plate_shrink = 1;

fudge = 0.2;

bolt_diam = 2;
bolt_fudge = 0.1;
bolt_size = bolt_diam + bolt_fudge;
spacer_size = bolt_size + 1.7 + fudge; // extra wiggle

module bolts() {
    reflect([1,0,0])
    for(bolt = bolt_map) {
        translate(bolt)
        children();
    }
}

module bolts_external() {
    bolts()
    circle(d=bolt_size);
}

module bolts_internal() {
    bolts()
    circle(d=spacer_size);
}

module switches(map, apply_scaling, colour) {
    for(sw = map) {
        color(colour ? sw[3] : "gray")
        translate([sw.x,sw.y])
        scale([apply_scaling ? sw[2] : 1,1])
        children();
    }
}

module bt(apply_scaling = false, colour = true) {
    switches(bt_map, apply_scaling, colour)
        children();
}

module fx(apply_scaling = false, colour = true) {
    switches(fx_map, apply_scaling, colour)
        children();
}

module switches_all(apply_scaling = false, colour = true) {
    switches(fx_map, apply_scaling, colour)
        children();
    switches(bt_map, apply_scaling, colour)
        children();
}

module encoders() {
    for(enc = encoder_map) {
        translate(enc)
        children();
    }
}

module leds() {
    reflect([1,0,0])
    for(led = led_map) {
        translate([led.x, led.y])
        rotate([0,0,led[2]])
        children();
    }
}