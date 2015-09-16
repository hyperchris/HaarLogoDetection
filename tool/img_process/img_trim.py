#!/usr/bin/python

import os
from PIL import Image
import sys 
from random import randint
import time

# How to use: $ python trim_img.py [srcDir] [dstDir]

WIDTH = 200
HEIGHT = 150
PIECE = 4 # how many images each street view generates 

def trim_img(img_path, width, height, piece, dstDir):
    print 'Processing ' + img_path
    try:
        f = Image.open(img_path)
    except:
        print 'ERROR: cannot open: ' + img_path
        return
    else:
        xsize, ysize = f.size
        for i in range(piece):
            xstart = randint(0, xsize - width)
            ystart = randint(0, ysize - height)
            box = (xstart, ystart, xstart + width, ystart + height)
            f.crop(box).save(dstDir + '/' + `int(round(time.time() * 10000))` + '.jpg')

# Set the directory you want to start from
def dir_walk(rootDir, indent, dstDir):
    for dirName, subdirList, fileList in os.walk(rootDir):
        print indent + dirName
        for fname in fileList: # file 
            print indent + '\t'+ fname
            if '.jpg' in fname or '.JPG' in fname:
                trim_img(rootDir + fname, WIDTH, HEIGHT, PIECE, dstDir)
        for subDirName in subdirList:
            dir_walk(rootDir + subDirName, '\t' + indent, dstDir)

if __name__ == '__main__':
    # should go through all imgs here...
    if len(sys.argv) != 3:
        sys.exit('ERROR: wrong argc!')
    if not os.path.exists(sys.argv[1]):
        sys.exit('ERROR: ' + sys.argv[1] + ' was not found!')

    rootDir = sys.argv[1]
    dstDir = sys.argv[2]
    # create dest dir
    if not os.path.exists(dstDir):
        os.makedirs(dstDir)

    dir_walk(sys.argv[1], '', sys.argv[2]) # walk through 
    
    print "done"