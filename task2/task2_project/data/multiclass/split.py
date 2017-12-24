#!/usr/bin/python
import os
import sys

n=1
t=3

file = open("test_labels.txt", 'r')
test = open("test.txt",'w')
train = open("train.txt", 'w')

for line in file:
	if n <t:
		train.write(line)
		n+=1
	else:
		test.write(line)
		n=1

#close(file)
#close(test)
#close(train)
