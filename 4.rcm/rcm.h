/*
      Parallel and Distributed Systems
      \file   rcm.h
      \brief  Header file for the rcm project

      \author Dimitra Karatza
      \AEM    8828
      \date   2020-09-19
*/

typedef struct Queue{
  int capacity;
  int size;
  int front;
  int rear;
  int *elements;
}Queue;
