/*!
  \file   knnring_sequential.c
  \brief  Sequential implementation.

  \author Dimitra Karatza
  \AEM    8828
  \date   2019-12-10
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cblas.h>
#include "../inc/knnring.h"
#include "quickselect.h"


knnresult kNN(double * X, double * Y, int n, int m, int d, int k){

  knnresult result;
  double *D=malloc(m*n*sizeof(D));
  double *C=malloc(m*n*sizeof(double)); //result of cblas
  int *indices=malloc(m*n*sizeof(int)); //array of indexes
  double temp;
  double S; //sum of elements

  //Calculate the sum of X^2
  //x1^2+x2^2+...+xn^2
  for(int i=0;i<n;i++){
    S=0;
    for(int j=0;j<d;j++){
      temp=pow(X[i+j*n],2);
      S+=temp;
    }
    //fill all the elements of array D which need this sum
    for(int j=0;j<m;j++){
      D[i+j*n]=S;
    }
  }


  //Calculate the sum of Y^2
  //y1^2+y2^2+...+yn^2
  for(int i=0;i<m;i++){
    S=0;
    for(int j=0;j<d;j++){
      temp=pow(Y[i+j*m],2);
      S+=temp;
    }
    for(int j=0;j<n;j++){
      D[i*n+j]+=S;
    }
  }

  //Multiplication of X and trans Y
  //-2*X*Y.'
  cblas_dgemm(CblasColMajor, CblasNoTrans, CblasTrans, n, m, d, 1, X, n, Y, m, 0, C, n);
  for(int i=0;i<n;i++){
    for(int j=0;j<m;j++){
      D[i+j*n]-=2*C[i+j*n];
      //in case distance==0
      if(D[i+j*n]!=D[i+j*n]){
        D[i+j*n]=0.0;
      }
      D[i+j*n]=sqrt(D[i+j*n]);
    }
  }

  //array of indexes
  for(int i=0;i<n*m;i++){
    indices[i]=i%n;
  }

  //find the k nearest elements
  for(int i=0;i<m;i++){
    for(int j=k;j>0;j--){
      if(j==k){
        quickSelect(D,i*n,i*n+n,j,indices);
      }else{
        quickSelect(D,i*n,i*n+j,j,indices);
      }
    }
  }

  //return values
  result.m=m;
  result.k=k;
  result.ndist=malloc(m*k*sizeof(double));
  result.nidx=malloc(m*k*sizeof(int));

  //save the k smallest distances
  for(int i=0;i<k;i++){ //for every neighbor
    for(int j=0;j<m;j++){ //for every query point
      result.ndist[j+i*m]=D[i+j*n];
      result.nidx[j+i*m]=indices[j*n+i];
    }
  }

  return result;
}
