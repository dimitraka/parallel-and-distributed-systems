## Bandwidth reduction - The Reverse CutHill-McKee Algorithm ###
### Parallelized using OpenMP ###

![results](https://github.com/dimitraka/parallel-and-distributed-systems/4.rcm//blob/master/graph.png?raw=true)

#### The Bandwidth Reduction problem ####
For a given symmetric sparse matrix, M(nxn), the problem is to reduce its bandwidth B by permuting rows and columns such as to move all the nonzero elements of M in a band as close as possible to the diagonal.

#### About the RCM algorithm ####
The Reverse Cuthill-McKee (RCM) algorithm is a well-known heuristic for reordering sparse matrices. It is typically used to speed up the computation of sparse linear systems of equations. By an appropriate renumbering of the nodes, it is often possible to produce a matrix with a much smaller bandwidth.

#### The algorithm ####

For a given graph G(n):

- Step 0: Prepare an empty queue Q and an empty result array R.
- Step 1: Select the node in G(n) with the lowest degree (ties are broken arbitrarily) that hasn't previously been inserted in the result array. Let us name it P (for Parent).
- Step 2: Add P in the first free position of R.
- Step 3: Add to the queue all the nodes adjacent with P in the increasing order of their degree.
- Step 4.1: Extract the first node from the queue and examine it. Let us name it C (for Child).
- Step 4.2: If C hasn't previously been inserted in R, add it in the first free position and add to Q all the neighbors of C that are not in R in the increasing order of their degree.
- Step 5: If Q is not empty repeat from Step 4.1 .
- Step 6: If there are unexplored nodes (the graph is not connected) repeat from Step 1 .
- Step 7: Reverse the order of the elements in R. Element R[i] is swapped with element R[n+1-i].

The result array will be interpreted like this: R[L] = i means that the new label of node i (the one that had the initial label of i) will be L.

#### Parallelization of RCM####

RCM is an algorithm worth parallelizing due to its multiple and time-consuming calculations for matrices with large dimensions. There were noticed two main parts whose parallelization enhances the execution time of the algorithm.



#### How to run ####

`make`: To compile and execute both sequential and parallel implementations.

`make sequential`: To compile and execute only the sequential implemetation.

`make openmp`: To compile and execute only the parallel implemetation.
