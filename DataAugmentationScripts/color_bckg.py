import os
import cv2
import numpy as np
from random import randint

folder = "./final_white/"
location = "./pistols/"
bckg = "./coco"

files = os.listdir(folder)
b_files = os.listdir(bckg)
b_len = len(b_files)
for ff in files:
    if ".jpg" in ff:
        b_files = np.random.permutation(b_files)
        img = cv2.imread(folder + ff)
        img = cv2.resize(img, (112,112))
        row, col, chn = img.shape
        img_rand = randint(0, b_len - 1)
        bk_img = cv2.imread(bckg + b_files[img_rand])
        bk_img = cv2.resize(bk_img, (112,112))
        for r in xrange(row):
            for c in xrange(col):
                if sum(img[r,c]) >= 750:
                    img[r,c] = bk_img[r,c]
        cv2.imwrite(location + "C_" + ff, img)
        print ff,"processed .." 