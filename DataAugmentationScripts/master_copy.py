import cv2
import os
import numpy as np
import json
import math


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
    return rotated_mat


location = "./unified/"
store = "./master_112/"
files = os.listdir(location)
jf = open("./unified.txt", "r")
jobj = json.load(jf)
jobj = jobj['bounds']
file_name = 1
f_start = -1
for ff in xrange(len(jobj)):
    if ff > f_start:
        img = cv2.imread("./" + jobj[ff]['filename'])
        print jobj[ff]['filename']
        row,col,chn = img.shape
        if chn > 1:
            for ann in xrange(len(jobj[ff]['annotations'])):
                crop = jobj[ff]['annotations'][ann]
                y_axis, x_axis = min(max(crop['y'], 0), row), min(max(crop['x'], 0), col)
                img_c = img[y_axis : min(y_axis + crop['height'], row), x_axis : min(x_axis + crop['width'], col)]
                img_c = cv2.resize(img_c, (112,112))
                cv2.imwrite(store + "I_" + str(file_name) + ".jpg", img_c)
                file_name += 1
                '''
                img_c_f = cv2.flip(img_c,1)
                rows, cols, chan = img_c.shape
                for ang in range(0, 360, 15):
                    r_img = rotate_image(img_c, ang)
                    r_img_f = rotate_image(img_c_f, ang)
                    cv2.imwrite(store + "CI_" + str(file_name) + "_" + str(ang) + ".jpg", r_img)
                    cv2.imwrite(store + "RCI_" + str(file_name) + "_" + str(ang) + ".jpg", r_img_f)
                    print "Image",ff,ang,"processed ..."
                    file_name += 1
                '''