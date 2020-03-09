# define the shell to bash
SHELL := /bin/bash

lib: library

library:	
	gcc -O3 quickselect.c vptree_sequential.c main.c -o vptree_sequential -lm
	gcc -O3 quickselect.c vptree_pthreads.c main.c -o vptree_pthreads -lm -pthread
	gcc -O3 quickselect.c vptree_openmp.c main.c -o vptree_openmp -lm -fopenmp
	gcc -O3 quickselect.c vptree_cilk.c main.c -o vptree_cilk -lm -fcilkplus




