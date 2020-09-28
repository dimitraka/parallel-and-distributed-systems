# define the shell to bash
SHELL := /bin/bash

# define the C/C++ compiler to use
CC = gcc
EXECS =  sequential openmp

# define command to remove files
RM = rm -rf

.PHONY: $(EXECS)
all: $(EXECS)

sequential:
	cd src; $(CC) main.c queue.c quicksort.c sequential.c bandwidth.c -o sequential ; \
	./sequential
	$(RM) src/sequential

openmp:
	cd src; $(CC) -O3 main.c queue.c quicksort.c openmp.c bandwidth.c -o openmp -fopenmp ; \
	./openmp
	$(RM) src/openmp
