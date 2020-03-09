/*!
  \file   knnring_synchronous.c
  \brief  Synchronous communication for MPI implementation.

  \author Dimitra Karatza
  \AEM    8828
  \date   2019-12-10
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cblas.h>
#include "mpi.h"
#include "knnring.h"
#include "quickselect.h"


knnresult distrAllkNN(double * X, int n, int d, int k){

  knnresult result;
  double *D=malloc(n*n*sizeof(D)); //distances
  int *indices=malloc(n*n*sizeof(int)); //aray of indexes
  double *C=malloc(n*n*sizeof(double)); //result of cblas
  double temp;
  double S; //sum of elements
  double *Y=malloc(n*d*sizeof(double)); //new chunk of data that was received
  double *Xtmp=malloc(n*d*sizeof(double)); //temporary array of array Y


  //to know which is this process
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);



  //Define the destination and the departure process
  //so that the first and the last process can exchange messages
  int send=world_rank+1;
  int recv=world_rank-1;
  if(world_rank==world_size-1){
    send=0;
  }else if(world_rank==0){
    recv=world_size-1;
  }

  //In the beggining Y=X
  for(int i=0;i<n*d;i++){
    Y[i]=X[i];
  }

  //Calculation of distance and exchange of chunks of data must be done
  //as many times as the number of the processes. At the end,
  //each process has had access to the whole array X
  for(int p=0;p<world_size;p++){

    //Here starts the calculation of the distances and the search for
    //the k nearest neighbors.
    //This part of code is same as the V0 sequential version

    //Calculate the sum of query^2 (Y^2)
    //y1^2+y2^2+...+yn^2
    for(int i=0;i<n;i++){
      S=0;
      for(int j=0;j<d;j++){
        temp=pow(X[i+j*n],2);
        S+=temp;
      }
      //fill all the elements of array D which need this sum
      for(int j=0;j<n;j++){
        D[i+j*n]=S;
      }
    }

    //Calculate the sum of coprus (X^2)
    //x1^2+x2^2+...+xn^2
    for(int i=0;i<n;i++){
      S=0;
      for(int j=0;j<d;j++){
        temp=pow(Y[i+j*n],2);
        S+=temp;
      }
      for(int j=0;j<n;j++){
        D[i*n+j]+=S;
      }
    }

    //Multiplication of X and trans Y
    //-2*X*Y.'
    cblas_dgemm(CblasColMajor, CblasNoTrans, CblasTrans, n, n, d, 1, X, n, Y, n, 0, C, n);
    for(int i=0;i<n;i++){
      for(int j=0;j<n;j++){
        D[i+j*n]-=2*C[i+j*n];
        D[i+j*n]=sqrt(D[i+j*n]);
        //in case distance==0
        if(D[i+j*n]!=D[i+j*n] || D[i+j*n]<0.000001){
          D[i+j*n]=0.0;
        }
      }
    }

    //array of indexes
    for(int i=0;i<n;i++){
      for(int j=0;j<n;j++){

        indices[j+i*n] = (world_rank-1)*n+j-p*n;

        //modular arithmetic is applied to indices[]
        if(indices[j+i*n]<0){
            indices[j+i*n]=((world_rank-p-1)%world_size+world_size)*n+j;
        }
        if(indices[j+i*n]>=world_size*n){
            indices[j+i*n]=((world_rank-p-1)%world_size)*n+j;
        }

      }
    }




    //find the k nearest elements
    for(int i=0;i<n;i++){
      for(int j=k;j>0;j--){
        if(j==k){
          quickSelect(D,i*n,i*n+n,j,indices);
        }else{
          quickSelect(D,i*n,i*n+j,j,indices);
        }
      }
    }

    //Only for the first iteration there is no need to compare
    //the result with the previous and update the knnresult
    if(p==0){

      //return values
      result.m=n;
      result.k=k;
      result.ndist=malloc(n*k*sizeof(double));
      result.nidx=malloc(n*k*sizeof(int));

      //save the k smallest distances
      for(int i=0;i<k;i++){ //for every neighbor
        for(int j=0;j<n;j++){ //for every query point
          result.ndist[j+i*n]=D[i+j*n];
          result.nidx[j+i*n]=indices[j*n+i];
        }
      }

    }else{
      //check if there is any nearer neighbor found
      double *new=malloc(n*k*sizeof(double));
      int *newIdx=malloc(n*k*sizeof(int));
      for(int i=0;i<n;i++){ //for every element

        int j1=0,j2=0,z=0; //j2 is counter for ndist array and j1 counter for D array

        while (z<k){
          if(D[j1+i*n]<result.ndist[j2*n+i]){
            new[z*n+i]=D[j1+i*n];
            newIdx[z*n+i]=indices[j1+i*n];
            j1++;
            z++;
          }else{
            new[z*n+i]=result.ndist[j2*n+i];
            newIdx[z*n+i]=result.nidx[j2*n+i];
            j2++;
            z++;
          }
        }
      }
      result.ndist=new;
      result.nidx=newIdx;
    }

    //Now it's time to send the X array to the next process
    if(world_rank%2==0){
      MPI_Send(X, n*d, MPI_DOUBLE, send, 1, MPI_COMM_WORLD);
      MPI_Recv(X, n*d, MPI_DOUBLE, recv, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }else{
      for(int i=0;i<n*d;i++){
        Xtmp[i]=X[i];
      }
      MPI_Recv(X, n*d, MPI_DOUBLE, recv, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Send(Xtmp, n*d, MPI_DOUBLE, send, 1, MPI_COMM_WORLD);
    }

  }


  free(Y);
  free(Xtmp);
  free(D);
  free(indices);
  free(C);

  return result;
}
