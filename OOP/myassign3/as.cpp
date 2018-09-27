//Assignment #2
//Class Number: CSE24102
//Student Number: 20131674
//Name: Gho Jeong Wan


#define ROW 300
#define COLUMN 35

#include <iostream>
using namespace std;


void get2DCharArrayInput(char array[ROW][COLUMN]);

int charToNum(char ch);
void swapChar(char &x, char &y);
void swapInt(int &x, int &y);
int isBlackWin(char array[COLUMN]); //did the black win the poker game? 1: win, 2: lose, 3: tie





int main()
{
	char array[ROW][COLUMN];

	get2DCharArrayInput(array);
	
	for(int r=0; r<ROW; r++){
		if (array[r][0] == '\0'){
			break;
		}

		isBlackWin(array[r]);
		cout<<endl;
	}


	return 0;
}




void get2DCharArrayInput(char array[ROW][COLUMN]){
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
			if( charInput=='\n' ){
				j=0;
				return;
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
}


int charToNum(char ch){
	switch (ch){
		case 'A': return 1;
		case '2': return 2;
		case '3': return 3;
		case '4': return 4;
		case '5': return 5;
		case '6': return 6;
		case '7': return 7;
		case '8': return 8;
		case '9': return 9;
		case 'T': return 10;
		case 'J': return 11;
		case 'Q': return 12;
		case 'K': return 13;
	}
	return 0;
}


void swapChar(char &x, char &y){
	char temp;
	temp=x;
	x=y;
	y=temp;
}


void swapInt(int &x, int &y){
	int temp;
	temp=x;
	x=y;
	y=temp;
}


int isBlackWin(char array[COLUMN]){
	char blackChar[5];
	char whiteChar[5];
	int blackInt[5];
	int whiteInt[5];

	for(int i=0; i<5; i++){
		blackInt[i]=charToNum( array[3*i] );
		blackChar[i]=array[3*i+1];
		whiteInt[i]=charToNum( array[3*i+15] );
		whiteChar[i]=array[3*i+16];
	}

	for (int j=0; j<4; j++){
		for (int i=0; i<4-j; i++){
			if( blackInt[i] > blackInt[i+1] ){
				swapInt( blackInt[i], blackInt[i+1] );
				swapChar( blackChar[i], blackChar[i+1] );
			}
			if( whiteInt[i] > whiteInt[i+1] ){
				swapInt( whiteInt[i], whiteInt[i+1] );
				swapChar( whiteChar[i], whiteChar[i+1] );
			}
		}
	}

	for (int i=0; i<5; i++){
		cout<< blackInt[i]<<blackChar[i]<<"    ";
	}
	for (int i=0; i<5; i++){
		cout<<whiteInt[i]<<whiteChar[i]<<" ";
	}
}
