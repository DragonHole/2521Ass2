// Centrality Measures ADT interface
// COMP2521 Assignment 2

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "CentralityMeasures.h"
#include "FloydWarshall.h"
#include "Graph.h"

/**
 * Finds  the  edge  betweenness  centrality  for each edge in the given
 * graph and returns the results in a  EdgeValues  structure.  The  edge
 * betweenness centrality of a non-existant edge should be set to -1.0.
 */
EdgeValues edgeBetweennessCentrality(Graph g) {
	EdgeValues e = {0};

	int n = GraphNumVertices(g);
	e.numNodes = n;

	e.values = malloc(n * sizeof(double *));
	for(int i = 0; i < n; i++){
    	e.values[i] = malloc(n * sizeof(double));
    	for(int j = 0; j < n; j++)
    		e.values[i][j] = -1;	// default all to -1
	}

	// make sure all valid edges have value 0
	// GraphOutIncident is the same, just switch i and l->v around
	AdjList l = NULL;
	for(int i = 0; i < n; i++){
		l = GraphInIncident(g, i); // vertices start from 0 to n-1
		// deep copy out-list
		while(l){
			e.values[l->v][i] = 0;
			l = l->next;
		}
	}

    // use FloydWarshall
    ShortestPaths sps = FloydWarshall(g);

    int c;

    // calculate betweenness centrality for each edge 
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			// retrieve the path 

			// if path from i to j doesn't exist, then skip this j
			if(sps.next[i][j] == -1)
				continue;

			// start from i 
			c = i;

			// traverse along this shortest path 
			while(c != j){
				if(e.values[c][sps.next[c][j]] == -1)
					e.values[c][sps.next[c][j]] = 0;

				e.values[c][sps.next[c][j]] += 1; // just 1 or 0, double is basically wasted
				c = sps.next[c][j];
			}
		}
	}

	return e;
}

/**
 * Prints  the  values in the given EdgeValues structure to stdout. This
 * function is purely for debugging purposes and will NOT be marked.
 */
void showEdgeValues(EdgeValues evs) {

}

/**
 * Frees all memory associated with the given EdgeValues  structure.  We
 * will call this function during testing, so you must implement it.
 */
void freeEdgeValues(EdgeValues evs) {
	for(int i = 0; i < evs.numNodes; i++){
		free(evs.values[i]);
	}
	free(evs.values);
}


