/*
      Parallel and Distributed Systems
      \file   main.c
      \brief  Main function for the RCM Algorithm

      \author Dimitra Karatza
      \AEM    8828
      \date   2020-09-19
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "../inc/rcm.h"

// global variables for time execution
struct timeval startwtime, endwtime;
double p_time;

Queue * createQueue(int maxElements);
int *rcm(int *M, int n, Queue *Q, Queue *R);
int find_bandwidth(int *X, int n);
int find_bandwidth_new(int *X, int n,int *M);

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
  int bandwith_before = find_bandwidth(M,n);

  // Instantiate an empty queue Q and empty array for permutation order of the objects R.
	Queue *Q = createQueue(n*n);
	Queue *R = createQueue(n);

  gettimeofday (&startwtime, NULL);

  int *Result = rcm(M, n, Q, R);

  gettimeofday (&endwtime, NULL);
  p_time = (double)((endwtime.tv_usec - startwtime.tv_usec)/1.0e6 + endwtime.tv_sec - startwtime.tv_sec);
  printf("RCM algorithm returned in %fsec\n",p_time);

  // Calculate bandwith after the permutation
  int bandwith_after = find_bandwidth_new(Result,n,M);

  printf("Bandwidth before the RCM algorith is %d, while after the permutations is %d.\n",bandwith_before,bandwith_after);

  free(R);
  free(M);

  return 0;
}
