// Iterator.cpp
//Class Number: CSE24102
//Student Number: 20131674
//Name: Gho Jeong Wan

#include "Iterator.h"

// circular doubly-linked list iterator

    // initializes curr to currIn
Iterator::Iterator(Node *currIn){
	curr=currIn;
}

    // dereference curr, returns plaintext string
const std::string Iterator::operator*() const{
	return curr->getWord();
}

    // pre-increment operator
Iterator& Iterator::operator++(){
	curr=curr->getNext();
	return *this;
}

    // post-increment operator
Iterator Iterator::operator++(int){
	curr=curr->getNext();
	return Iterator(curr->getPrev());
}

    // for iterator math
    // for example it = it + 2;
Iterator Iterator::operator+(const int& k){
	int i=k;
	Iterator temp(curr);

	if (i>0){
		for(i; i--; i>0){
			temp++;
		}
	}
	else if (i<0){
		for (i; i++; i<0){
			temp--;
		}
	}
	return temp;
}

    // for iterator math
    // for example it += 2;
Iterator Iterator::operator+=(const int& k){
	int i=k;
	if (i>0){
		for(i; i--; i>0){
			curr=curr->getNext();
		}
	}
	else if (i<0){
		for (i; i++; i<0){
			curr=curr->getPrev();
		}
	}
	return *this;
}

    // pre-decrement operator
Iterator& Iterator::operator--(){
	curr=curr->getPrev();
	return *this;
}

    // post-decrement operator
Iterator Iterator::operator--(int){
	curr=curr->getPrev();
	return Iterator(curr->getNext());
}

    // for iterator math
    // for example it = it - 2;
Iterator Iterator::operator-(const int & k){
	int i=k;
	Iterator temp(curr);

	if (i>0){
		for(i; i--; i>0){
			temp--;
		}
	}
	else if (i<0){
		for (i; i++; i<0){
			temp++;
		}
	}
	return temp;
}

    // for iterator math
    // for example it -= 2;
Iterator Iterator::operator-=(const int & k){
	int i=k;
	if (i>0){
		for(i; i--; i>0){
			curr=curr->getPrev();
		}
	}
	else if (i<0){
		for (i; i++; i<0){
			curr=curr->getNext();
		}
	}
	return *this;
}

    // equality operator
    // is true if 2 iterators are pointing to the same node
bool Iterator::operator==(const Iterator &other) {
	Iterator temp=other;
	return ( temp.getCurr()==curr );
}

    // inequality operator
    // is true if 2 iterators are pointing to different nodes
bool Iterator::operator!=(const Iterator &other) {
	Iterator temp=other;
	return ( temp.getCurr()!=curr);
}

    // returns the node to which the iterator points
Node *& Iterator::getCurr() {
	return curr;
}

