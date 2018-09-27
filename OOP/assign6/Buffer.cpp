// Buffer.cpp
//Class Number: CSE24102
//Student Number: 20131674
//Name: Gho Jeong Wan

#include "Buffer.h"
#include <iostream>
using namespace std;

    // default constructor
    // creates empty linked list of count = 0
Buffer::Buffer(){
	head=new Node;
	count=0;
}

    // copy constructor for buffer
    // creates a deep copy, of the ll,
    // and stores it in other
    // calls copy()
Buffer::Buffer(const Buffer & other){
	head=new Node;
	count=0;
	copy(other);
}

    // destructor
    // calls clear();
Buffer::~Buffer(){
	clear();
}

    // overloaded assignment operator
    // calls copy()
Buffer& Buffer::operator=(const Buffer & original){
	copy(original);
	return *this;
}

    // accessor
    // returns the head
Node * Buffer::getHead() const{
	return head;
}

    // accessor
    // returns the tail node
Node * Buffer::getTail(){
	return ( *head->getNext() ).getPrev();	
}

    // returns the string in the head node of the ll
    // this operation is not valid for an empty ll (i.e. buffer)
    // (If it is called on an empty buffer, return an emptry string "".)
std::string Buffer::getHeadElement(){
	if ( isEmpty() ){
		return "";
	}
	return ( *head->getNext() ).getWord();
}

    // returns the string in the tail node of the ll
    // this operation is not valid for an empty ll  (i.e. buffer)
    // (If it is called on an empty buffer, return an emptry string "".)
std::string Buffer::getTailElement(){
	if ( isEmpty() ){
		return "";
	}
	return ( *this->getTail() ).getWord();	
}

    // creates a node containing str
    // adds the string at the head of the ll
    // (i.e. before the current head node)
    // after the operation, head node is the newly added one
void Buffer::produceAtHead(const std::string str){
	if ( isEmpty() ){
		Node* newNodePtr=new Node(str);
		head->setNext(newNodePtr);
		newNodePtr->setNext(newNodePtr);
		newNodePtr->setPrev(newNodePtr);
		count=1;
	}
	else{
		Node* tailPtr=this->getTail();
		Node* firstPtr=head->getNext();
		Node* newNodePtr=new Node(str, tailPtr);
		head->setNext(newNodePtr);
		tailPtr->setNext(newNodePtr);
		firstPtr->setPrev(newNodePtr);
		newNodePtr->setNext(firstPtr);
		count++;
	}
}

    // creates a node containing str
    // adds the string at the end of the ll
    // (i.e. after the current tail node)
void Buffer::produceAtTail(const std::string str){
	if ( isEmpty() ){
		Node* newNodePtr=new Node(str);
		head->setNext(newNodePtr);
		newNodePtr->setNext(newNodePtr);
		newNodePtr->setPrev(newNodePtr);
		count=1;
	}
	else{
		Node* tailPtr=this->getTail();
		Node* firstPtr=head->getNext();
		Node* newNodePtr=new Node(str, tailPtr);
		newNodePtr->setNext( firstPtr );
		tailPtr->setNext(newNodePtr);
		firstPtr->setPrev( newNodePtr );
		count++;
	}
}

    // inserts Node containing "str" into the ll in front of "pos"
    // return an iterator that points to the the newly inserted elements.
    // any iterators after pos are considered invalid
Iterator Buffer::produceAtMiddle(Iterator pos, const std::string str){
	if ( isEmpty() ){
		Node* newNodePtr=new Node(str);
		head->setNext(newNodePtr);
		newNodePtr->setNext(newNodePtr);
		newNodePtr->setPrev(newNodePtr);
		count=1;
	}
	else{
		Node* currNodePtr=pos.getCurr();
		Node* prevNodePtr=currNodePtr->getPrev();
		Node* newNodePtr=new Node(str, prevNodePtr );
		newNodePtr->setNext(currNodePtr);
		currNodePtr->setPrev(newNodePtr);
		prevNodePtr->setNext(newNodePtr);
		count++;
		return Iterator(newNodePtr);
	}
}

    // buffer must not be empty before calling this
    // deletes the first element (i.e. current head node)
void Buffer::consumeAtHead(){
	if(count>1){
		Node* firstPtr=head->getNext();
		Node* secondPtr=firstPtr->getNext();
		Node* tailPtr=this->getTail();
		head->setNext(secondPtr);
		secondPtr->setPrev(tailPtr);
		tailPtr->setNext(secondPtr);
		delete firstPtr;
		count--;
	}
	else{
		clear();
		head=new Node;
	}
}

    // buffer must not be empty before calling this
    // deletes the last element (i.e. current tail node)
void Buffer::consumeAtTail(){
	if(count>1){
		Node* firstPtr=head->getNext();
		Node* tailPtr=this->getTail();
		Node* prevTailPtr=tailPtr->getPrev();
		prevTailPtr->setNext(firstPtr);
		firstPtr->setPrev(prevTailPtr);
		delete tailPtr;
		count--;
	}
	else{
		clear();
		head=new Node;
	}
}

    // "it" is pointing to the element to be removed
