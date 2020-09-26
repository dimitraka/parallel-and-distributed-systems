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

// global variables for time execution
struct timeval startwtime, endwtime;
double p_time;

int *rcm(int *M, int n);

int main(){

  int n; //size
  double d; //density

  printf("Please give the desirable size of matrix: ");
  scanf("%d", &n);
  printf("Now give a number between 1 and 100, which will state the percentage of density: ");
  scanf("%lf", &d);

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

  // Print created matrix
  /*for(int i=0; i<n; i++){
      for(int j=0; j<n; j++){
        printf("%d ", M[n*i + j]);
      }
      printf("\n");
  }
  printf("\n");*/
  gettimeofday (&startwtime, NULL);

  int *R = rcm(M, n);

  gettimeofday (&endwtime, NULL);
  p_time = (double)((endwtime.tv_usec - startwtime.tv_usec)/1.0e6 + endwtime.tv_sec - startwtime.tv_sec);
  printf("Returned in %fsec\n",p_time);

  //Print result
  /*for(int i=0; i<n; i++){
    printf("%d ", R[i]);
  }
  printf("\n");*/

  free(R);
  free(M);

  return 0;
}
