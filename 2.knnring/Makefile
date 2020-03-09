# define the shell to bash
SHELL := /bin/bash

# define the C/C++ compiler to use,default here is clang
CC = gcc-7
MPICC = mpicc

lib: library

library:
	$(CC) -c src/knnring_sequential.c -I inc/
	$(CC) -c src/quickselect.c
	$(MPICC) -lopenblas -c src/knnring_synchronous.c -I inc/
	$(MPICC) -lopenblas -c src/knnring_asynchronous.c -I inc/

	ar rcs lib/knnring_sequential.a knnring_sequential.o quickselect.o
	ar rcs lib/knnring_synchronous.a knnring_synchronous.o quickselect.o
	ar rcs lib/knnring_asynchronous.a knnring_asynchronous.o quickselect.o
