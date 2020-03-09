# define the shell to bash
SHELL := /bin/bash

lib: library

library: 
	gcc -c src/vptree_sequential.c -I inc/
	gcc -c -pthread src/vptree_pthreads.c -I inc/
	gcc -c -fopenmp src/vptree_openmp.c -I inc/
	gcc -c -fcilkplus src/vptree_cilk.c -I inc/
	gcc -c src/quickselect.c
	ar rcs lib/vptree_sequential.a vptree_sequential.o quickselect.o
	ar rcs lib/vptree_pthreads.a vptree_pthreads.o quickselect.o
	ar rcs lib/vptree_openmp.a vptree_openmp.o quickselect.o
	ar rcs lib/vptree_cilk.a vptree_cilk.o quickselect.o





