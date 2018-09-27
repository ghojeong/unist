#include <iostream>
#include <iomanip>
#include <string.h>
#include <stdio.h>
using namespace std;

#define MAXDIGITS 10
class ReversibleInteger {
	private:
		char digits[MAXDIGITS]; /* represent the number */
		int lastdigit; /* index of high-order digit */
	public:
		ReversibleInteger()
		{cin.getline(digits,MAXDIGITS)}
		ReversibleInteger(unsigned n=0);
		ReversibleInteger reverse();
		ReversibleInteger add(ReversibleInteger arg);
		unsigned getValue(); // returns the integer value of this object
		bool isPalindrome(); // returns true if this integer is a palidrome
};





int main()
{
int ncases;
cin >> ncases;
int final[ncases];//int array for final output

for(int cases=ncases;cases>0; cases--)
	{
	ReversibleInteger object;
	}
}


ReversibleInteger ReversibleInteger::reverse(){
ReversibleInteger t;

for(int i =0 ;i<10;i++)
t.digits[i] = digits[10-i];

return t;
}
