import cv2
import os
import numpy as np
import json
import math
from random import randint

def skew_image(mat):
    pts1 = np.float32([[randint(6,21),randint(6,21)],[randint(89,105),randint(6,21)],[randint(6,21),randint(89,105)],[randint(89,105),randint(89,105)]])
    pts2 = np.float32([[0,0],[111,0],[0,111],[111,111]])
    M = cv2.getPerspectiveTransform(pts1,pts2)
    dst = cv2.warpPerspective(mat,M,(112,112))
    return dst


def rotate_image(mat, angle):
    height, width = mat.shape[:2]
    image_center = (width / 2, height / 2)
    rotation_mat = cv2.getRotationMatrix2D(image_center, angle, 1)
    radians = math.radians(angle)
    sin = math.sin(radians)
    cos = math.cos(radians)
    bound_w = int((height * abs(sin)) + (width * abs(cos)))
    bound_h = int((height * abs(cos)) + (width * abs(sin)))
    rotation_mat[0, 2] += ((bound_w / 2) - image_center[0])
    rotation_mat[1, 2] += ((bound_h / 2) - image_center[1])
    rotated_mat = cv2.warpAffine(mat, rotation_mat, (bound_w, bound_h), borderMode = cv2.BORDER_REPLICATE)
    rotated_mat = cv2.resize(rotated_mat, (112,112))
    return skew_image(rotated_mat)


location = "./master_112_test/"
store = "./skew_112_test/"
files = os.listdir(location)
file_name = 1
f_start = -1
for ff in files:
    if ".jpg" in ff:
        img_c = cv2.imread(location + ff)
        row,col,chn = img_c.shape
        img_c_f = cv2.flip(img_c,1)
        rows, cols, chan = img_c.shape
        ang = 0
        while ang < 360:
            r_img = rotate_image(img_c, ang)
            r_img_f = rotate_image(img_c_f, ang)
            cv2.imwrite(store + "CI_" + str(file_name) + "_" + str(ang) + ".jpg", r_img)
            cv2.imwrite(store + "RCI_" + str(file_name) + "_" + str(ang) + ".jpg", r_img_f)
            print "Image",ff,ang,"processed ..."
            file_name += 1
            ang += randint(15,30)
        cv2.imwrite(store + "CI_O_" + str(file_name) + ".jpg", img_c)
        file_name += 1
        cv2.imwrite(store + "RCI_O_" + str(file_name) + ".jpg", img_c_f)
        file_name += 1
        print ""