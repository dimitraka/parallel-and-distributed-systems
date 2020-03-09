/*!
  \file   quickselect.h
  \brief  Header file for quickselect.c file

  \author Dimitra Karatza
  \AEM    8828
  \date   2019-11-4
*/
#ifndef QUICKSELECT_H
#define QUICKSELECT_H

int partition(double arr[], int l, int r);

double kthSmallest(double arr[], int l, int r, int k);

double quickSelect(double *arr, int n);

#endif
