/*
      Parallel and Distributed Systems
      \file   main.c
      \brief  Main function for the RCM Algorithm

      \author Dimitra Karatza
      \AEM    8828
      \date   2020-09-30
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "../inc/rcm.h"

#define RESET  "\033[0m"
#define RED  "\033[0;31m"
#define GREEN  "\033[0;32m"
#define BGREEN  "\033[1;32m"
#define YELLOW  "\033[0;33m"
#define BCYAN  "\033[1;36m"

// global variables for time execution
struct timeval startwtime, endwtime;
double p_time;

int main(){

  int n=20000; //size
  double d=0.1; //density

  int *M = malloc(n*n * sizeof(int)); //matrix

  srand(time(0));
  double binary;

  // Create random matrix
  for(int i=0; i<n; i++){
    for(int j=i; j<n; j++){
      if(i == j)
        M[n*i + j] = 1;
      else{
        binary = (double)rand() / RAND_MAX;
        if(binary <= 0.01*d)
          M[n*i + j] = 1;
        else
          M[n*i + j] = 0;
      }
    }

    for(int j=0; j<i; j++)
      M[n*i + j] = M[n*j + i];
  }

  // Calculate bandwith before the permutation
  int bandwidth_before = find_bandwidth(M,n);

  // Instantiate an empty queue Q and empty array for permutation order of the objects R.
	Queue *Q = createQueue(n*n);
	Queue *R = createQueue(n);

  gettimeofday (&startwtime, NULL);

  int *Result = rcm(M, n, Q, R);

  gettimeofday (&endwtime, NULL);
  p_time = (double)((endwtime.tv_usec - startwtime.tv_usec)/1.0e6 + endwtime.tv_sec - startwtime.tv_sec);

  // Calculate bandwith after the permutation
  int bandwidth_after = find_bandwidth_new(Result,n,M);

  // Print a short description and results
  printf(YELLOW"\nThis code created a random sparse nxn matrix.\n");
  printf("RCM Algorithm was used to reduce bandwidth.\n");
  printf("Results are shown below.\n");

  printf(BGREEN"\n----------SUMMARY----------\n\n"RESET);
  printf("Dimension n      : %d\n",n);
  printf("Density          : %.2lf%c\n",d,'%' );
  printf("Bandwidth before : "RED"%d\n"RESET,bandwidth_before);
  printf("Bandwidth after  : "GREEN"%d\n"RESET,bandwidth_after);
  printf(BCYAN"\nRCM returned in  : %8f sec\n\n"RESET,p_time);

  free(R);
  free(M);

  return 0;
}
