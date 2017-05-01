

source = open("test.txt", 'r')
w_0 = open("test_0.txt", "w")
w_1 = open("test_1.txt", "w")

for x in source:
	if int(x.split(" ")[-1]) == 0:
		w_0.write(x)
	elif int(x.split(" ")[-1]) == 1:
		w_1.write(x)
