//Assignment #5
//Class Number: CSE24102
//Student Number: 20131674
//Name: Gho Jeong Wan

#include "ExtendableArray.h"
#include <ostream>
using namespace std;

ElementRef::ElementRef( ExtendableArray& theArray, int i ){
	intArrayRef=&theArray;
	index=i;
}
ElementRef::ElementRef( const ElementRef& other ){
	intArrayRef = other.intArrayRef;
	index = other.index;
} // copy constructor
ElementRef::~ElementRef(){}ElementRef& ElementRef::operator=( const ElementRef& rhs ){
	(*intArrayRef).arrayPointer[index] = (*rhs.intArrayRef).arrayPointer[rhs.index];
	return *this;
}
ElementRef& ElementRef::operator=( int val ){
	(*intArrayRef).arrayPointer[index]=val;
	return *this;
}
ElementRef::operator int() const{
	return (*intArrayRef).arrayPointer[index];
}



ExtendableArray::ExtendableArray(){
	size=2;
	arrayPointer=new int[size];
	for(int i=0; i<size; i++)
		arrayPointer[i]=0;
} // allocates memory space for 2 integers
ExtendableArray::ExtendableArray( const ExtendableArray& other ){
	size=other.size;
	arrayPointer=new int[size];
	for (int i=0; i<size; i++)
		arrayPointer[i]=other.arrayPointer[i];
} // copy constructor
ExtendableArray::~ExtendableArray(){
	delete [] arrayPointer;
} // destructor
ExtendableArray& ExtendableArray::operator=( const ExtendableArray& rhs ){
	if (this == &rhs) return *this;
	delete [] arrayPointer;
	size=rhs.size;
	arrayPointer=new int[size];
	for (int i=0; i<size; i++)
		arrayPointer[i]=rhs.arrayPointer[i];
	return *this;
}
ElementRef ExtendableArray::operator[]( int i ){
	if (i>2147483646 || i<0)
		i=size;
	if (i+1>size){
		ExtendableArray temp(*this);
		delete [] arrayPointer;
		size=i+1;
		arrayPointer=new int[size];
		for(int j=0; j<size; j++)
			arrayPointer[j]=0;
		for (int j=0; j<temp.size; j++)
			arrayPointer[j]=temp.arrayPointer[j];
		ElementRef erTrue(*this, i);
		return erTrue;
	}
	ElementRef erFalse(*this, i);
	return erFalse; 
}
