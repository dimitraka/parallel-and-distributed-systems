/*
      Parallel and Distributed Systems
      \file   openmp.c
      \brief  OpenMP Implementation for the RCM Algorithm

      \author Dimitra Karatza
      \AEM    8828
      \date   2020-09-30
*/
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#include "../inc/rcm.h"

int find_min_not_inserted(int size, int *array, int *is_inserted){
	int minimum = size;
	int location = -1;

  for (int i = 0; i < size; i++){
    if( (array[i] < minimum) && (is_inserted[i] == 0)){
		  minimum = array[i];
		  location = i;
    }
  }

	return location;
}

void fill_Q(int n, int *M, Queue *Q, int *degrees, int element){

	// Find all the neighbors of a node
	int *neighbors = malloc(degrees[element] * sizeof(int));
	int pos = 0;

	// Parallelize only if n > 2000
	if(n>2000){
		#pragma omp parallel num_threads(8)
		{
			int *temp_neighbors = malloc(degrees[element] * sizeof(int));
			int temp_pos = 0;
			#pragma omp for
			for(int i=0;i<n;i++){
				if(M[n*element+i]==1 && element!=i){
					temp_neighbors[temp_pos] = i;
					temp_pos++;
				}
			}

			#pragma omp critical
			{
				memcpy(neighbors+pos, temp_neighbors, temp_pos * sizeof(int));
				pos+=temp_pos;
			}
			free(temp_neighbors);
		}
	}else{
		for(int i=0;i<n;i++){
			if(M[n*element+i]==1 && element!=i){
				neighbors[pos] = i;
				pos++;
			}
		}
	}

	// Sort the neighbors
	quickSort(neighbors, degrees, 0, degrees[element] - 1);

	for(int i=0; i<degrees[element]; i++){
		Enqueue(Q, neighbors[i]);
	}

	free(neighbors);
}

int *rcm(int *M, int n, Queue *Q, Queue *R){

	int *degrees = malloc(n * sizeof(int)); //array with the degrees of each element
	int *is_inserted = malloc(n * sizeof(int)); //binary array indicating if an element is inserted in R

	// Parallelize only if n > 2000
	if(n>2000){
		#pragma omp parallel num_threads(16)
		{
			// Find the degrees of all elements of M matrix
			#pragma omp for schedule(dynamic,1)
			for(int i=0;i<n;i++){
				int d = 0;
				for(int j=0;j<n;j++){
					// Degree is the sum of all elements minus 1 (for the diagonal elements)
					// of each row because we have matrix with binary elements
					d = d + M[n*i+j];
				}
				degrees[i] = d - 1;
			}

			// If node i is already inserted to R then is_inserted[i]=0 else is_inserted[i]=1
			// At first, no elements are inserted to R
			#pragma omp for schedule(dynamic,1)
			for(int i=0; i<n; i++)
				is_inserted[i] = 0;
		}
	}else{
		// Find the degrees of all elements of M matrix
	  for(int i=0;i<n;i++){
	    int d = 0;
	    for(int j=0;j<n;j++){
	      // Degree is the sum of all elements minus 1 (for the diagonal elements)
	      // of each row because we have matrix with binary elements
	      d = d + M[n*i+j];
	    }
	    degrees[i] = d - 1;
	  }

	  // If node i is already inserted to R then is_inserted[i]=0 else is_inserted[i]=1
	  // At first, no elements are inserted to R
	  for(int i=0; i<n; i++)
			is_inserted[i] = 0;
	}

	// Repeat until R is full
	do{

		// Select the node with the lowest degree that hasn't previously been inserted in the result array
		int parent = find_min_not_inserted(n, degrees, is_inserted);

		// Add P in the first free position of R
		Enqueue(R, parent);
		is_inserted[parent]=1;

		// Add to the queue all the nodes adjacent with parent in the increasing order of their degree.
		fill_Q(n, M, Q, degrees, parent);

		// Repeat until Q is empty
		while((Q->size)!=0){

			// Extract the first node from the queue and examine it.
			int child = front(Q);
			Dequeue(Q);

			//If child hasn't previously been inserted in R, add it in the first free position and add to
			//Q all the neighbours of child that are not in R in the increasing order of their degree.
			if(is_inserted[child] == 0){
				Enqueue(R, child);
				fill_Q(n, M, Q, degrees, child);
				is_inserted[child]=1;
			}

		}

	}while((R->size) < (R->capacity));


	// Reverse the order of the elements in R
	for(int i=0; i<(R->size)/2; i++){
		swap(&(R->elements[i]), &(R->elements[n-1-i]));
	}

	free(Q);
	free(degrees);
	free(is_inserted);

	return R->elements;

}
