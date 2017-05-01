import os
import numpy as np
from random import randint

p_folders = os.listdir("./profile_images")
n_folders = os.listdir("./coco_images")

train = open("./p_train.txt", "w")
validate = open("./p_validate.txt", "w")
test = open("./p_test.txt", "w")

p_table = []
for files in p_folders:
	if ".jpg" in files:
		p_table.append("gun_images/profile_images/" + files)

print "profile", len(p_table)

n_table = []
for files in n_folders:
	if ".jpg" in files:
		n_table.append("gun_images/coco_images/" + files)

print "negative",len(n_table)


p_table = np.random.permutation(p_table)
n_table = np.random.permutation(n_table)
random_array = np.random.permutation(np.append(p_table, n_table))
count = 1
class_v = 1
obj = test
for ele in random_array:
	#print str(ele)
	if "COCO" in (str(ele)).split("/")[-1]:
		class_v = 1
	else:
		class_v = 0
	if count > 1000 and count <= 3000:
		obj = validate
	elif count > 4000:
		obj = train
	obj.write(ele + ' ' + str(class_v))
	obj.write("\n")
	count += 1

print "Completed"
