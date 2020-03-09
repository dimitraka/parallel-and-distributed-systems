/*!
  \file   quickselect.c
  \brief  Implementation of QuickSelect

  \author Dimitra Karatza
	\AEM    8828
  \date   2019-11-4
*/
#include <stdio.h>
#include <stdlib.h>
#include "quickselect.h"

// Standard partition process of QuickSort().
// It considers the last element as pivot
// and moves all smaller element to left of
// it and greater elements to right
int partition(double arr[], int l, int r)
{
	double x = arr[r], temp;
	int i = l;
	for (int j = l; j <= r - 1; j++) {
		if (arr[j] <= x) {
			temp=arr[i];
			arr[i]= arr[j];
			arr[j]= temp;
			i++;
		}
	}
	temp=arr[i];
	arr[i]= arr[r];
	arr[r]= temp;
	return i;
}

// This function returns k'th smallest
// element in arr[l..r] using QuickSort
// based method. ASSUMPTION: ALL ELEMENTS
// IN ARR[] ARE DISTINCT
double kthSmallest(double arr[], int l, int r, int k)
{
	// If k is smaller than number of
	// elements in array
	if (k > 0 && k <= r - l + 1) {

		// Partition the array around last
		// element and get position of pivot
		// element in sorted array
		int index = partition(arr, l, r);

		// If position is same as k
		if (index - l == k - 1)
			return arr[index];

		// If position is more, recur
		// for left subarray
		if (index - l > k - 1)
			return kthSmallest(arr, l, index - 1, k);

		// Else recur for right subarray
		return kthSmallest(arr, index + 1, r, k - index + l - 1);
	}

	// If k is more than number of
	// elements in array
	//return INT_MAX;
}

// Driver program to test above methods
double quickSelect(double *arr,int n){

	int k;
	double r;
	if((n)%2!=0){
		//n odd
		k = (n+1)/2;
		return kthSmallest(arr, 0, n - 1, k);
	}else{
		//n even
		r = kthSmallest(arr,0,n-1,n/2+1)+kthSmallest(arr,0,n-1,n/2);
		return r/2.0;
	}
	//printf("k is:%d\n",k);
	//printf("K-th smallest element is: %lf\n",kthSmallest(arr, 0, n - 1, k));
}
