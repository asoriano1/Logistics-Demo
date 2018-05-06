// ConsoleApplication1.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"

/*
const int maze_size_x=80;
const int maze_size_y=25;
vector < vector < bool > > maze;

maze.resize(maze_size_y);
for (size_t y=0;y<maze_size_y;y++)
maze[y].resize(maze_size_x);

for (size_t x=0;x<maze_size_x;x++)
for (size_t y=0;y<maze_size_y;y++)
maze[y][x]=false;
*/


// Program to print BFS traversal from a given
// source vertex. BFS(int s) traverses vertices 
// reachable from s.


#include<iostream>
#include <list>
#include <time.h>
#include <vector>

#include "graph.h"
#include "pose.h"
#include "robot.h"
#include "scenario.h"
#include "cart.h"

using namespace std;


// Driver program to test methods of graph class
int main()
{

	const int maze_size_x = 80;
	const int maze_size_y = 25;
	const int num_robots = 3;
	vector < vector < bool > > maze;

	Scenario scn= Scenario(3,3);
	Robot r1 = Robot("r1",2,0);
	Robot r2 = Robot("r2", 2, 1);

	Cart c1 = Cart("c1", 0, 0);
	Cart c2 = Cart("c2", 0, 1);

	scn.addRobot(r1);
	scn.addRobot(r2);

	scn.addCart(c1);
	scn.addCart(c2);

	scn.print();
		
	// Create a graph given in the above diagram
	/*Graph g(4);
	g.addEdge(0, 1);
	g.addEdge(0, 2);
	g.addEdge(1, 2);
	g.addEdge(2, 0);
	g.addEdge(2, 3);
	g.addEdge(3, 3);

	cout << "Following is Breadth First Traversal "
		<< "(starting from vertex 2) \n";
	g.BFS(2);*/

	return 0;
}



