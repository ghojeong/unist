#ifndef HASHMAP_H
#define HASHMAP_H

#include <iostream>

/** 
  * Assignment 6 for CSE221 Data Structures
  *
  * 2015. 11. 17
  *20131674, JeongWan Gho
  */

// Map element
template <class KeyType, class ValType>
class MapElem
{
public:
	typedef KeyType ktype;
	typedef ValType vtype;
	
	KeyType key;
	ValType val;
	
	MapElem<KeyType, ValType>* link;
};

bool inline operator==(std::string a, std::string b)
{
	if((a).compare(b) == 0) return true;
	return false;
}

//
// Hash Map data structure
//
template <class HashMapElemType> 
class HashMap
{
public:
	typedef typename HashMapElemType::ktype KeyType;
	typedef typename HashMapElemType::vtype ValType;
	
	// constructor
	HashMap(unsigned int c = 1000);
	
	// destructor
	~HashMap();
	
	// Modify below functions
	int size() { return mapsize; };
	
	bool isEmpty() { return (mapsize == 0); };
	
	// ToDo
	HashMapElemType* find(const KeyType k);
	
	void insert(const KeyType k, const ValType v);
		
	bool remove(const KeyType k);
	
	unsigned int hashfunction(const KeyType k);
	
	void print();
	
private:
	// Hash Table
	HashMapElemType** ht;
	
	unsigned int mapsize, capacity, divisor;
};



//
// - Implementation -
//

// constructor
template <class HashMapElemType>
HashMap<HashMapElemType>::HashMap(unsigned int c) 
{
	// ToDo

	capacity=c;
	mapsize=0;
	divisor=1000003; //large prime number
	ht=(HashMapElemType**) calloc(divisor,sizeof(HashMapElemType*));
	for(int i=0; i<divisor; i++){
		ht[i]=(HashMapElemType*) malloc(sizeof(HashMapElemType));
		ht[i]->val=0;
		ht[i]->link=NULL;
	}//construct static hash table
}

// destructor
template <class HashMapElemType>
HashMap<HashMapElemType>::~HashMap() 
{
	// ToDo
	MapElem<std::string, unsigned int>* temp;
	MapElem<std::string, unsigned int>* temp2;
	for(unsigned int i=0; i<divisor; i++){
		temp=(MapElem<std::string, unsigned int>*) ht[i];
		temp=temp->link; //first node of the chain for i-th bucket
		while(temp!=NULL){
			temp2=temp->link;
			delete temp;
			mapsize--;
			temp=temp2;
		}
	}
	delete [] ht;
	mapsize=0;
	capacity=0;
}

template <class HashMapElemType>
HashMapElemType* 
HashMap<HashMapElemType>::find(const KeyType k) 
{ 
	// ToDo
	unsigned int val=hashfunction(k);
	MapElem<std::string, unsigned int>* temp=(MapElem<std::string, unsigned int>*) ht[val];
	temp=temp->link;//first node of the chain for val-th bucket
	std::string sk, st;
	sk=(std::string) k;
	HashMapElemType* retun;

	if(val>0){
		while(temp!=NULL){
			st=(std::string) temp->key;
			if(sk==st){
			//if k is in the map, return the pair (k,v) as MapElem
				retun=(HashMapElemType*) temp;
				return retun;
			}
			temp=temp->link;
		}		
	}
	return NULL;	//If find is not successful, return NULL
}

template <class HashMapElemType>
void 
HashMap<HashMapElemType>::insert(const KeyType k, const ValType v) 
{
	// ToDo

	MapElem<std::string, unsigned int>* temp=(MapElem<std::string, unsigned int>*) find(k);//check wheter k is already in the map
	if(temp!=NULL){//if k is laready in the map, change its value with v
		temp->val=v;
	}
	else{//insert a pair (k,v) in chaining form. the sorted chain in the map
		MapElem<std::string, unsigned int>* elem=new MapElem<std::string, unsigned int>;
		mapsize++;
		elem->key=k;
		elem->val=v;

		unsigned int val=(unsigned int) v;
		MapElem<std::string, unsigned int>* hNode=(MapElem<std::string, unsigned int>*) ht[val]; //header node of the chain for val-th bucket
	
		elem->link=hNode->link;
		hNode->link=elem;
	}
}

template <class HashMapElemType>
bool 
HashMap<HashMapElemType>::remove(const KeyType k) 
{
	 // ToDo

	MapElem<std::string, unsigned int>* temp=(MapElem<std::string, unsigned int>*) find(k);//find the node to delete
	if(temp!=NULL){//if k is in the map, remove its pair(k,v) and return true
		MapElem<std::string, unsigned int>* temp2=temp->link;
		if(temp2!=NULL){
			temp->key=temp2->key;
			temp->val=temp2->val;
			temp->link=temp2->link;
			delete temp2;
			mapsize--;
		}
		else{
			delete temp;
			mapsize--;
			temp=NULL;
		}		
		return true;
	}
	else{//If k is not in the map, return false
		return false;
	}
}

template <class HashMapElemType>
unsigned int 
HashMap<HashMapElemType>::hashfunction(const KeyType k)
{
	// ToDo
	//returns an integer hash value converted from a key k

	unsigned int val=0;
	std::string key=(std::string) k;
	val+=key.length();
	unsigned int multiplier=100;
	unsigned int temp;

	for(int i=0; i<key.length(); i++){		
		temp=(unsigned int) key[i];
		val+=temp*multiplier;
		multiplier*=2;
	}
	//algorithm discussed in the class is +=key<<8
	//however unsigned int memory size can't store this if the word is longer than 10 letters
	//so I changed the algorithm

	val=val%divisor;
	//division hash function

	if(val>0 && val<divisor) return val;
	return 0;
}

template <class HashMapElemType>
void 
HashMap<HashMapElemType>::print()
{
	// ToDo

	MapElem<std::string, unsigned int>* temp;
	std::string key;
	unsigned int val;

	//print all key:value pair in decreasing order of value
	for(unsigned int i=divisor-1; i>=0; i--){
		temp=(MapElem<std::string, unsigned int>*) ht[i]; 
		temp=temp->link; //first node of the chain for i-th bucket
		while(temp!=NULL){
			key=(std::string) temp->key;
			val=(unsigned int) temp->val;
			printf("%s:%u\n",key.c_str(),val);
			temp=temp->link;
		}
	}
}

#endif

