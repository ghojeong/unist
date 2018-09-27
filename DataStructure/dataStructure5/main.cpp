#include "dijkstra.h"
#include <iostream>

/** 
 * Assignment 5 for CSE221 Data Structures
 *
 * 2015. 11. 2
 *
 */


int main()
{
	Dijkstra ds;
	
	ds.ReadGraph("graph-medium.txt");
	
	std::cout << "Shortest path between 0 and 9201 : \n";
	
	ds.FindPath(0,9201);
	
	return 0;
}
