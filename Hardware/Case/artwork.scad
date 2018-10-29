use <imports/font.ttf>
use <imports/dimlines.scad>

include <board_primitives.scad>
use <board_shape.scad>
use <case.scad>
use <utils.scad>

$fn = 128;

// palette, adjust actual values if needed
black = "black";
white = "white";
grey = "gray";
blue = [0.15,0.5,0.75,1];
pink = [0.86,0.24,0.46,1];
orange = "orange";

border_offset = 1;

// enabled in commandline flags
enable_bleed = 0;
module bleed_trim() {
    if(enable_bleed) {
        board();
    } else {
        art_outline();
    }
}

module box_polygon(width, height, inset_len_bot, inset_len_top,
        inset_len_side, inset = 1.6) {
    // always nice angles
    inset_begin = inset > 0 ? inset : -inset;
    inset_end   = inset > 0 ? -inset : inset;
    bot_off = (width - inset_len_bot) / 2;
    top_off = (width - inset_len_top) / 2;
    side_off = (height - inset_len_side) / 2;
    translate([-width/2, -height/2])
    polygon([
        // bottom
        [0,0],
        [bot_off,0],
        [bot_off+inset_begin, inset],
        [width-bot_off+inset_end,inset],
        [width-bot_off,0],
        [width,0],
        // right
        [width, side_off],
        [width-inset, side_off+inset_begin],
        [width-inset, height-side_off+inset_end],
        [width, height-side_off],
        [width, height],
        // top
        [width-top_off, height],
        [width-top_off+inset_end, height-inset],
        [top_off+inset_begin, height-inset],
        [top_off,height],
        [0, height],
        // left
        [0, height-side_off],
        [inset, height-side_off+inset_end],
        [inset, side_off+inset_begin],
        [0, side_off],
    ]);
}

module box_outline(thick, width, height, inset_len_bot, inset_len_top,
        inset_len_side, inset = 1.6) {
    difference() {
        box_polygon(width, height, inset_len_bot, inset_len_top,
        inset_len_side, inset);
        offset(-thick)
        box_polygon(width, height, inset_len_bot, inset_len_top,
        inset_len_side, inset);
    }
}

btbox_width = 125;
btbox_height = 35;
btbox_inset_len_bot = 49.5;
btbox_inset_len_top = 69;
btbox_inset_len_side = 25;
module bt_box_polygon() {
    translate([0, bt_map[1][1]-1])
    box_polygon(btbox_width, btbox_height, btbox_inset_len_bot,
        btbox_inset_len_top, btbox_inset_len_side);
}
module bt_box_outline() {
    translate([0, bt_map[1][1]-1])
    box_outline(0.8, btbox_width, btbox_height, btbox_inset_len_bot, 
        btbox_inset_len_top, btbox_inset_len_side);
}

fxbox_width = 44;
fxbox_height = 19;
fxbox_inset_len_bot = 20;
fxbox_inset_len_top = 20;
fxbox_inset_len_side = 10;
module fx_box_polygon() {
    box_polygon(fxbox_width, fxbox_height, fxbox_inset_len_bot,
        fxbox_inset_len_top, fxbox_inset_len_side);
}
module fx_box_outline() {
    box_outline(0.8, fxbox_width, fxbox_height, fxbox_inset_len_bot, 
        fxbox_inset_len_top, fxbox_inset_len_side);
}

module fx_inner() {
    intersection() {
        fx()
        offset(-1.6)
        fx_box_polygon();
        
        bleed_trim();
    }
}

module fx_outline() {
    intersection() {
        fx()
        fx_box_outline();
        
        bleed_trim();
    }
}

module fx_keepout() {
    fx()
    fx_box_polygon();
}

module sdvx_text(string, size=3) {
    text(string, font="Digital:style=Regular", size=size, halign="center", valign="center");
}

module tick(width, height, radius) {
    translate([-width/2,radius]) {
        //square([width,height]);
        square([width, height-width]);
        translate([width/2,height-width])
        circle(d=width);
    }
}

module knob_highlights(offset) {
    intersection() {
        bleed_trim();
        translate(offset)
        ring(encoder_radius_board-top_plate_shrink, 3);
    }
}

module knob_ticks() {
    ticks_radius = encoder_radius + 1.9;
    ticks = [
    // ticks per rev, width, height
        [128, 0.2, 0.75],
        [64, 0.2, 1.5],
        [16, 0.4, 2],
        [8,  0.8,   2.5],
        [4,  1,   3.3],
    ];
    intersection() {
        bleed_trim();
        union() {
            encoders()
            ring(encoder_radius+top_plate_shrink*2, 0.4);
            
            for(t = ticks) {
                interval = 360/t[0];
                for(i = [0:t[0]-1]) {
                    rot = interval * i;
                    encoders()
                    rotate([0,0,rot])
                    tick(t[1], t[2], ticks_radius);
                }
            }
        }
    }
}

