#include "dijkstra.h"
#include <fstream>

/** 
  * Assignment 5 for CSE221 Data Structures
  *
  * 2015. 11. 2
  *
  */


Dijkstra::~Dijkstra()
{
	if(mheap!=NULL) delete mheap;
	if(adjList!=NULL){
		unsigned int numV=adjList->idx;
		graphNode* currentNode;
		graphNode* tempNode;
		for(unsigned int i=0; i<numV; i++){
			currentNode=&(adjList->link[i]);
			tempNode=currentNode->link;
			for(unsigned int j=0; j<adjList->link[i].idx; j++){
				currentNode=tempNode;
				if(j+1<adjList->link[i].idx) tempNode=currentNode->link;
				delete currentNode;			
			}
		}
		delete[] adjList->link;
		delete adjList;
	}
}


void 
Dijkstra::ReadGraph(const char* file)
{
	//number of vertices & edges, 2 vertex element of edge, weight 
	unsigned int numV, numE, u, v;
	double wght; 

	std::ifstream is;
	is.open(file);

	is>>numV>>numE;
	mheap=new MinHeap(numE);
	adjList=new graphNode;
	adjList->idx=numV; //idx of adjList means the number of vertices
	adjList->link=new graphNode[numV]; 
	//adjList is linked to the array of head nodes
	graphNode* currentNode[numV];
	for(unsigned int i=0; i<numV; i++){
		currentNode[i]=&(adjList->link[i]);
		adjList->link[i].idx=0; 
		// idx of head node means the number of chains
	}
	for(unsigned int i=0; !is.eof()&&i<numE; i++){
//directed graph
		is>>u>>v>>wght;
		currentNode[u]->link=new graphNode;
		currentNode[u]=currentNode[u]->link;
		currentNode[u]->idx=v;
		currentNode[u]->weight=wght;
		adjList->link[u].idx++;
/*undirected graph
		currentNode[v]->link=new graphNode;
		currentNode[v]=currentNode[v]->link;
		currentNode[v]->idx=u;
		currentNode[v]->weight=wght;
		adjList->link[v].idx++;
*/
	}

	is.close();
}


double 
Dijkstra::FindPath(const unsigned int v0, const unsigned int v1)
{
	graphNode* currentNode;
	heapElem e;
	unsigned int v,w;
	unsigned int numV=adjList->idx;
	double dst[numV]; //distance
	bool s[numV];
	unsigned int path[numV][10000]; 
	//list of vertices consisting path from v0 to w
	//10000 is the capacity of the path
	unsigned int numPath[numV]; 
	//number of vertices consisting path
	for(v=0; v<numV; v++){
		numPath[v]=0;
	}

	//for all v in the graph
	for(v=0; v<numV; v++){
		dst[v]=99999999999999; // infinite distance
		s[v]=false;
	}

	//set dist[v0]=0 and push it to heap
	dst[v0]=0;
	e.vidx=v0; e.dist=dst[v0];
	mheap->Push(e);

	//while heap is not empty
	while( !mheap->IsEmpty() ){
		//v=pop the top element from the heap
		v=mheap->Top().vidx;
		mheap->Pop();

		s[v]=true;
		if(v==v1) break;

		//for all adjacent w of v
		currentNode=&(adjList->link[v]);
		for(unsigned int i=0; i<adjList->link[v].idx; i++){
			currentNode=currentNode->link;
			w=currentNode->idx;

			if(!s[w] && dst[w]> dst[v]+currentNode->weight){
				dst[w]=dst[v]+currentNode->weight;

				//store v as the previous vertex of w
				numPath[w]=numPath[v];
				for(unsigned int j=0; j<numPath[v]; j++){
					path[w][j]=path[v][j];
				}
				path[w][ numPath[w] ]=w;
				numPath[w]++;

				//if w is not in the heap, push w to heap
				//else update heap for w
				e.vidx=w;
				e.dist=dst[w];
				if( mheap->IsInHeap(e) ) mheap->Modify(e);
				else mheap->Push(e);
			}
		}		
	}

	if(s[v1]){
		printf("%u", v0);
		for(unsigned int i=0; i<numPath[v1]; i++) printf(",%u", path[v1][i]);
		printf("\nDistance : %f\n", dst[v1]);
		return dst[v1];
	}
	else{
		printf("No path");
		return -1;
	}
}
