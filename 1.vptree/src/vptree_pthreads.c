/*!
  \file   vptree_pthreads.c
  \brief  Pthreads Version for Vantage Point implementation.

  \author Dimitra Karatza
  \AEM    8828
  \date   2019-11-4
*/
#include "../inc/vptree.h"
#include "quickselect.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

int activeThreads = 0, maxThreads = 16;
int sequential=0;
pthread_attr_t attr;
pthread_mutex_t lock;

typedef struct{
  int id;
  double *X;
  double *dis;
  int index,max;
  int n,d;
}parmDis;

typedef struct{
  double *X;
  int *indexes;
  int n,d;
}parmSubtree;

vptree * helper_buildvp(double *X, int *indexes, int n, int d);

void *distanceThread(void *arg){

  parmDis *p = (parmDis *)arg;
  int n=p->n;
  int d=p->d;

  double S;

  for(int i=p->index; i<p->max ;i++){

    S=0;
    for(int j=0;j<d;j++){
      S=S+pow((p->X)[i+j*n]-(p->X)[n-1+j*n],2);
    }
    (p->dis)[i]=sqrt(S);
  }
  pthread_exit(NULL);
}

double* getDistanceParallel(double *X, int n, int d){
  pthread_t threads[maxThreads];
  int rc;
  parmDis *p;
  int t=0;
  int split=n/maxThreads;
  int left=n-split*maxThreads;

  p = (parmDis *)malloc(sizeof(parmDis)*maxThreads);
  p->dis=malloc(n*sizeof(double));

  for(t=0; t<maxThreads; t++){
    //creating 1st thread
    //printf("In threadCreation: creating thread %ld\n", t);
    p[t].id=t;
    p[t].X=X;
    p[t].dis=p->dis;
    p[t].n=n;
    p[t].d=d;

    if(t==0){
      p[t].index=split*t;
    }else{
      p[t].index=p[t-1].max;
    }
    if(left>0){
      p[t].max=p[t].index+split+1;
      left--;
    }else{
      p[t].max=p[t].index+split;
    }
    //printf("Thread %d starts from %d and has max %d\n",t,p[t].index,p[t].max);

    rc = pthread_create(&threads[t], NULL, distanceThread, (void *)(p+t));
    if (rc){
      printf("ERROR; return code from pthread_create() is %d\n", rc);
      exit(-1);
    }
  }

  for (int i = 0; i<maxThreads; i++) {
    pthread_join(threads[i],NULL);
  }

  return p->dis;
}

double* getDistanceSequential(double *X, int n, int d){

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

void* helper_buildvp_parallel(void * argSubtree){
  parmSubtree *p = (parmSubtree *)argSubtree;
  int n=p->n;
  int d=p->d;
  double *X=p->X;
  int *indexes=p->indexes;

  pthread_t o1,o2; //outer subtree
  void *r1, *r2; //result

  //same as helper_buildvp
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

  if(n<10000){
    dis=getDistanceSequential(X,n,d);
  }else{
    dis=getDistanceParallel(X,n,d);
  }

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
  //end of same as helper_buildvp

  int parallel = 0;
  parmSubtree *p_inner = malloc(sizeof(*p_inner));
  p_inner->X = innerX;
  p_inner->n = sizeInner;
  p_inner->d = d;
  p_inner->indexes = idxInner;
  parmSubtree *p_outer = malloc(sizeof(*p_outer));
  p_outer->X = outerX;
  p_outer->n = sizeOuter;
  p_outer->d = d;
  p_outer->indexes = idxOuter;

  if (activeThreads < maxThreads) {
    pthread_mutex_lock (&lock);
    activeThreads+=2;
    pthread_mutex_unlock (&lock);
    pthread_create( &o1, &attr, helper_buildvp_parallel, (void *)p_inner);
    pthread_create( &o2, &attr, helper_buildvp_parallel, (void *)p_outer);
    parallel = 1;
  }

  if (parallel) {
    pthread_join(o1,&r1);
    pthread_join(o2,&r2);
    pthread_mutex_lock (&lock);
    activeThreads-=2;
    pthread_mutex_unlock (&lock);

    T->inner = (vptree *)r1;
    T->outer = (vptree *)r2;
  }else {
    sequential=1;
    T->inner=helper_buildvp(innerX,idxInner,sizeInner,d);
    T->outer=helper_buildvp(outerX,idxOuter,sizeOuter,d);
  }

  free(idxInner);
  free(idxOuter);
  free(outerX);
  free(innerX);
  free(dis);
  free(sortedDis);
  return (void *) T;
}

vptree * helper_buildvp(double *X, int *indexes, int n, int d){
  vptree *T;

  if(sequential==1 || n<10000){
    //create a pointer to a new structure of type vptree
    T=NULL;

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

    if(n<10000){
      dis=getDistanceSequential(X,n,d);
    }else{
      dis=getDistanceParallel(X,n,d);
    }

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
  }else{
    //parallel subtree
    pthread_t t;
    void*result;
    parmSubtree *newParm=(parmSubtree *)calloc(1,sizeof(parmSubtree));
    newParm->X = X;
    newParm->indexes = indexes;
    newParm->n = n;
    newParm->d = d;
    pthread_create( &t, NULL, helper_buildvp_parallel, (void *) newParm);
    pthread_join(t,&result);
    T=(vptree*)result;
    free(newParm);
  }

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
