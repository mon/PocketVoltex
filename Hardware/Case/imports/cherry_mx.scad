$fn=60;
/**
a cherry mx switch.

most of the measurements done with a caliper. some taken from
http://geekhack.org/index.php?topic=47744.0

This is just to illustrate and to drop in a a gross reference. It is mostly artistic apart from the steam and mounting plate dimensions
*/

module cherry_mx_model() {

    translate([0,0,14.85])
    {

        //1. steam
        color("brown"){
            //1.1. l-r tab is 1.35mm
            translate([0,0,-3.62/2])
                cube([1.35,4.5,3.62], center=true);
            //1.2. f-b tab is 1.15mm. it has a smal notch that i will ignore.
            translate([0,0,-3.62/2])
                cube([4.5,1.15,3.62], center=true);
            //1.3. base. it has a chamfered top that i will ignore.
            translate([0,0,-5.62])
                cube([7.2,5.56,4], center=true);
        }
        // 2. top
        color("grey"){
            difference(){
                // make a trapezoid with the general shape (volume?) of the top
                hull(){
                    translate([0,0,-4]) //distance from top of switch... some i measured 3.9 others 4.2... so leaving at 4
                        cube([9.87,10.62,0.1], center=true);
                    translate([0,0,-4 -5.2]) // bottom has a measured 5.3... so move 5.2 and use the 0.1 bellow
                        cube([14.58,14.58,0.1], center=true);
                }

            // and subtract:
            // the front led. NOTE: totally off... measured by eye. just for astetics
            // adding just so there is a visual cue of the direction
                translate([0,-4.7,-6])
                    cylinder(r=3/2, h=6, center=true);
                translate([0,-5.5,-6])
                    cube([8,4,5], center=true);
            // the four corners
                // TODO waste of time? this is all for looks, you shouldn't invade any of that space anyway...
            }
        }

        // 3. bottom
        color("green")
            // 3.1 main body volume
            hull(){
                    translate([0,0,-4 -5.3]) //steam + top
                        cube([13.98,13.98,0.1], center=true);
                    translate([0,0,-4 -5.3 -2.2]) //steam + top + straigth part
                        cube([13.98,13.98,0.1], center=true);
                    translate([0,0,-4 -5.3 -5.5]) //steam + top + bottom (measured 5.5)
                        cube([12.74,13.6,0.1], center=true);
            }
            // 3.2 tabs
            // note: only measured the lenght, if they are slightly off-center, this will be all wrong :)
            color("black")
            difference(){
                translate([0,0,-4 -5.3 -0.82/2]) //steam + top
                    cube([15.64,15.64,0.82], center=true);
                translate([0,0,-4 -5.3 -0.82/2  ]) // front-back cut
                    cube([5.64,20,0.82 +2], center=true);
                translate([0,0,-4 -5.3 -0.82/2  ]) //side cut
                    cube([20,11.64,0.82 +2], center=true);
            }

            // 3.3 tab (plate snap on). to use this mechanically, you have to take into account the bending (as it will move the bottom part slightly up...) just for gross reference here for now
            color("white"){
                // 3.3.1 top
                translate([0,0,-4 -5.3 -0.82/2  ]) // front-back cut
                    cube([1.82,16.33,0.82], center=true);
                // 3.3.2 bottom
                difference(){
                    hull(){
                        translate([0,0,-4 -5.3 -0.82/2 -1.76  ]) // front-back cut
                            cube([3.65,14,0.1], center=true);
                        translate([0,0,-4 -5.3 -0.82/2 -2.2  ]) // front-back cut
                            cube([3.65,14.74,0.1], center=true);
                        translate([0,0,-4 -5.3 -0.82/2 -2.89  ]) // front-back cut
                            cube([3.65,14,0.1], center=true);
                    }
                    translate([0,0,-4 -5.3 -0.82/2 -1.76   ]) // front-back cut
                        cube([2.2,20,4], center=true);

                }
            }

            // 4. bottom guides
            // again, i'm assuming everything is centered...
            color("darkGreen"){
                // 4.1 cylinder
                translate([0,0,-4 -5.3 -5.5 -2/2]) //steam + top + bottom (measured 5.5)
                    cylinder(r=3.85/2, h=2, center=true);
                translate([0,0,-4 -5.3 -5.5 -2 -1/2]) //steam + top + bottom (measured 5.5)
                    cylinder(r2=3.85/2, r=2.8/2, h=1, center=true);
                // 4.2 PCB pins
                translate([4.95,0,-4 -5.3 -5.5 -2/2]) //steam + top + bottom (measured 5.5)
                    cylinder(r=1.6/2, h=2, center=true);
                translate([4.95,0,-4 -5.3 -5.5 -2 -1/2]) //steam + top + bottom (measured 5.5)
                    cylinder(r2=1.6/2, r=1/2, h=1, center=true);
                translate([-4.95,0,-4 -5.3 -5.5 -2/2]) //steam + top + bottom (measured 5.5)
                    cylinder(r=1.6/2, h=2, center=true);
                translate([-4.95,0,-4 -5.3 -5.5 -2 -1/2]) //steam + top + bottom (measured 5.5)
                    cylinder(r2=1.6/2, r=1/2, h=1, center=true);
            }

            // 5. pins
            color("orange"){
                translate([-3.77,2.7,-4 -5.3 -5.5 -3.1/2]) //steam + top + bottom (measured 5.5)
                    cube([.86, 0.2,3.1], center=true);
                translate([2.7,5.2,-4 -5.3 -5.5 -3.1/2]) //steam + top + bottom (measured 5.5)
                    cube([.86, 0.2,3.1], center=true);
            }

    }
}
