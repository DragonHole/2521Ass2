// Floyd Warshall ADT interface
// COMP2521 Assignment 2

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "FloydWarshall.h"
#include "Graph.h"

// returns the smaller int from the two parameters
static int min(int x1, int x2);

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
	sps->numNodes = n;

	// my 2D memo table 
	sps->next = calloc(n*n, sizeof(int));
	sps->dist = calloc(n*n, sizeof(int));
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			sps->dist[i][j] = INFINITY;
			sps->next[i][j] = -1;
	}

	// do a deep copy of the input matrix from its in and out Adjacent lists
	AdjList l = NULL;
	for(int i = 0; i < n; i++){
		l = GraphOutIncident(g, i); // vertices start from 0 to n-1
		// deep copy out-list
		while(l){
			sps->dist[i][l->v] = l->weight;
			sps->next[i][l->v] = l->v; // by default? not sure why
			l = l->next;
		}

		// deep copy in-list 
		l = GraphInIncident(g, i);
		while(l){
			sps->dist[l->v][i] = l->weight;
			sps->next[l->v][i] = i; // by default, not sure why...
			l = l->next;
		}
	}

	// set the diagonal line to 0, since every vertex is 0 distance from itself
	for(int i = 0; i < n; i++){
		sps->dist[i][i] = 0; 
		sps->next[i][i] = i; // myself to myself..
	}

	// start processing
	for(k = 0; k < n; k++){
		for(int i = 0; i < n; i++){
			for(int j = 0; j < n; j++){
				if(sps->dist[i][j] > sps->dist[i][k] + sps->dist[k][j]){
					sps->dist[i][j] = sps->dist[i][k] + sps->dist[k][j];
					// deal with path
					sps->next[i][j] = sps->next[i][k]; 
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

static int min(int x1, int x2){
	return x1 < x2 ? x1 : x2;
}