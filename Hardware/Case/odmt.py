#!/usr/bin/python

'''
@license GPLv3
@sources https://github.com/lautr3k/odmt
@author  Sebastien Mischler <skarab>
@author  http://www.onlfait.ch
'''

import argparse, os, sys, fnmatch, ezdxf

# configuration
app_name        = 'odmt'
app_version     = '1.0.0'
app_description = 'OpenSCAD DXF Merge Tool (odmt) - v' + app_version

# default i/o
input  = ['./input']
output = './output/merged.dxf'

# search and ignore files pattern
search = ['*.dxf']
ignore = ['*_ignore_*']

# layers indexed colors
# http://sub-atomic.com/~moses/acadcolors.html
colors = range(10)
colors.extend(range(10, 250, 10))
colors = map(str, colors)

# command line parser
parser = argparse.ArgumentParser(
    prog        = app_name,
    description = app_description)
parser.add_argument('--input', '-i',
    nargs   = '+',
    default = input,
    metavar = 'path',
    help    = 'input file or directory - default: ['
                + ', '.join(input) + ']')
parser.add_argument('--output', '-o',
    default = output,
    metavar = 'path',
    help    = 'output file - default: ' + output)
parser.add_argument('--search',
    nargs   = '+',
    default = search,
    metavar = 'pattern',
    help    = 'search file pattern - default: ['
                + ', '.join(search) + ']')
parser.add_argument('--ignore',
    nargs   = '+',
    default = ignore,
    metavar = 'pattern',
    help    = 'ignored file/directory pattern - default: ['
                + ', '.join(ignore) + ']')
parser.add_argument('--colors',
    nargs   = '+',
    default = colors,
    metavar = 'index',
    help    = 'layers indexed colors - default: ['
                + ', '.join(colors) + ']')
parser.add_argument('--nolayers',
    action = 'store_true',
    help   = 'if set, all files will be merged into the same layer')
parser.add_argument('--version', '-v',
    action  = 'version',
    version = '%(prog)s ' + app_version)

# parse the command line
args = parser.parse_args()

# local variables assignment
input    = args.input
output   = os.path.realpath(args.output)
search   = args.search
ignore   = args.ignore
colors   = map(int, args.colors)
nolayers = args.nolayers

# test output directory
output_dir = os.path.dirname(output)

if os.path.isdir(output_dir) == False:
    print 'output directory not found :', output_dir
    sys.exit(1);

def file_match(input, patterns):
    'return if input match at least one pattern'
    for pattern in patterns:
        if fnmatch.fnmatch(input, pattern):
            return True
    return False

def dxf_search(input):
    'scan an input file or directory for DXF file'
    found   = []
    ignored = []
    if os.path.isdir(input):
        for file in os.listdir(input):
            r = dxf_search(os.path.join(input, file))
            found.extend(r[0])
            ignored.extend(r[1])
    elif os.path.isfile(input):
        if file_match(input, search):
            if file_match(input, ignore):
                ignored.append(input)
            else:
                found.append(input)
        else:
            ignored.append(input)
    return found, ignored

def dxf_parse(file):
    '''
        extract all LINE tags from an OpenSCAD DXF file
        and return an array of POLYLINE points
    '''
    counter    = 0
    points     = []
    last_block = []
    polylines  = []

    with open(file) as f:
        for line in f:
            line = line.strip('\n');

            # block start
            if line == 'LINE':
                counter = 1;
                block   = [[0, 0], [0, 0]];

            # in the block
            if counter > 0:
                # start line
                if counter == 5:
                    block[0][0] = line
                if counter == 7:
                    block[0][1] = line
                # end line
                if counter == 9:
                    block[1][0] = line
                if counter == 11:
                    block[1][1] = line
                # increment
                counter += 1

            # end block
            if counter == 13:
                counter = 0

                # discontinued line
                if len(last_block) and block[0] != last_block[1]:
                    polylines.append(points)
                    points = []

                points.append((block[0][0], block[0][1]))
                points.append((block[1][0], block[1][1]))
                last_block = block

    # return polylines
    if len(points):
        polylines.append(points)
        return polylines

    # no polyline found
    return None

def dxf_merge(files, colors = range(0, 256), nolayer = False):
    'merge DXF file and convert continuous line to polyline.'

    # DXF file
    dwg = ezdxf.new('AC1015')
    msp = dwg.modelspace()

    # layer vars
    layer_num = 1

    layer_name  = 'layer0'
    layer_names = []

    layer_colors = iter(colors)
    layer_color  = next(layer_colors)

    # for each files
    for file in files:
        # layer name
        if nolayers == False:
            layer_name = os.path.basename(file)
            n = layer_name
            i = 1
            while n in layer_names:
                n = layer_name + '_' + str(i)
                i += 1
            layer_names.append(n)
            layer_name = n

        # create layer
        if layer_num < 2 or nolayers == False:
            try:
                dwg.layers.create(
                    name       = layer_name,
                    dxfattribs = {'color': layer_color})
            except AttributeError: # new syntax
                dwg.layers.new(
                    name       = layer_name,
                    dxfattribs = {'color': layer_color})

        # parse file
        polylines = dxf_parse(file)
        if len(polylines):
            for polyline in polylines:
                msp.add_lwpolyline(polyline, dxfattribs={'layer': layer_name})

        # next layer color
        layer_color = next(layer_colors, False)
        if layer_color == False:
            layer_colors = iter(colors)
            layer_color  = next(layer_colors)

        # increment layer num
        layer_num += 1

    #return the dwg object
    return dwg

# DXF files
input_files   = []
ignored_files = []

# make the files tree
for item in input:
    result = dxf_search(os.path.realpath(item))
    input_files.extend(result[0])
    ignored_files.extend(result[1])

# do the serious job
dxf_merge(input_files, colors, nolayers).saveas(output)

# success message
print 'input   :', '\n\t  '.join(input_files)
if len(ignored_files):
    print '\nignored :', '\n\t  '.join(ignored_files)
print '\noutput  :', output
