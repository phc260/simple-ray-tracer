"""
simple PPM to JPG converter
usage: on command line type "python ppm2jpg.py filename"
"""
from PIL import Image
import os, sys

if len(sys.argv) <2:
	print "input file name?\n";
	sys.exit(0)

infile = sys.argv[1]
name, extension = os.path.splitext(infile)

f = open(infile)
color = f.readline().splitlines()
x, y = f.readline().split()
maxv = f.readline().splitlines()

x = int(x)
y = int(y)


im = Image.new("RGB", (x, y), "black")
data = f.read().split()
k = 0
for j in xrange(y):
	for i in xrange(x):
		im.putpixel((i, j), (int(data[k]), int(data[k+1]), int(data[k+2])))
		k += 3
im.save(name + ".jpg", quality=100)

print chr(7)

