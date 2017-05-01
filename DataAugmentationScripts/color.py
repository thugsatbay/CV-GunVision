import os
import cv2
import numpy as np
from random import randint

folder = "./final_white/"
location = "./pistols/"

files = os.listdir(folder)

for ff in files:
    if ".jpg" in ff:
        img = cv2.imread(folder + ff)
        row, col, chn = img.shape
        img = cv2.resize(img, (96,96))
        row, col, chn = img.shape
        pixel_count = 0
        color_pixel = 0
        for mv in xrange(randint(5,12)):
            rrow,rcol = randint(0,row), randint(0,col)
            mrrow, mrcol = randint(rrow, row), randint(rcol, col)
            colors = (randint(0,255),randint(0,255),randint(0,255))
            for r in xrange(rrow,mrrow):
                for c in xrange(rcol, mrcol):
                    if sum(img[r,c]) >= 750:
                        img[r,c] = colors
        for r in xrange(row):
            for c in xrange(col):
                if sum(img[r,c]) >= 750:
                    if pixel_count == 0:
                        pixel_count = randint(7,21) * randint(96 / randint(1,4), 72 * randint(2,3))
                        color_pixel = (randint(0,255),randint(0,255),randint(0,255))
                    todo = randint(0,9)
                    if todo <= 8:
                        img[r,c] = color_pixel
                    else:
                        img[c,r] = color_pixel
                    pixel_count -= 1
        cv2.imwrite(location + "C_" + ff, img)
        print ff,"processed .." 