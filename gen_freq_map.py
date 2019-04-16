#!/usr/bin/env python
 
for i in range(0, 512):
	freq = i * 43

	seperator = "HZ\t"
	if i <= 232: seperator = "HZ\t\t"

	print("#define FREQ_"+ str(freq) + seperator + str(i));
