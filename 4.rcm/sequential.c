/*
      Parallel and Distributed Systems
      \file   sequential.c
      \brief  Serial Implementation for the RCM Algorithm

      \author Dimitra Karatza
      \AEM    8828
      \date   2020-09-19
*/
#include <stdio.h>
#include <stdlib.h>
#include "rcm.h"

void swap(int* a, int* b);
Queue * createQueue(int maxElements);
void Dequeue(Queue *Q);
int front(Queue *Q);
void Enqueue(Queue *Q,int element);
void quickSort(int arr1[], int arr2[], int low, int high);

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
	for(int i=0;i<n;i++){
		if(M[n*element+i]==1 && element!=i){
			neighbors[pos] = i;
			pos++;
		}
	}

	// Sort the neighbors
	quickSort(neighbors, degrees, 0, degrees[element] - 1);

	for(int i=0; i<degrees[element]; i++){
		Enqueue(Q, neighbors[i]);
	}

	free(neighbors);
}

int *rcm(int *M, int n){
  // Instantiate an empty queue Q and empty array for permutation order of the objects R.
	Queue *Q = createQueue(n*n);
	Queue *R = createQueue(n);

  // Find the degrees of all elements of M matrix
	int *degrees = malloc(n * sizeof(int));
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
  int *is_inserted = malloc(n * sizeof(int));
  for(int i=0; i<n; i++)
		is_inserted[i] = 0;

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
