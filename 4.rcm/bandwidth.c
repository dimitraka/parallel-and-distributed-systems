/*
      Parallel and Distributed Systems
      \file   bandwidth.c
      \brief  Implementation of bandwidth

      \author Dimitra Karatza
      \AEM    8828
      \date   2020-09-19
*/

#include "rcm.h"
#include <stdio.h>
#include <stdlib.h>

int find_bandwidth(int *X, int n);

typedef struct node
{
	int vertex;
	struct node *next;
} node;

typedef struct Graph
{
	int numVertices;
	node **adjLists;
} Graph;


node *createNode(int v)
{
	node *newNode = malloc(sizeof(node));
	newNode->vertex = v;
	newNode->next = NULL;

	return newNode;
}

Graph *createGraph(int vertices)
{
	Graph *graph = malloc(sizeof(Graph));
	graph->numVertices = vertices;

	graph->adjLists = malloc(vertices * sizeof(node *));

	for (int i = 0; i < vertices; i++)
		graph->adjLists[i] = NULL;

	return graph;
}

void addEdge(Graph *graph, int s, int d)
{
	//! Add edge from s to d
	node *newNode = createNode(d);
	newNode->next = graph->adjLists[s];
	graph->adjLists[s] = newNode;

	//! Add edge from d to s
	newNode = createNode(s);
	newNode->next = graph->adjLists[d];
	graph->adjLists[d] = newNode;
}

int find_bandwidth_new(int *permutation, int n, int *X){
  int *changes = malloc(n * sizeof(int));
  for (int i = 0; i < n; i++)
    changes[permutation[i]] = i;

//! Create the input graph according to input matrix
Graph *inp_graph = createGraph(n);
for (int i = 0; i < n; i++)
    for (int j = i + 1; j < n; j++)
        if (X[n * i + j])
            addEdge(inp_graph, i, j);

  //! Create the output graph
  Graph *out_graph = createGraph(n);
  for (int i = 0; i < n; i++)
  {
     struct node *temp = inp_graph->adjLists[permutation[i]];
     while (temp)
     {
         addEdge(out_graph, i, changes[temp->vertex]);
         temp = temp->next;
     }
  }

  //! Create output matrix and initialize it with zeros
  for (int i = 0; i < n; i++)
      for (int j = 0; j < n; j++)
          if (i == j)
              X[n * i + j] = 1;
          else
              X[n * i + j] = 0;

  //! Build the output matrix according to output graph
  for (int v = 0; v < out_graph->numVertices; v++)
  {
      struct node *temp = out_graph->adjLists[v];
      while (temp)
      {
          X[n * v + temp->vertex] = 1;
          temp = temp->next;
      }
  }

  return find_bandwidth(X,n);

}

int find_bandwidth(int *X, int n)
{
	int band_hi = 0;
	int band_lo = 0;
	int temp;

	for (int i = 0; i < n; i++)
	{
		temp = 0;
		for (int j = n - 1; j > i; j--)
			if (X[n * i + j] != 0)
			{
				temp = j - i;
				break;
			}
			else
				continue;

		if (temp > band_hi)
			band_hi = temp;

		temp = 0;
		for (int j = 0; j < i; j++)
			if (X[n * i + j] != 0)
			{
				temp = i - j;
				break;
			}
			else
				continue;

		if (temp > band_lo)
			band_lo = temp;
	}

	int bandwidth = band_lo + band_hi + 1;

	return bandwidth;
}
