#
# Example python script to generate a Seeed friendly BOM from a KiCad generic netlist
#

"""
    @package
    Generate a Tab delimited list (csv file type).
    Components are sorted by ref and grouped by value with same footprint
    Fields are (if exist)
    'Ref', 'Qnty', 'Value', 'Cmp name', 'Footprint', 'Description', 'Part number'

    Command line:
    python "pathToFile/bom_csv_grouped_by_value_with_fp.py" "%I" "%O.csv"
"""

# be able to find the kicad stuff
import sys
sys.path.append('/usr/share/kicad/plugins')
sys.path.append('C:/Program Files/KiCad/bin/scripting/plugins')

# Import the KiCad python helper module and the csv formatter
import kicad_netlist_reader
import csv

# Generate an instance of a generic netlist, and load the netlist tree from
# the command line option. If the file doesn't exist, execution will stop
net = kicad_netlist_reader.netlist(sys.argv[1])

# Open a file to write to, if the file cannot be opened output to stdout
# instead
try:
    f = open(sys.argv[2], 'w')
except IOError:
    e = "Can't open output file for writing: " + sys.argv[2]
    print(__file__, ":", e, sys.stderr)
    f = sys.stdout

# Create a new csv writer object to use as the output formatter
out = csv.writer(f, lineterminator='\n', delimiter=',', quotechar='\"', quoting=csv.QUOTE_ALL)

# Output a set of rows for a header providing general information
print('Component Count:{}'.format(len(net.components)))
out.writerow(['Designator', 'MPN', 'Qty', 'Link', 'Description'])

uniques = set()

# Get all of the components in groups of matching parts + values
# (see ky_generic_netlist_reader.py)
grouped = net.groupComponents()

# Output all of the component information
for group in grouped:
    refs = []

    # Add the reference of every component in the group and keep a reference
    # to the component so that the other data can be filled in once per group
    for component in group:
        refs.append(component.getRef())
        c = component

    part = c.getField("Part number")
    if not part or part == '-':
        print('Skipping {}'.format(c.getRef()))
        continue

    uniques.add(part)

    # Fill in the component groups common data
    out.writerow([','.join(refs), part, len(group), c.getField("Link"), c.getDescription()])

print('{} unique parts'.format(len(uniques)))
