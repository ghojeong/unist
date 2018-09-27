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
		char odigits[MAXDIGITS];//original digits
	public:
		ReversibleInteger(unsigned n=0);
		ReversibleInteger reverse();
		ReversibleInteger add(ReversibleInteger arg);
		unsigned getValue(); // returns the integer value of this object
		bool isPalindrome(); // returns true if this integer is a palidrome
};

int check=0;//method for preventing multipe getline


void change(char &a, char &b)// swap a & b
{
        char temp;
        temp=a;
        a=b;
        b=temp;
}

ReversibleInteger :: ReversibleInteger(unsigned n)//constructor that input values in digits
{
for(int a=0;a<10;a++)
{digits[a]=NULL;}
if(check==0)
{cin.getline(digits,MAXDIGITS);
check++;}}

ReversibleInteger ReversibleInteger :: reverse()//reverse the digits in object
{
	ReversibleInteger t;
        for (int a=0;a<10;a++)
                {t.digits[a]=digits[a];}
	for (int a=0;a<10;a++)
		{t.odigits[a]=digits[a];}
	int length=0;
		for (int a=0;a<10;a++)
		{if(t.digits[a]>='0')
			{length++;}}
	for (int a=0; a<(length)/2;a++)
		{change(t.digits[a],t.digits[length-1-a]);}
	return t;
}

ReversibleInteger ReversibleInteger :: add(ReversibleInteger arg)//add the digits of arg and given object
{
	ReversibleInteger t;
        for (int a=0;a<10;a++)
                {t.digits[a]=digits[a];}
        for (int a=0;a<10;a++)
                {t.odigits[a]=digits[a];}
        int length=0;
                for (int a=0;a<10;a++)
                {if(digits[a]>='0')
                        {length++;}}

	int sum=0;//variable that store the added digits

		int ten1=1;
		for (int k=length-1;k>=0;k--)
			{
			int arbitrary=(int)digits[k]-'0';
			arbitrary*=ten1;
			sum+=arbitrary;
			ten1*=10;
			}


		int ten2=1;
                for (int k=length-1;k>=0;k--)
                        {
                        int arbitrary=(int)arg.digits[k]-'0';
                        arbitrary*=ten2;
                        sum+=arbitrary;
			ten2*=10;
                        }
	int arbSum=sum;

	int lengthSum=0;
		for (int a=0;a<10;a++)
			{
			if(sum>0)
			{lengthSum++;}
			sum=sum/10;
			}

	int ten=1;
		for(int a=lengthSum-1;a>0;a--)
			{
			ten*=10;
			}

	sum=arbSum;
	
	for(int a=0;a<lengthSum;a++)//input the sum numbers in t.digits
		{
		int arb=sum/ten;

		t.digits[a]=(char)arb+'0';
		sum-=arb*ten;
		ten=ten/10;

		}


	return t;
}

unsigned ReversibleInteger :: getValue()//change cstring of digits in unsinged value
{
        unsigned length=0;
                for (int a=0;a<10;a++)
                {if(digits[a]>='0')
                        {length++;}}
	unsigned ret=0;//return

                unsigned ten1=1;
                for (int k=length-1;k>=0;k--)
                        {
                        unsigned arbitrary=(unsigned)digits[k]-'0';
                        arbitrary*=ten1;
                        ret+=arbitrary;
                        ten1*=10;
                        }


	return ret;
}

bool ReversibleInteger :: isPalindrome()//check wheter given digits are palindrome or not
{
	bool ret2=true;//return
        int length2=0;
                for (int a=0;a<10;a++)
                {if(digits[a]>='0')
                        {length2++;}}
        for (int a=0; a<(length2)/2;a++)
                {
		if(digits[a]==digits[length2-1-a])
			{ret2=true;}
		else
			{ret2=false;
			break;}
		}
	return ret2;
}






int main()
{
int ncases;
cin >> ncases;
unsigned final[ncases];// array for final second  output in a line
unsigned finaln[ncases];//array for final first output in a line
char garbi[10];
cin.getline(garbi,10);

for(int cases=0;cases<ncases; cases++)
	{
	check=0;
	ReversibleInteger object(0);
	finaln[cases]=0;

	for (int a=0; a<1000; a++)
		{
		if(object.isPalindrome())
			{
			final[cases]=object.getValue();
			break;
			}
		else
			{
			object=object.add(object.reverse());
			finaln[cases]++;
			}

		}

	}

for(int cases=0;cases<ncases;cases++)
	{
	cout<<finaln[cases]<<" "<<final[cases]<<endl;
	}
}
