#!/usr/bin/env python3
import os
from pathlib import Path
import re
basedir = "../sip/src"


def process_file(file):
    ftext = Path(file).read_text()
    classes = re.findall('^(?:class |struct )(\w+)', ftext, re.M)
    r = {}
    for c in classes:
        r[c] = {
            'type': 'class',
        }
    return r


module_tree = {
    '': {
        'name': '',
        'text': Path(basedir + "/header.rst").read_text(),
        'children': {},
        'type': 'directory',
        'level': 0
    }
}

for dirpath, dnames, fnames in os.walk(basedir):
    dirs = dirpath[len(basedir):].split('/')
    cdir = module_tree
    level = 0
    lname = ''
    for d in dirs:
        cdir = cdir[d]['children']
        level += 1
        lname += d + '.'

    for d in dnames:
        file = dirpath + '/' + d + '/header.rst'
        cdir[d] = {
            'text': Path(file).read_text() if os.path.isfile(file) else None,
            'name': lname[1:] + d,
            'children': {},
            'type': 'directory',
            'level': level
        }
    for f in fnames:
        if f.endswith('.sip'):
            cdir[f[:-4]] = {
                'text': '',
                'children': process_file(dirpath + '/' + f),
                'type': 'file',
            }

header_levels = ['=', '-', '~', '_']

output = []


def format_dir(d):
    if(d['text'] is not None):
        output.append(d['text'])
    else:
        output.append('')
        output.append(d['name'])
        output.append(header_levels[d['level']] * len(d['name']))
    for elem in d['children']:
        if(d['children'][elem]['type'] == 'directory'):
            format_dir(d['children'][elem])
        if(d['children'][elem]['type'] == 'file'):
            format_file(d['children'][elem])


def format_file(f):
    for c in f['children']:
        format_class(c)


def format_class(c):
    output.append("""
.. autoclass:: alpenglow.cpp.""" + c + """
    :members:
    :undoc-members:
    :show-inheritance:
""")


format_dir(module_tree[''])
with open("source/alpenglow.cpp.rst", "w") as f:
    f.write("\n".join(output))