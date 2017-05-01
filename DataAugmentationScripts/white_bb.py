import cv2
import os
import numpy as np

unified = "./unified/"
location = "./unified"
white = "./white/"
if not os.path.exists(unified):
	os.makedirs(unified)
if not os.path.exists(white):
	os.makedirs(white)
gun_type = "DD_"
folder = os.listdir("./")
white_name = 1260
unified_name = 2100
not_allowed = ['white', 'unified', 'white_bb.py']
for clss in folder:
	if not clss in not_allowed and not "." in clss:
		folders = os.listdir("./" + clss + "/")
		for no in folders:
			location = "./" + clss + "/" + no + "/"
			print "location changed : ", location
			files = os.listdir(location)
			for f in files:
				if ".jpg" in f:
					print ("processing",f)
					img = cv2.imread(location + "/" + f)
					row, col, cha = img.shape
					count = 0
					for r in xrange(row):
						for c in xrange(col):
							if sum(img[r,c]) == 765:
								count += 1
					ff = open(location + "/" + f, 'rb').read()
					if count >= 300:
						ww = open(white + gun_type + str(white_name) + ".jpg", 'wb')
						ww.write(ff)
						white_name += 1
						print (f, "copied to white", count)
						#os.remove(location + "/" + f)
					else:
						ww = open(unified + gun_type + str(unified_name) + ".jpg", 'wb')
						ww.write(ff)
						unified_name += 1
						print (f, "copied to unified", count)
						#os.remove(location + "/" + f)

