from lxml import etree

class Art(object):
    def __init__(self, name, colour = None):
        self.name = name
        self.colour = colour if colour else name
        self.load()
        self.recolour()

    @property
    def path(self):
        return 'outputs/art_{}.svg'.format(self.name)

    def load(self):
        with open(self.path, 'rb') as f:
            self.xml = etree.parse(f)
        self.root = self.xml.getroot()

        self.polygon = self.find_el('path')
        if self.polygon is None:
            raise AttributeError('Loaded svg has no path element')

    def find_el(self, el):
        return self.root.find('{{*}}{}'.format(el))

    def recolour(self):
        attribs = self.polygon.attrib
        attribs['stroke'] = 'none'
        attribs['style'] = 'stroke:none'
        attribs['fill'] = self.colour
        attribs['stroke-width'] = '0'

    def set_dims(self, value):
        self.root.attrib['width']  = str(value[0]) + 'mm'
        self.root.attrib['height'] = str(value[1]) + 'mm'

    @property
    def viewbox(self):
        return [int(x) for x in self.root.attrib['viewBox'].split(' ')]

    @viewbox.setter
    def viewbox(self, value):
        self.root.attrib['viewBox'] = ' '.join(str(x) for x in value)

def add_layer(master, layer_id, layer_name, contents):
    group = etree.Element('g')
    group.attrib['{http://www.inkscape.org/namespaces/inkscape}groupmode'] = 'layer'
    group.attrib['id'] = layer_id
    group.attrib['{http://www.inkscape.org/namespaces/inkscape}label'] = layer_name
    for c in contents:
        group.append(c.polygon)
    master.root.append(group)

layers = [
    Art('black'),
    Art('white'),
    Art('grey'),
    Art('orange'),
    Art('blue', 'rgb(38,128,192)'),
    Art('pink', 'rgb(220,64,117)'),
    Art('cuts', 'rgb(255,0,255)'),
    Art('dimensions', 'rgb(0,0,0)')
]

master = layers[0]

title = master.find_el('title')
if title is not None:
    title.text = 'Pocket Voltex Artwork'

# cuts get stroke instead of fill
cuts = layers[-2]
cut_attribs = cuts.polygon.attrib
cut_attribs['stroke'] = cuts.colour
cut_attribs['stroke-width'] = '0.1'
cut_attribs['fill'] = 'none'
cut_attribs.pop('style')

# delete layer 0's path
master.polygon.getparent().remove(master.polygon)

# layers with actual names
add_layer(master, "layer1", "artwork", layers[:-2])
add_layer(master, "layer2", "cuts", [cuts])
add_layer(master, "layer3", "dimensions", [layers[-1]])

# fix viewbox
bottom_left_corner = master.viewbox[:2]
upper_right_corner = [left+size for left, size in zip(bottom_left_corner, master.viewbox[2:])]
for l in layers[1:]:
    layer_bottom_left = l.viewbox[:2]
    layer_upper_right = [left+size for left, size in zip(layer_bottom_left, l.viewbox[2:])]
    for i in range(2):
        # left start x, y
        bottom_left_corner[i] = min(bottom_left_corner[i], layer_bottom_left[i])
        # width and height
        upper_right_corner[i] = max(upper_right_corner[i], layer_upper_right[i])
box = bottom_left_corner + [topright-bottomleft for topright, bottomleft in zip(upper_right_corner, bottom_left_corner)]
master.viewbox = box
master.set_dims(box[2:])

out = etree.tostring(master.xml, pretty_print=True)
with open('outputs/artwork.svg', 'wb') as f:
    f.write(out)
