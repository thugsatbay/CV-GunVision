import cv2
import os
import numpy as np
import math
from random import randint

def color(img):
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
	return img

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
    rotated_mat = cv2.warpAffine(mat, rotation_mat, (bound_w, bound_h), borderValue = (255,255,255))
    rotated_mat = cv2.resize(rotated_mat, (112,112))
    return rotated_mat


folder = "./white/"
files = os.listdir(folder)
location = "./final_white/"
file_name = 1
for ff in files:
	if ".jpg" in ff:
		img = cv2.imread(folder + ff)
		img_f = cv2.flip(img,1)
		print ff
		rows, cols, chan = img.shape
		#add_bd = max(cols, rows) / 4
		for ang in range(0, 360, 15):
			r_img = rotate_image(img, ang)
			r_img_f = rotate_image(img_f, ang)
			r_img = color(r_img)
			r_img_f = color(r_img_f)
			cv2.imwrite(location + "I_" + str(file_name) + "_" + str(ang) + ".jpg", r_img)
			cv2.imwrite(location + "RI_" + str(file_name) + "_" + str(ang) + ".jpg", r_img_f)
			print "Image",ff,ang,"processed ..."
			file_name += 1
