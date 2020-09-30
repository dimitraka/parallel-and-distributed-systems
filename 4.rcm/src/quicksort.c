/*
      Parallel and Distributed Systems
      \file   quicksort.c
      \brief  Implementation of quicksort algorithm

      \author Dimitra Karatza
      \AEM    8828
      \date   2020-09-30
*/
#include <stdio.h>

// A utility function to swap two elements
void swap(int* a, int* b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

/* This function takes last element as pivot, places
   the pivot element at its correct position in sorted
    array, and places all smaller (smaller than pivot)
   to left of pivot and all greater elements to right
   of pivot */
int partition (int arr1[], int arr2[], int low, int high)
{
    int pivot = arr2[ arr1[high] ];    // pivot
    int i = (low - 1);  // Index of smaller element


    for (int j = low; j <= high- 1; j++)
    {
        // If current element is smaller than the pivot
        if (arr2[ arr1[j] ] < pivot)
        {
            i++;    // increment index of smaller element
            swap(&arr1[i], &arr1[j]);
        }
    }
    swap(&arr1[i + 1], &arr1[high]);
    return (i + 1);
}

/* The main function that implements QuickSort
 arr[] --> Array to be sorted,
  low  --> Starting index,
  high  --> Ending index */
void quickSort(int arr1[], int arr2[], int low, int high)
{
    if (low < high)
    {
        /* pi is partitioning index, arr[p] is now
           at right place */
        int pi = partition(arr1, arr2, low, high);

        // Separately sort elements before
        // partition and after partition
        quickSort(arr1, arr2, low, pi - 1);
        quickSort(arr1, arr2, pi + 1, high);
    }
}
