import os
import numpy as np
from random import randint

p_location = "/skew/skew_train/"
n_location = "/skew/result_skew/"
p_folders = os.listdir("." + p_location)
n_folders = os.listdir("." + n_location)
t_folders = os.listdir("./skew/skew_test")

train = open("./train.txt", "w")
validate = open("./validate.txt", "w")
test_0 = open("./test_0.txt", "w")
test_1 = open("./test_1.txt", "w")

p_table = []
for files in p_folders:
	if ".jpg" in files:
		p_table.append("gun_images" + p_location + files)

print "imfdb", len(p_table)

n_table = []
for files in n_folders:
	if ".jpg" in files:
		n_table.append("gun_images" + n_location + files)

print "negative",len(n_table)

p_table = np.random.permutation(p_table)
n_table = np.random.permutation(n_table)
nt_table = n_table[:15000]
n_table = n_table[15000:]
n_table = n_table[:min(135000, len(n_table))]
random_array = np.random.permutation(np.append(p_table, n_table))
count = 1
class_v = 1
obj = validate
count_tr = 0
count_v = 0
count_tt = 0

#for ele in nt_table:
#	test_1.write(ele + ' ' + str(1))
#	test_1.write("\n")

for ele in t_folders:
	test_0.write("gun_images/skew/skew_test/" + ele + ' ' + str(0))
	test_0.write("\n")

for ele in random_array:
	break
	#print str(ele)
	if (str(ele)).split("/")[-1][0] in ['G']:
		class_v = 1
	else:
		class_v = 0
	if count > 25000:
		obj = train
		if class_v == 0:
			count_tr += 1
	if count <= 25000:
		obj = validate
		if class_v == 0:
			count_v += 1
	obj.write(ele + ' ' + str(class_v))
	obj.write("\n")
	count += 1

print "Completed", count
print "Completed Test"
print "Positive in Train", count_tr
print "Positive in Validate", count_v
