//Assignment #3
//Class Number: CSE24102
//Student Number: 20131674
//Name: Gho Jeong Wan

#include <iostream>
using namespace std;

class IntArray{
	private:
		int nelements;
		int *elements;
		void copy(int* temp, int* element, int nelement){
			if (nelement>0){
				for(int i=0; i<nelement; i++){
					temp[i]=element[i];
				}
			}
		}
	public:
		IntArray(){
			nelements=0;
			elements=new int[nelements];
		}

		IntArray(const IntArray &that){
			if(this!=&that){
				nelements=that.nelements;
				elements=new int [nelements];
				for (int i=0; i<nelements; i++){
					elements[i]=that.elements[i];
				}
			}
		}

		int pop(){
			int* temp;
			int ntemp;
			if(nelements>1){
				ntemp=nelements-1;
				temp=new int[ntemp];
				copy(temp, elements+1, ntemp);
				delete [] elements;
				nelements=ntemp;
				elements=new int[nelements];
				copy(elements, temp, ntemp);
				delete [] temp;
			}
			else if (nelements==1){
				nelements=0;
				delete [] elements;
				elements=new int[0];
			}
			return 0;
		}

		bool empty() const{
			if(nelements<1){
				return true;
			}
			return false;
		}

		int top() const{
			return elements[0];
		}

		void push(int i){
			int* temp;
			int ntemp=nelements;

			temp=new int[ntemp];
			copy(temp, elements, nelements);

			delete [] elements;
			nelements++;
			elements=new int [nelements];

			copy(elements+1, temp, ntemp);
			elements[0]=i;

			delete [] temp;
		}

		int& operator[](int sub){
			if(sub<0 || sub>=nelements){
				printf("Error: invalid memory access");
				exit(1);
			}
			return elements[sub];
		}

		void appendCopyAtEnd(const IntArray& that){
			int* temp;
			int ntemp=nelements;

			temp=new int[ntemp];
			copy(temp, elements, nelements);

			delete [] elements;
			nelements+=that.nelements;
			elements=new int [nelements];

			copy(elements, temp, ntemp);
			copy(elements+ntemp, that.elements, that.nelements);

			delete [] temp;
		}



		friend ostream& operator<<(ostream& os, const IntArray& arg){
			for (int i=0;i<arg.nelements;i++) {
				os << arg.elements[i] << ' ';
			}
			return os;
		}

		IntArray& operator=(const IntArray& that){
			if(this==&that){
				return *this;
			}
			if (nelements >0){
				delete [] elements;
				nelements=0;
			}
			nelements=that.nelements;
			elements=new int [nelements];
			for (int i=0; i<nelements; i++){
				elements[i]=that.elements[i];
			}
			return *this;
		}

		~IntArray(){
			delete [] elements;
		}
};
