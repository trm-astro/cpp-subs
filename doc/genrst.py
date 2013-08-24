#!/usr/bin/env python

"""
Makes rst files from program files
"""

import os

names = ('weekday','gap','tcorr')

src  = '../src/'
dest = '_store'

fnames = [(os.path.join(src, name + '.cc'), \
               os.path.join(dest, name + '.rst')) \
               for name in names]

for source, rst in fnames:
    out = False
    with open(source) as fin, open(rst,'w') as fout:
        for line in fin:
            if line.startswith('!!sphinx'):
                out = not out
            elif out:
                fout.write(line)