module perspective_mesh(thick, num_horiz, num_vert, bottom_width, top_width, top_height) {
    translate([-bottom_width/2,0])
    for(x = [0:num_horiz]) {
        logguh = ((x / num_horiz) * 9) + 1; // input 1-10 so log is 0-1
        height = top_height * log(logguh);
        polygon([
            [0, height],
            [bottom_width, height],
            [bottom_width, height+thick],
            [0, height+thick]
        ]);
    }
    top_offset = (bottom_width - top_width) / 2;
    translate([-bottom_width/2,0])
    for(y = [0:num_vert]) {
        startx = bottom_width / num_vert * y;
        endx = top_width / num_vert * y;
        polygon([
            [startx, 0],
            [endx - thick/2 + top_offset, top_height],
            [endx + thick/2 + top_offset, top_height],
            [startx+thick, 0]
        ]);
    }
}

module labels() {
    // BT
    for(s = [ for (i = [1:4]) bt_map[i] ]) {
        translate([s.x, s.y - 13])
        sdvx_text(s[4]);
    }
    // FX
    for(s = fx_map) {
        translate([s.x, s.y + 13])
        sdvx_text(s[4]);
    }
    // START
    s = bt_map[0];
    translate([s.x + 0.15, s.y - 11.5])
    sdvx_text(s[4]);
    
    // MACRO
    translate(macro_pos + [0,3.5])
    sdvx_text("MACRO", 1.5);
    
    // VOL
    translate([0,-24]) {
        translate(encoder_map[0] + [0.5,0])
        sdvx_text("VOL-L", 4);
        translate(encoder_map[1] + [0,0])
        sdvx_text("VOL-R", 4);
    }
}

module start_arrows() {
    tri_base = 7;
    tri_height = 2.5;
    s = bt_map[0];
    for(i = [-1:2:1]) {
        translate([s.x + i*12, s.y - tri_base/2])
        polygon([
            [0,0],
            [0, tri_base],
            [tri_height*i, tri_base/2]
        ]);
    }
}

module start_necklace_half() {
    thick = 1.5;
    top_thick = 6;
    bottom_y = -15.5;
    pivot_point = 30;
    top_x = 48.5;
    top_y = 10;
    
    intersection() {
        polygon([
            [0,bottom_y],
            [pivot_point+thick, bottom_y],
            [top_x+top_thick/2, top_y],
            [top_x-top_thick/2, top_y],
            [pivot_point, bottom_y+thick],
            [0, bottom_y+thick],
        ]);
        bleed_trim();
    }
}

module start_necklace() {
    start_necklace_half();
    mirror([1,0,0])
    start_necklace_half();
}

module logo() {
    w = 16;
    translate([0,-82.9]) {
        box_outline(0.3, 23, 2.0, w, w, 2, inset = -0.5);
        sdvx_text("POCKET   VOLTEX", 1.3);
    }
}

module macro_arrows() {
    tri_w = 2;
    tri_h = 1;
    tri_off = tri_w/2;
    
    translate(macro_pos) {
        for(r = [0:180:180]) {
            rotate([0,0,r])
            translate([macro_diam/2 + 0.6,0])
            polygon([
                [0,0],
                [tri_h, tri_off],
                [tri_h, -tri_off]
            ]);
        }
    }
}

// this is actually straight up disgusting
module angled_lines(segments, offset = [0,0], prev) {
    num_segs = len(segments);
    if(num_segs > 0) {
        s = segments[0];
        difference() {
            translate(offset) {
                difference() {
                    rotate([0, 0, s[2]])
                    translate([-s.y/2,-s.y/2])
                    difference() {
                        // extra len for cutting off
                        square([s.x+s.y, s.y]);
                    }
                    if(prev == undef || s.y >= prev.y) {
                        rotate([0,0,90+prev[2]])
                        translate([-s.y,-s.x*2])
                        square([s.y,s.x*4]);
                    }
                }
            }
            if(num_segs > 1 && (prev == undef || s.y >= prev.y)) {
                diff = [cos(s[2])*s.x - s.y/2,sin(s[2])*s.x];
                translate(diff)
                rotate([0,0,90+segments[1][2]])
                translate([segments[1].y/2,-s.x/2])
                square([s.y*2,s.x]);
            }
        }
    }
    if(num_segs > 1) {
        s = segments[0];
        diff = [cos(s[2])*s.x - s.y/2,sin(s[2])*s.x];
        
        angled_lines([ for (i = [1:num_segs-1]) segments[i] ],
            offset + diff,
            s);
    }
}

module wings_half() {
    entry_thick = 5;
    down_thick = 4;
    exit_thick = 5;
    gap = 0.5;
    count = 4;
    
