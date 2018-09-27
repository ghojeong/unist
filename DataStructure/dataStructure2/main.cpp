#include "matrix.h"
#include <iostream>

// matrix test
using namespace std;

int main()
{
	ifstream myReadFile;
	myReadFile.open("matrix.txt");
		
	// create matrix from file
	Matrix<int> t(myReadFile);
	myReadFile.close();
	t.Print();
	Matrix<int> p(t);
	cout << "----" << endl;	

	// insert new element
	t = t.CreateElement(5,8,10);
	t.Print();
	cout << "----" << endl;
	
	t.Transpose();

	t.CreateElement(3,10,20);
	(p*t).Print();
		
	return 0;
}
