# openscad will not adjust viewports set in code when rendering from the cmdline
# what a shame! so we do it here

import os
import subprocess
from math import floor, ceil

scad = 'C:\\Program Files\\OpenSCAD\\openscad.exe'
folder = 'outputs'

final_size = [800, 600]
render_size = [x*2 for x in final_size]

def quad_ease_inout(t):
    return 2*t*t if t<.5 else -1+(4-2*t)*t

def interpolate(vec, index_float):
    vec1 = vec[floor(index_float)]
    vec2 = vec[ceil(index_float)]
    percent = index_float % 1
    return [percent*(high-low) + low for low, high in zip(vec1, vec2)]

def thread_call(args):
    print(' '.join(args))
    subprocess.check_output(args)

def gen_args():
    interval = 2
    percents = [i / 100.0 for i in range(0, 100+interval, interval)]

    # vpt[3], vpr[3], vpd[1]
    anim = [
        [0,        -65.9053, 71.5148,  30,  0,  0,     360],
        [-16.5687, -50.6543, 84.6026,  66.5,0, -37.8,  550.777]
    ]

    for p in percents:
        camera = interpolate(anim, quad_ease_inout(p))
        args = [
            scad,
            '-o', "{}/anim_{}.png".format(folder, p),
            '-D', '$t={}'.format(p),
            '--imgsize={}'.format(','.join(str(x) for x in render_size)),
            '--camera={}'.format(','.join(str(x) for x in camera)),
            '--colorscheme=Tomorrow',
            '--projection=p',
            'case.scad',
            '-D', 'build="render"'
        ]
        yield args

from multiprocessing.dummy import Pool as ThreadPool
pool = ThreadPool(4)
pool.map(thread_call, gen_args())

def call_and_log(cmd):
    print(cmd)
    os.system(cmd)
# openscad has no anti-aliasing so let's just do it ourselves
pngs = [arg[2] for arg in gen_args()]
pngs = ['-delay 200', pngs[0], '-delay 5'] + pngs[1:-1] + ['-delay 200', pngs[-1]]
call_and_log('bash -c "convert {pngs} -resize {img_size} \\"{folder}/render_animation.gif\\""'.format(
    pngs=' '.join(pngs),
    folder=folder,
    img_size='x'.join(str(x) for x in final_size)))
call_and_log('bash -c "rm {folder}/anim_*.png"'.format(folder=folder))
