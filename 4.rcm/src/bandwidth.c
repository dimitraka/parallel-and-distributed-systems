/*
      Parallel and Distributed Systems
      \file   bandwidth.c
      \brief  Implementation of bandwidth

      \author Dimitra Karatza
      \AEM    8828
      \date   2020-09-30
*/

#include "../inc/rcm.h"
#include <stdio.h>
#include <stdlib.h>

// Create graph

node *createNode(int v){
	node *newNode = malloc(sizeof(node));
	newNode->vertex = v;
	newNode->next = NULL;

	return newNode;
}

Graph *createGraph(int vertices){
	Graph *graph = malloc(sizeof(Graph));

	graph->num_of_vertexes = vertices;
	graph->adjacent = malloc(vertices * sizeof(node *));

	for (int i = 0; i < vertices; i++)
		graph->adjacent[i] = NULL;

	return graph;
}

void addEdge(Graph *graph, int s, int d){
	//Add edge from s to d
	node *newNode = createNode(d);
	newNode->next = graph->adjacent[s];
	graph->adjacent[s] = newNode;

	//Add edge from d to s
	newNode = createNode(s);
	newNode->next = graph->adjacent[d];
	graph->adjacent[d] = newNode;
}


// Find the bandwidth of the new created array
int find_bandwidth_new(int *permutation, int n, int *X){

  int *new_labels = malloc(n * sizeof(int));
  for (int i = 0; i < n; i++)
    new_labels[permutation[i]] = i;

	// Create the input graph using the input matrix
	Graph *input_graph = createGraph(n);
	for (int i = 0; i < n; i++)
	    for (int j = i + 1; j < n; j++)
	        if (X[n * i + j])
	            addEdge(input_graph, i, j);

  // Create the output graph using the permutations
  Graph *output_graph = createGraph(n);
  for (int i = 0; i < n; i++){
     struct node *temp = input_graph->adjacent[permutation[i]];
     while (temp)
     {
         addEdge(output_graph, i, new_labels[temp->vertex]);
         temp = temp->next;
     }
  }

  // Initialize the output matrix with zeros
  for (int i = 0; i < n; i++)
      for (int j = 0; j < n; j++)
          if (i == j)
              X[n * i + j] = 1;
          else
              X[n * i + j] = 0;

  // Create the output matrix using output graph
  for (int v = 0; v < output_graph->num_of_vertexes; v++){
      struct node *temp = output_graph->adjacent[v];
      while(temp){
          X[n * v + temp->vertex] = 1;
          temp = temp->next;
      }
  }

  return find_bandwidth(X,n);

}


// Find the bandwidth of the input array
int find_bandwidth(int *X, int n){
	int high_value = 0;
	int low_value = 0;
	int temp;

	for (int i = 0; i < n; i++){
		temp = 0;
		for (int j = n - 1; j > i; j--)
			if (X[n * i + j] != 0){
				temp = j - i;
				break;
			}
			else
				continue;

		if (temp > high_value)
			high_value = temp;

		temp = 0;
		for (int j = 0; j < i; j++)
			if (X[n * i + j] != 0){
				temp = i - j;
				break;
			}
			else
				continue;

		if (temp > low_value)
			low_value = temp;
	}

	int bandwidth = low_value + high_value + 1;

	return bandwidth;
}
