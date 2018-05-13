black = [0.1,0.1,0.1];
blue = [0,0.6,1];
// x, y, keycap width in units, rotation, colour
switches = [[0,0, 1, 0, blue],
            [-42.862,-37.592, 1.25, 0, "white"],
            [ 42.862,-37.592, 1.25, 0, "white"],
            [-14.287,-37.592, 1.25, 0, "white"],
            [ 14.287,-37.592, 1.25, 0, "white"],
            [-37.9,-70.8, 1.75, -22.5, black],
            [ 37.9,-70.8, 1.75, 22.5, black]];
bolts = [[30.025, 6.65],
         [71, -16.925],
         [71, 16.5],
         [42.732, -82.791]
         ];
encoders = [[-64,0.4],
            [ 64,0.4]];
// Not the encoders themselves, but the cutout that surrounds them
encoderRadius = 13;
// and for the board outline
encoderRadiusBoard = 22.222;

// encoder solder points hit the case unless we cut these out
// x, y, hole size
encoderHoles = [[-8, 0, 4],
                [ 8, 0, 4]];
// the 3 A/B/Gnd connections
// x (mirrored), y, hole size
encoderHull = [5, -10.5, 3];

// x, y
leds = [[-39.9, -1.4],
        [-53.6, -26.2],
        [-54.8, -57.9],
        [-17.3, -75.7]];
ledDims = [6.5, 6.5];
                
macroHole = [0,-75.692];
macroDiam = 3;

usbWidth = 9;
usbPos = [-21.311, 10.3];