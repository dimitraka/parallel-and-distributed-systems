/*!
  \file   vptree_sequential.c
  \brief  Sequential Version for Vantage Point implementation.

  \author Dimitra Karatza
  \AEM    8828
  \date   2019-11-4
*/
#include "../inc/vptree.h"
#include "quickselect.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double* getDistance(double *X, int n, int d){

  double S; //save the sum of the pow's
  double *dis=malloc(n*sizeof(dis)); //array of all distances from vp

  //calculate all the distances from the vantage point
  for(int i=0;i<n-1;i++){
    S=0;
    for(int j=0;j<d;j++){
      S=S+pow(X[i+j*n]-X[n-1+j*n],2);
    }
    dis[i]=sqrt(S);
  }

  return dis;
}

vptree * helper_buildvp(double *X, int *indexes, int n, int d){
  //create a pointer to a new structure of type vptree
  vptree *T=NULL;

  //check if recursion is to be ended
  if(n==0){
    return T;
  }

  //allocate memory for the new structure pointer
  T=(vptree *)calloc(1,sizeof(vptree));

  //select the last point as the vantage point
  T->vp = malloc(d*sizeof(T->vp));
  for(int i=0; i<d; i++){
    T->vp[i] = X[n+i*n-1];
  }

  //set the index of the vantage point in the original set
  T->idx = indexes[n-1];

  if(n==1){
    return T;
  }

  //find the median distance of the vantage point to the other points
  double *dis,*sortedDis;
  sortedDis= malloc(n*sizeof(sortedDis));

  dis=getDistance(X,n,d);

  for(int i=0;i<n-1;i++){
    sortedDis[i]=dis[i];
  }
  T->md =quickSelect(sortedDis,n-1);

  //find inner & outer subtree
  double *innerX = (double*)malloc(d*sizeof(double));
  double *outerX = (double*)malloc(d*sizeof(double));
  int *idxInner = malloc(sizeof(int));
  int *idxOuter = malloc(sizeof(int));
  int sizeInner=0, sizeOuter=0;
  for(int i=0;i<n-1;i++){
    if(dis[i]<=T->md){
      sizeInner++;
      innerX=realloc(innerX,sizeInner*d*sizeof(double));
      idxInner=realloc(idxInner,sizeInner*sizeof(int));
      //move elements to the right position
      for(int j=d;j>1;j--){
        for(int k=sizeInner*j-2; k>=sizeInner*(j-1);k--){
          innerX[k]=innerX[k-(j-1)];
        }
      }
      //fill array with new elements
      int k=0;
      for(int j=sizeInner-1; j<sizeInner*d;j+=sizeInner){
        innerX[j]=X[i+k*n];
        k++;
      }
      idxInner[sizeInner-1]=indexes[i];
    }else{
      sizeOuter++;
      outerX=realloc(outerX,sizeOuter*d*sizeof(double));
      idxOuter=realloc(idxOuter,sizeOuter*sizeof(int));
      //move elements to the right position
      for(int j=d;j>1;j--){
        for(int k=sizeOuter*j-2; k>=sizeOuter*(j-1);k--){
          outerX[k]=outerX[k-(j-1)];
        }
      }
      //fill array with new elements
      int k=0;
      for(int j=sizeOuter-1; j<sizeOuter*d;j+=sizeOuter){
        outerX[j]=X[i+k*n];
        k++;
      }
      idxOuter[sizeOuter-1]=indexes[i];
    }
  }

  T->inner=helper_buildvp(innerX,idxInner,sizeInner,d);
  T->outer=helper_buildvp(outerX,idxOuter,sizeOuter,d);

  free(idxInner);
  free(idxOuter);
  free(outerX);
  free(innerX);
  free(dis);
  free(sortedDis);
  return T;
}

vptree * buildvp(double *X, int n, int d){

  int *indexes = malloc(n*sizeof(int));
  vptree *T;

  for(int i=0;i<n;i++){
    indexes[i]=i;
  }

  T = helper_buildvp(X,indexes,n,d);
  free(indexes);
  return T;
}

vptree * getInner(vptree * T){
  return T->inner;
}

vptree * getOuter(vptree * T){
  return T->outer;
}

double getMD(vptree * T){
  return T->md;
}

double * getVP(vptree * T){
  return T->vp;
}

int getIDX(vptree * T){
  return T->idx;
}

void destroy(vptree *T){

  if (T == NULL){
    return;
  }

  destroy(T->inner);
  destroy(T->outer);

  free(T->vp);
  free(T);
}
