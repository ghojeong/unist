#ifndef MATRIX_H
#define MATRIX_H

//
// Sparse matrix class using linked lists
//
// CSE221 Fall 2015
//
// Won-Ki Jeong (wkjeong@unist.ac.kr)
//
#include <iostream>
#include <fstream>
#include <math.h>

// Triple type
template <class type>
struct Triple {
	int row, col;
	type value;
};

//
// MatrixNode class
//
template <class type>
class MatrixNode {
	typedef Triple<type> TripleType;
	template<class T> friend class Matrix;
	
public:
	// true : header
	// false : element
	MatrixNode(bool b, TripleType* t)
	{
		head = b;
		right = down = next = this;
		if(!b) 
		{
			triple = *t; // element
		}
	}
	
private:
	MatrixNode *down, *right, *next;
	bool head;
	TripleType triple;
};

//
// Matrix class
//
template <class type>
class Matrix
{
public:
	
	typedef MatrixNode<type> MatrixNodeType;
	typedef Triple<type> TripleType;
	
	// Constructor, create empty sparse matrix with (row, col)
	Matrix(int row, int col);
	
	// Constructor from istream
	Matrix(std::ifstream& is);

	// Copy constructor
	Matrix(const Matrix& a);
	
	// Assignment operator
	Matrix& operator=(const Matrix& a);
	
	// Destructor
	~Matrix();

	// Add Matrix b to *this
	Matrix operator+(const Matrix& b);
	
	// Subtract Matrix b to *this
	Matrix operator-(const Matrix& b);
	
	// Multiply Matrix b to *this
	Matrix operator*(const Matrix& b);
	
	// Swap i-th row with j-th row
	Matrix& SwapRow(int i, int j);
	
	// Swap i-th col with j-th col
	Matrix& SwapCol(int i, int j);
	
	// Create single element in the matrix
	Matrix& CreateElement(int i, int j, type val);
	
	// Transpose matrix
	Matrix& Transpose();
	
	// Print matrix in row-major order
	void Print() {
		MatrixNodeType* curr = headnode->next;
		
		while(curr != headnode)
		{
			MatrixNodeType* row = curr->right;
			while(row != curr)
			{
				std::cout << "Row : " 
						<< (row->triple).row << ", Col: "
						<< (row->triple).col << ", Value: " 
						<< (row->triple).value << std::endl;
				row = row->right;
			}
			curr = curr->next;
		}
	};
	
	
private:
	MatrixNodeType *headnode;
	int nRow, nCol, nElem;
};

#ifndef matrix_TXX
#define matrix_TXX
#include "matrix.txx"
#endif

#endif
