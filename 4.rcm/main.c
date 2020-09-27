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
#include "rcm.h"

// global variables for time execution
struct timeval startwtime, endwtime;
double p_time;

Queue * createQueue(int maxElements);
int *rcm(int *M, int n, Queue *Q, Queue *R);
int find_bandwidth(int *X, int n);

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

  /*for(int i=0;i<n;i++){
    for(int j=0;j<n;j++){
      printf("%d",M[n*i+j]);
    }
    printf("\n");
  }
  printf("\n");*/

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

  /*for(int i=0;i<n;i++){
    printf("%d",Result[i]);
  }
  printf("\n\n");*/

  // Create the final array by applying the permutations as they are shown in Result
  int *F = malloc(n*n * sizeof(int)); //final matrix
  int permutation = -1;
  for(int i=0;i<n;i++){
    permutation = Result[i];
    for(int j=0;j<n;j++){
      F[n*i+j] = M[n*permutation+j];
    }
  }

  /*for(int i=0;i<n;i++){
    for(int j=0;j<n;j++){
      printf("%d",F[n*i+j]);
    }
    printf("\n");
  }
  printf("\n");*/

  // Calculate bandwith after the permutation
  int bandwith_after = find_bandwidth(F,n);

  printf("Bandwith before the RCM algorith is %d, while after the permutations is %d.\n",bandwith_before,bandwith_after);

  free(R);
  free(M);

  return 0;
}

int find_bandwidth(int *X, int n)
{
	int band_hi = 0;
	int band_lo = 0;
	int temp;

	for (int i = 0; i < n; i++)
	{
		temp = 0;
		for (int j = n - 1; j > i; j--)
			if (X[n * i + j] != 0)
			{
				temp = j - i;
				break;
			}
			else
				continue;

		if (temp > band_hi)
			band_hi = temp;

		temp = 0;
		for (int j = 0; j < i; j++)
			if (X[n * i + j] != 0)
			{
				temp = i - j;
				break;
			}
			else
				continue;

		if (temp > band_lo)
			band_lo = temp;
	}

	int bandwidth = band_lo + band_hi + 1;

	return bandwidth;
}
