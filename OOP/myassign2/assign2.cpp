//Assignment #2
//Class Number: CSE24102
//Student Number: 20131674
//Name: Gho Jeong Wan


#define ROW 110
#define COLUMN 90

#include <iostream>
using namespace std;


bool get2DCharArrayInput(char array[ROW][COLUMN]);
void print2Darray(char array[ROW][COLUMN]);

bool decryptArray(char array[ROW][COLUMN]);




int main()
{
	bool isDecrypted;
	int numCases;
	char array[ROW][COLUMN];

	cin>>numCases;

	for (;numCases>=0; numCases--){
		if( !get2DCharArrayInput(array) ){
			continue;
		}
		isDecrypted=decryptArray(array);

		if(isDecrypted){
			print2Darray(array);
			if(numCases!=0){
				cout<<endl;
			}
		}
//If decryption is impossible, output "No solution."
		else{
			cout<<"No solution."<<endl;
		}
	}
}




bool get2DCharArrayInput(char array[ROW][COLUMN]){
	char charInput;
	int j=0;
	int k=0;

        for(int r=0; r<ROW; r++){
		for (int c=0; c<COLUMN; c++){
	                array[r][c]='\0';
		}
        }

	for(;;){
		fflush(stdin);
		charInput=getchar();
		if( charInput=='\n' ){
			array[j][k]='\0';
			j++;
			k=0;

			fflush(stdin);
			charInput=getchar();
			if( charInput=='\n' || charInput==EOF ){
				j=0;
				if( array[0][0] == '\0' ){
					return false;
				}
				else{
					return true;
				}
			}
			else{
				array[j][k]=charInput;
				k++;
			}
		}
		else{
			array[j][k]=charInput;
			k++;
		}
	}
	return false;
}


void print2Darray(char array[ROW][COLUMN]){
        for(int r=0; r<ROW; r++){
		if (array[r][0] == '\0'){
			return;
		}

		for (int c=0; c<COLUMN; c++){
	                cout<<array[r][c];
		}
		cout << endl;
        }	
}




bool decryptArray(char array[ROW][COLUMN]){
	bool isPlaintext;
	char plaintext[44]="the quick brown fox jumps over the lazy dog";
	char encryptedPlaintext[44];


// get the line of the encrpyted plaintext "the quick brown fox jumps over the lazy dog" in array
        for(int r=0; r<ROW; r++){
		isPlaintext=true;

		for (int c=0; c<43; c++){
			if( array[r][c] == '\0' ){
				isPlaintext=false;
				break;
			}
		}

		if (array[r][3] != ' '){
			isPlaintext=false;
		}
		if (array[r][9] != ' '){
			isPlaintext=false;
		}
		if (array[r][15] != ' '){
			isPlaintext=false;
		}
		if (array[r][19] != ' '){
			isPlaintext=false;
		}
		if (array[r][25] != ' '){
			isPlaintext=false;
		}
		if (array[r][30] != ' '){
			isPlaintext=false;
		}
		if (array[r][34] != ' '){
			isPlaintext=false;
		}
		if (array[r][39] != ' '){
			isPlaintext=false;
		}

		if (array[r][43] != '\0'){
			isPlaintext=false;
		}

		if(isPlaintext){
			for (int i=0; i<44; i++){
				encryptedPlaintext[i]=array[r][i];
			}
			break;
		}
        }



//decrypt the sentences by using the plaintext
        for(int r=0; r<ROW; r++){
		if( array[r][0]=='\0'){
			break;
		}
		for (int c=0; c<COLUMN; c++){
			if( array[r][c]=='\0'){
				break;
			}
			for (int i=0; i<44; i++){
//If decryption is impossible, this program should output "No solution"
				if(i==43){
					return false;
				}
				if(array[r][c]==encryptedPlaintext[i]){
					array[r][c]=plaintext[i];
					break;
				}
			}
		}
        }
	return true;
}
