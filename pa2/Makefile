#Makefile \
Jeff DiPaola\
CS 214 - Assignment 2

all:
	gcc -Wall -c sorted-list.c -o sorted-list.o
	ar r libsl.a sorted-list.o
	gcc -L. -lsl main.c -o sl sorted-list.o
clean:
	rm - f sl sorted-list.o libsl.a
