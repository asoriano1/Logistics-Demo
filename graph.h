#ifndef GRAPH
#define GRAPH

#include<iostream>
#include <list>
#include <time.h>
#include <vector>

using namespace std;

class Graph
{
	int V; // No. of vertices

		   // Pointer to an array containing adjacency
		   // lists
	list<int> *adj;

public:
	Graph(int V); // Constructor

	// function to add an edge to graph
	void addEdge(int v, int w);

	// prints BFS traversal from a given source s
	void BFS(int s);
};
#endif