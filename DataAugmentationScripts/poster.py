import cv2
import os
import numpy as np
import json
import math
import random

folder = "./pistols/"

files = os.listdir(folder)
files = random.sample(files, 10*10)
b_img = np.zeros((1120,1120,3), np.uint8)
r,c = 0,0
size = 112
for ff in files:
    if ".jpg" in ff:
        img = cv2.imread(folder + ff)
        img = cv2.resize(img, (size,size))
        b_img[r:r+size, c:c+size] = img
        c += size
        if c == 1120:
            c = 0
            r += size
        if r == 1120:
            break
cv2.imwrite("white_pistol.jpg", b_img)