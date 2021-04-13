// Floyd Warshall ADT interface
// COMP2521 Assignment 2

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "FloydWarshall.h"
#include "Graph.h"

/**
 * Finds all shortest paths between all pairs of nodes.
 * 
 * The  function  returns  a 'ShortestPaths' structure with the required
 * information:
 * - the number of vertices in the graph
 * - distance matrix
 * - matrix of intermediates (see description above)
 */
ShortestPaths FloydWarshall(Graph g) {
	// TODO: Implement this function
	ShortestPaths sps = {0};

	int n = GraphNumVertices(g);
	sps.numNodes = n;

	//printf("papa %d\n", n);

	// my 2D memo table 
	//sps.next = calloc(n*n, sizeof(int));
	//sps.next = malloc(sizeof(int[n][n]));
	sps.next = malloc(n * sizeof(int *));
	for(int i = 0; i < n; i++)
    	sps.next[i] = malloc(n * sizeof(int));

	sps.dist = malloc(n * sizeof(*sps.dist));
	for(int i = 0; i < n; i++)
    	sps.dist[i] = malloc(n * sizeof(*sps.dist[i]));


	// set default values for non-existent paths
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			sps.dist[i][j] = INFINITY;
			sps.next[i][j] = -1;
		}
	}

	// do a deep copy of the input matrix from its in and out Adjacent lists
	AdjList l = NULL;
	for(int i = 0; i < n; i++){
		l = GraphOutIncident(g, i); // vertices start from 0 to n-1
		// deep copy out-list, GraphInIncident is the same, just switch i and l->v around
		while(l){
			sps.dist[i][l->v] = l->weight;
			//printf("%d -> %d = %d \n", i, l->v, l->weight);
			sps.next[i][l->v] = l->v; // by default? not sure why
			l = l->next;
		}
	}

	// set the diagonal line to -1, no self loop.
	for(int i = 0; i < n; i++){
		sps.dist[i][i] = 0; // no self-loop 
		sps.next[i][i] = -1; //  = i? myself to myself.. already set in init above
	}

	// debug, print the memo table out
	// for(int k = 0; k < n; k++){
	// 	for(int i = 0; i < n; i++){
	// 		printf("%10d ", sps.dist[k][i]);
	// 	}
	// 	putchar('\n');
	// }


	// start procgessing
	for(int k = 0; k < n; k++){
		for(int i = 0; i < n; i++){
			for(int j = 0; j < n; j++){
				// check for INFINITY case
				if(sps.dist[i][k] == INFINITY ||  sps.dist[k][j] == INFINITY){
					continue;
				}

				if(sps.dist[i][j] > sps.dist[i][k] + sps.dist[k][j]){
					sps.dist[i][j] = sps.dist[i][k] + sps.dist[k][j];
					// deal with path
					sps.next[i][j] = sps.next[i][k]; 
					// bec sps->next stores the next vertex along the shortest path from i to j
					// bec above 'if' checked i->k->j is better than i->j.
				}
			}
		}
	}

	return sps;
}

/**
 * This  function  is  for  you to print out the ShortestPaths structure
 * while you are debugging/testing your implementation. 
 * 
 * We will not call this function during testing, so you may  print  out
 * the  given  ShortestPaths  structure in whatever format you want. You
 * may choose not to implement this function.
 */
void showShortestPaths(ShortestPaths sps) {

}

/**
 * Frees  all  memory associated with the given ShortestPaths structure.
 * We will call this function during testing, so you must implement it.
 */
void freeShortestPaths(ShortestPaths sps) {
	// TODO: Implement this function
}

