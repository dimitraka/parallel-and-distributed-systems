/*!
  \file   quickselect.c
  \brief  Implementation of QuickSelect used in synchronous, asynchronous and sequential versions.

  \author Dimitra Karatza
  \AEM    8828
  \date   2019-12-10
*/

#include <stdio.h>
#include <stdlib.h>
#include "quickselect.h"

// Standard partition process of QuickSort().
// It considers the last element as pivot
// and moves all smaller element to left of
// it and greater elements to right
int partition(double arr[], int l, int r, int *indices)
{
	double x = arr[r], temp;
	int i = l;
	int tempidx;
	for (int j = l; j <= r - 1; j++) {
		if (arr[j] <= x) {
			//swap array
			temp=arr[i];
			arr[i]= arr[j];
			arr[j]= temp;
			//swap indices
			tempidx=indices[i];
			indices[i]= indices[j];
			indices[j]= tempidx;
			i++;
		}
	}
	//swap array
	temp=arr[i];
	arr[i]= arr[r];
	arr[r]= temp;
	//swap indices
	tempidx=indices[i];
	indices[i]= indices[r];
	indices[r]= tempidx;
	return i;
}

// This function returns k'th smallest
// element in arr[l..r] using QuickSort
// based method. ASSUMPTION: ALL ELEMENTS
// IN ARR[] ARE DISTINCT
double kthSmallest(double arr[], int l, int r, int k, int *indices)
{
	// If k is smaller than number of
	// elements in array
	if (k > 0 && k <= r - l + 1) {

		// Partition the array around last
		// element and get position of pivot
		// element in sorted array
		int index = partition(arr, l, r, indices);

		// If position is same as k
		if (index - l == k - 1)
			return arr[index];

		// If position is more, recur
		// for left subarray
		if (index - l > k - 1)
			return kthSmallest(arr, l, index - 1, k, indices);

		// Else recur for right subarray
		return kthSmallest(arr, index + 1, r, k - index + l - 1, indices);
	}

	// If k is more than number of
	// elements in array
	//return INT_MAX;
}

// Driver program to test above methods
void quickSelect(double *arr,int l, int r,int k, int *indices){

	kthSmallest(arr, l, r-1, k, indices);

	//printf("k is:%d\n",k);
	//printf("K-th smallest element is: %lf\n",kthSmallest(arr, 0, n - 1, k));
}
