// Node.cpp
//Class Number: CSE24102
//Student Number: 20131674
//Name: Gho Jeong Wan

#include "Node.h"

    // default constructor
Node::Node(){
	word="";
	next=NULL;
	prev=NULL;
}

    // constructor with single argument
Node::Node(std::string s){
	word=s;
	next=NULL;
	prev=NULL;
}

    // constructor with 2 arguments
    // 2nd argument is a pointer to prev node
    // primary constructor
Node::Node(std::string s, Node* n){
	word=s;
	next=NULL;
	prev=n;
}

    // accessor for word
    // returns contents of word
std::string& Node::getWord(){
	return word;
}

    // accessor for next
    // returns pointer to node to which next is pointing
Node* Node::getNext() const{
	return next;
}

    // accessor for next
    // returns pointer to node to which prev is pointing
Node* Node::getPrev() const{
	return prev;
}

    // mutator for word
    // changes string to value passed in
void Node::setWord(std::string s){
	word=s;
}

    // mutator for next
    // changes pointer to value passed in
void Node::setNext(Node* n){
	next=n;
}

    // mutator for prev
    // changes pointer to value passed in
void Node::setPrev(Node* n){
	prev=n;
}

