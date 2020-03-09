/*!
  \file   main.c
  \brief  Main function for Vantage Point implementation.

  \author Dimitra Karatza
  \AEM    8828
  \date   2019-11-4

  \To compile with gcc you should run in a terminal one of the
  \following commands according to the version you want:
  \
  \    >gcc quickselect.c vptree_sequential.c main.c -o name_of_executable -lm
  \    >gcc quickselect.c vptree_pthreads.c main.c -o name_of_executable -lm -pthread
  \    >gcc quickselect.c vptree_openmp.c main.c -o name_of_executable -lm -fopenmp
  \    >gcc quickselect.c vptree_cilk.c main.c -o name_of_executable -lm -fcilkplus
  \
  \Or you can use the Makefile of the folder "src".
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "../inc/vptree.h"
#include "quickselect.h"
#include <sys/time.h> //print time
#include <sys/times.h> //print time

#define n 100000 //number of points
#define d 10      //dimentions

//time duration
struct timeval startwtime, endwtime;
static clock_t st_time;
static clock_t en_time;
static struct tms st_cpu;
static struct tms en_cpu;
double seq_time;
double p_time;

double* setPoints(double *X){
  for(int i=0;i<n*d;i++){
    X[i]=(double)rand()/(double)RAND_MAX;
  }
  return X;
}

int main(int argc, const char** argv){

  double *X= (double*)malloc(d*n*sizeof(double)); //coordinates
  vptree *T;

  // Use current time as seed for random generator
  srand(time(0));

  //fill the X array with random input data points
  setPoints(X);

  //time duration
  gettimeofday (&startwtime, NULL);
  st_time = times(&st_cpu);

  //Build the vantage point tree
  T=buildvp(X,n,d);

  //time duration
  en_time = times(&en_cpu);
  gettimeofday (&endwtime, NULL);
  p_time = (double)((endwtime.tv_usec - startwtime.tv_usec)/1.0e6 + endwtime.tv_sec - startwtime.tv_sec);
  printf("Returned in %fsec\n",p_time);

  destroy(T);

  free(X);
}
