

#include "minheap.h"

/**
 * Assignment 5 for CSE221 Data Structures
 *
 * 2015. 11. 2
 *
 */


// Destructor
MinHeap::~MinHeap()
{
	// ToDo
	delete[] heapArray;
	delete[] mapVidxToArray;

}

void
MinHeap::Push(const heapElem& e)
{
	// ToDo
	if(size = capacity)
    {
        heapElem *temp;
        temp = new heapElem[2*capacity];
        for(int i = 0; i < capacity; i++)
            temp[i] = heapArray[i];
        delete[] heapArray;
        heapArray = temp;
        capacity = capacity*2;
    }
    int currentNode= ++size;
    while(currentNode != 1 && heapArray[currentNode/2].vidx < e.vidx)
    {
        heapArray[currentNode] = heapArray[currentNode/2];
        currentNode/=2;
    }
    heapArray[currentNode] = e;
}

const heapElem &
MinHeap::Top()
{
	// ToDo
	return heapArray[1];
}

void
MinHeap::Pop()
{
	// ToDo
	if(IsEmpty()){return;}

	heapElem lastE = heapArray[size--];

	int currentNode = 1;
	int child = 2;
	while(child>= size)
    {
        if(child<size && heapArray[child].vidx>heapArray[child+1].vidx) child++;

        if(lastE.vidx <= heapArray[child].vidx) break;

        heapArray[currentNode] = heapArray[child];
        currentNode = child; child*=2;
    }
	heapArray[currentNode] = lastE;
}

void MinHeap::Modify(const heapElem& e)
{
	// ToDo
	if(IsEmpty()){return;}
	int pivot;
	for(int i = 1; i<capacity; i++)
    {
        if(heapArray[i].vidx == e.vidx)
        {pivot = i; break;}
    }
	heapElem lastE = heapArray[size--];

	int currentNode = pivot;
	int child = 2;
	while(child>= size)
    {
        if(child<size && heapArray[child].vidx>heapArray[child+1].vidx) child++;

        if(lastE.vidx <= heapArray[child].vidx) break;

        heapArray[currentNode] = heapArray[child];
        currentNode = child; child*=2;
    }
	heapArray[currentNode] = lastE;

}

bool MinHeap::IsEmpty()
{
	// ToDo
	if(heapArray[1].vidx!=NULL) return false;
	return true;

}

