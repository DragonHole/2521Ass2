// Girvan-Newman Algorithm for community discovery
// COMP2521 Assignment 2

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "CentralityMeasures.h"
#include "GirvanNewman.h"
#include "Graph.h"

static void dfsComponent(int **g, int size, int *componentOf, int componentID, int vertex);
//static void insertDendrogram(Dendrogram dg, int *componentOf, int vertex);
//static bool isOnlyElementOfComponent(int vertex, int *componentOf);

/**
 * Generates  a Dendrogram for the given graph g using the Girvan-Newman
 * algorithm.
 * 
 * The function returns a 'Dendrogram' structure.
 */
Dendrogram GirvanNewman(Graph g) {
	
	int n = GraphNumVertices(g);
	EdgeValues ev = edgeBetweennessCentrality(g);
	
	Dendrogram dg = malloc(sizeof(DNode));
	
	int max = 0;	
	// find the edge with highest betweenness 
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			if(ev.values[i][j] > max)
				max = ev.values[i][j];
		}
	}
	printf("highest betweenness: %d\n", max);
	
	
	// adjacency matrix form of the graph
	int **graphCopy = malloc(n * sizeof(int *));
	for(int i = 0; i < n; i++){
	    graphCopy[i] = malloc(n * sizeof(int));
		for(int j = 0; j < n; j++)
			graphCopy[i][j] = -1;	// default all to -1
	}
	
	// get the graph edges 
	AdjList l = NULL;
	for(int i = 0; i < n; i++){
		l = GraphOutIncident(g, i); // vertices start from 0 to n-1
		while(l){
			graphCopy[i][l->v] = l->weight;
			printf("%d -> %d = %d \n", i, l->v, l->weight);
			l = l->next;	
		}
	}
	
	//debug, print the memo table out
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			printf("%10d ", graphCopy[i][j]);
		}
		putchar('\n');
	}
	
	// delete edges with highest betweenness
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			if(ev.values[i][j] == max){
				graphCopy[i][j] = -1;
				ev.values[i][j] = -1;
			}	
		}
	}
	putchar('\n');
	
	// to make it undirected graph, since we just want to see if weakly connected, don't care strongly connectedness.
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			if(graphCopy[i][j]){
				graphCopy[j][i] = graphCopy[i][j]; // for those left undeleted, make undirected for DFS
			}	
		}
	}
	
	//debug, print the memo table out
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			printf("%10d ", graphCopy[i][j]);
		}
		putchar('\n');
	}
	
	// mark the components after removing highest between vertices
	// record which component does each vertex belong to 
	int componentID = 1;
	int *componentOf = malloc(n * sizeof(int));
	for(int i = 0; i < n; i++)
		componentOf[i] = -1;

	// for(int i = 0; i < n; i++){
	//     componentOf[i] = malloc(n * sizeof(int));
	// 	for(int j = 0; j < n; j++)
	// 		componentOf[i][j] = -1;	// default all to -1
	// }
	
	// apply dfsComponent to every vertex of the graph (mark a componentID)
	for(int i = 0; i < n; i++){
		if(componentOf[i] == -1){
			dfsComponent(graphCopy, n, componentOf, componentID, i);
			componentID++;
		}
	}
	
	// debug, show the components of each vertex
	for(int i = 0; i < n; i++){
		printf("%d: component %d \n", i, componentOf[i]);
	}
	
	
	
	// after everything
	// add the vertices into subtrees in groups of their componentID
	// for(int i = 0; i < n; i++){
	// 	//insertDendrogram(dg, componentOf, i);
	//
	// }
			
	return dg;
	//return NULL;
}

/**
 * Frees all memory associated with the given Dendrogram  structure.  We
 * will call this function during testing, so you must implement it.
 */
void freeDendrogram(Dendrogram d) {
	// TODO: Implement this function
}

static void dfsComponent(int **g, int size, int *componentOf, int componentID, int vertex){
	componentOf[vertex] = componentID;
	
	for(int k = 0; k < size; k++){
		if(g[vertex][k] != -1 && componentOf[k] == -1){
			dfsComponent(g, size, componentOf, componentID, k);
		}
	}
}

// static void insertDendrogram(Dendrogram dg, int *componentOf, int vertex){
// 	if(dg == NULL){
// 		Dendrogram newNode = malloc(sizeof(DNode));
// 		newNode->vertex = vertex;
// 		newNode->left = NULL;
// 		newNode->right = NULL;
//
// 		return newNode
// 	}
// 	else if(){
// 		dg->left = insertDendrogram(dg->left, componentOf, vertex);
// 	}
// 	else if(dg->right == NULL){
// 		dg->right = insertDendrogram(dg->right, componentOf, vertex);
// 	}
//
//
//
//
//
// 	// if(isOnlyElementOfComponent(vertex, componentOf)){ // not a leaf node
// 	//
// 	// }
// 	// else{ // leaf node
// 	//
// 	// }
// }