void Buffer::consume(Iterator it){
	if(count>1){
		if( it==getHeadItr() ){
			consumeAtHead();
		}
		else if( it==getTailItr() ){
			consumeAtTail();
		}
		else{
			Node* currNodePtr=it.getCurr();
			Node* prevNodePtr=currNodePtr->getPrev();
			Node* nextNodePtr=currNodePtr->getNext();
			prevNodePtr->setNext(nextNodePtr);
			nextNodePtr->setPrev(prevNodePtr);
			delete currNodePtr;
			count--;
		}
	}
	else{
		clear();
		head=new Node;
	}
}

    // erases all nodes in the range [s,t)
    // s is erased and all nodes up to but not
    // including t are erased
void Buffer::consume(Iterator s, Iterator t){
	Node* currNodePtr;
	Node* prevNodePtr;
	Node* nextNodePtr;

	Iterator it( s.getCurr() );
	while(it!=t){
		if(count>1){
			if( it==getHeadItr() ){
				it++;
				consumeAtHead();
			}
			else if( it==getTailItr() ){
				it++;
				consumeAtTail();
			}
			else{
				currNodePtr=it.getCurr();
				it++;
				prevNodePtr=currNodePtr->getPrev();
				nextNodePtr=currNodePtr->getNext();
				prevNodePtr->setNext(nextNodePtr);
				nextNodePtr->setPrev(prevNodePtr);
				delete currNodePtr;
				count--;
			}
		}
		else{
			clear();
			head=new Node;
			break;
		}
	}
}

    // returns the number of elements in the ll
unsigned Buffer::size() const{
	return count;
}

    // returns true if the ll is empty, otherwise it returns false
bool Buffer::isEmpty(){
	if(count>0){
		return false;
	}
	return true;
}

    // brackets operator allows indexing into the ll
    // "fakes" random access to the ith element of the ll
    // returns contents of ith element of the ll
    // if ll looks like:
    // we -> are -> family ->
    // then element 2 would be "family"
    // remember the first element is the 0th element
std::string& Buffer::operator[](int i) const{
	Iterator it(head);
	it+=i+1;
	return (*it.getCurr()).getWord();
}

    // returns the iterator pointing to the first node in the ll
Iterator Buffer::getHeadItr(){
	return Iterator( head->getNext() );
}

    // returns an iterator pointing the tail node in ll
Iterator Buffer::getTailItr(){
	return Iterator( this->getTail() );
}

    // returns an iterator of the next node pointed by it
Iterator Buffer::getNextItr(Iterator it){
	return ++it; 
}

    // returns an iterator of the previous node pointed by it
Iterator Buffer::getPrevItr(Iterator it){
	return --it; 
}

    // prints out the ll like the following
    // contents of node, followed by a space,
    // followed by -> followed by a space
    // after list is printed, it skips a line
    // ie has 2 endls
    // if list is empty, it prints "->" followed by
    // 2 endls
void Buffer::print(){
	if( isEmpty() ){
		cout<<"->";
	}
	else{
		Iterator it(head);
		for(int i=1; i<count; i++){
			cout<<*(it+i)<<" -> ";
		}
		cout<< *(it+count);
	}
	cout<<endl<<endl;
}


    // makes a deep copy of the ll for copy constructor
    // and assignment operator
    // "other" is copied
void Buffer::copy(const Buffer & other){
	clear();
	head=new Node;
	count=other.size();

	if(count>0){
		Iterator ite(other.getHead());
		Node* nodePtr=head;
		Node* prevNodePtr;	

		for(int i=1; i<=count; i++){
			prevNodePtr=nodePtr;			
			nodePtr=new Node(*(ite+i),prevNodePtr);
			prevNodePtr->setNext(nodePtr);
		}
		nodePtr->setNext( head->getNext() );
		( *head->getNext() ).setPrev(nodePtr);
	}
}

    // clears the ll by deleting all nodes
    // calls the recursive function deleteBuffer()
void Buffer::clear(){
	deleteBuffer(head);
}

    // recursively deletes the ll p
    // THIS FUNCTION MUST BE RECURSIVE
void Buffer::deleteBuffer(Node * p){
	Node* next;
	if(count>0){
		next=p->getNext();
		delete p;
		count--;
		deleteBuffer(next);
	}
	else{
		delete p;
		count=0;
	}
}

    // YOU MAY ADD ANY OTHER PRIVATE FUNCTIONS HERE

    // BUFFER_CLASS

    // non-member function
    // locates "str" in the range [first,last]
    // if "str" is found in the range, return true
    // if "str" is not found, return false
bool findNode(Iterator first, Iterator last, std::string str){
	Iterator i( first.getCurr() );
	while(i!=last){
		if( (*i).compare(str)==0 ){
			return true;
		}
		i++;
	}
	if( (*last).compare(str)==0 ){
		return true;
	}
	return false;	
}
