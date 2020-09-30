/*
      Parallel and Distributed Systems
      \file   rcm.h
      \brief  Header file for the rcm project

      \author Dimitra Karatza
      \AEM    8828
      \date   2020-09-30
*/

#ifndef RCM_H
#define RCM_H

typedef struct Queue{
  int capacity;
  int size;
  int front;
  int rear;
  int *elements;
}Queue;

typedef struct node
{
	int vertex;
	struct node *next;
} node;

typedef struct Graph
{
	int num_of_vertexes;
	node **adjacent;
} Graph;

Queue * createQueue(int maxElements);
int *rcm(int *M, int n, Queue *Q, Queue *R);
int find_bandwidth(int *X, int n);
int find_bandwidth_new(int *X, int n,int *M);
void swap(int* a, int* b);
void Dequeue(Queue *Q);
int front(Queue *Q);
void Enqueue(Queue *Q,int element);
void quickSort(int arr1[], int arr2[], int low, int high);

#endif
