#ifndef QUICKSELECT_H
#define QUICKSELECT_H

int partition(double arr[], int l, int r, int *indices);

double kthSmallest(double arr[], int l, int r, int k, int *indices);

void quickSelect(double *arr, int l, int r, int k, int *indices);

#endif