    entry_x = 27;
    entry_y = -2;
    
    down_x = entry_x + 4;
    down_y = entry_y - 35;
    
    exit_x = down_x + 20;
    exit_y = down_y - 5;
    
    offx = down_thick+gap - 2;
    offy = entry_thick+gap;
    
    angle = 55;
    
    difference() {
        intersection() {
            translate([-85,-27])
            union() {
                len1 = 20;
                width1 = 4;
                ang1 = -10;
                ang2 = -85;
                difference() {
                    angled_lines([
                        [len1,width1,ang1],
                        [37,2,-84],
                        [11,2,-20],
                    ]);
                    translate([35,-51])
                    rotate([0,0,13])
                    square([60,25]);
                }
                for(i = [1:3]) {
                    translate([-i*2,-i*4])
                    angled_lines([
                        [len1,width1, ang1 - i*7],
                        [36-i*3,2,ang2+i],
                        [20+i*2,2,-22+i*3],
                    ]);
                }
                /*for(i=[0:count-1]) {
                    translate([i*offx,i*offy])
                    polygon([
                        [0,0],
                        [entry_x, entry_y],
                        [down_x, down_y],
                        [exit_x, exit_y],
                    
                        [exit_x, exit_y+exit_thick],
                        [down_x+down_thick/2, down_y+exit_thick/2],
                        [entry_x, entry_thick+entry_y],
                        [0,entry_thick],
                    ]);
                }*/
            }
            bleed_trim();
        }
        offset(border_offset)
        fx_keepout();
        offset(border_offset)
        bt_box_polygon();
        // kill some bleed in a hacky way
        translate([0,-15])
        fx_keepout();
    }
}

module wings() {
    wings_half();
    mirror([1,0])
    wings_half();
}

/*line(length, width=DIM_LINE_WIDTH,
            height=DIM_HEIGHT,
            left_arrow=false,
            right_arrow=false
            )*/

module dims_single(width, y_offset) {
    line_size = 0.4;
    font_scale = 0.5;
    
    line_start_y = 48;
    line_length = y_offset - line_start_y + 5;
    
    translate([-width/2,-y_offset])
    dimensions(width, line_size, DIM_FONTSCALE=font_scale);
    
    reflect()
    translate([-width/2,-line_start_y])
    rotate([0,0,-90])
    line(line_length, width=line_size);
}

module draw_dimensions() {
    width = board_width();
    small_width = board_width() - top_plate_shrink*2;

    // we want 2D
    projection() {
        dims_single(width, 100);
        dims_single(small_width, 90);
    }
}

module artwork_white() {
    labels();
    logo();
    start_arrows();
    start_necklace();
    wings();
    macro_arrows();
    bt_box_outline();
    fx_outline();
    knob_ticks();
}

module artwork_grey() {
    // mesh. big difference() to keep it out of the other sections
    difference() {
        intersection() {
            bleed_trim();
            translate([0,-90])
            perspective_mesh(0.3, 23, 15/*14*3, 7*3*/, 450, -50, 150);
        }
        offset(border_offset) {
        bt_box_polygon();
        fx_keepout();
        labels();
        start_arrows();
        start_necklace();
        wings();
        rad = encoder_radius_board-top_plate_shrink;
        encoders() circle(r=rad, center = true);
        logo();
        }
    }
}

module artwork_orange() {
    fx_inner();
}

module artwork_blue() {
    knob_highlights(encoder_map[0]);
}
module artwork_pink() {
    knob_highlights(encoder_map[1]);
}

module artwork_2d() {
    artwork_white();
    artwork_grey();
    artwork_orange();
    artwork_pink();
    artwork_blue();
}

module artwork() {
    color(black)
    linear_extrude(0.05)
    bleed_trim();
    
    color(white)
    linear_extrude(0.1)
    artwork_white();
    
    color(grey)
    linear_extrude(0.1)
    artwork_grey();
    
    color(orange)
    linear_extrude(0.1)
    artwork_orange();
    
    color(blue)
    linear_extrude(0.1)
    artwork_blue();
    
    color(pink)
    linear_extrude(0.1)
    artwork_pink();
    
    /*color(black)
    linear_extrude(0.1)
    draw_dimensions();*/
}

// override in commandline
build = "render";
if(build == "render") {
    // correct lighting for fullbright
    //light_from_above()
    artwork();
} else if(build == "black") {
    bleed_trim();
} else if(build == "white") {
    artwork_white();
} else if(build == "grey") {
    artwork_grey();
} else if(build == "orange") {
    artwork_orange();
} else if(build == "blue") {
    artwork_blue();
} else if(build == "pink") {
    artwork_pink();
} else if(build == "cuts") {
    art_outline();
} else if(build == "dimensions") {
    draw_dimensions();
}