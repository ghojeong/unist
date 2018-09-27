//Assignment #3
//Class Number: CSE24102
//Student Number: 20131674
//Name: Gho Jeong Wan

#define COLUMN 35

#include <iostream>
using namespace std;


void getCharArrayInput(char array[COLUMN]);

int charToNum(char ch);
void swapChar(char &x, char &y);
void swapInt(int &x, int &y);
char getWinner(char array[COLUMN]); //did the black win the poker game? 1: win, 2: lose, 3: tie





int main()
{
	char array[COLUMN];


	
	for(;;){
		getCharArrayInput(array);

		switch ( getWinner(array) ){
			case 'B': cout<<"Black wins."<<endl; break;
			case 'W': cout<<"White wins."<<endl; break;
			case 'S': cout<<"Tie."<<endl; break;
			default: cout<<"There is an error"<<endl; break;
		}
	}
	return 0;
}




void getCharArrayInput(char array[COLUMN]){
	fflush(stdin);
	for (int i=0; i<30; i++){
		array[i]=getchar();
		if(array[i]==EOF || array[0]=='\n'){
			exit(1);
		}
	}
}


int charToNum(char ch){
	switch (ch){
		case 'A': return 14;
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










char getWinner(char array[COLUMN]){
	bool black=true;
	bool white=true;
	char blackChar[5];
	char whiteChar[5];
	int blackInt[5];
	int whiteInt[5];

//variables needed for assigning card values of Four of a Kind, Full House, Three of a Kind, Two Pairs, and Pair
	int blackFH1;
	int blackFH2;
	int whiteFH1;
	int whiteFH2;

//assign values of cards from array
	for(int i=0; i<5; i++){
		blackInt[i]=charToNum( array[3*i] );
		blackChar[i]=array[3*i+1];
		whiteInt[i]=charToNum( array[3*i+15] );
		whiteChar[i]=array[3*i+16];
	}

// arrange Black's cards and White's cards in increasing number order
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

//find the winner if White or Black has Straight Flush
	black=true; white=true;
	for (int i=0; i<4; i++){
		if( blackChar[i]!=blackChar[i+1] ){
			black=false;
		}
		if( blackInt[i]+1!=blackInt[i+1] ){
			black=false;
		}
		if( whiteChar[i]!=whiteChar[i+1] ){
			white=false;
		}
		if( whiteInt[i]+1!=whiteInt[i+1] ){
			white=false;
		}
	}
	if ( black ){
		if(white){
			for (int i=0; i<5; i++){
				if (blackInt[4-i]>whiteInt[4-i]){
					return 'B';
				}
				else if (blackInt[4-i]<whiteInt[4-i]){
					return 'W';
				}
			}
			return 'S';	
		}
		else{
			return 'B';
		}
	}
	else if( white ){
		return 'W';
	}



//find the winner if White or Black has Four of a Kind
	black=false; white=false;
	if ( blackInt[0]==blackInt[3] ){
		black=true;
		blackFH1=blackInt[0];
	}

	if ( blackInt[4]==blackInt[1] ){
		black=true;
		blackFH1=blackInt[4];
	}

	if ( whiteInt[0]==whiteInt[3] ){
		white=true;
		whiteFH1=whiteInt[0];
	}

	if ( whiteInt[4]==whiteInt[1] ){
		white=true;
		whiteFH1=whiteInt[4];
	}

	if ( black ){
		if(white){
			if (blackFH1>whiteFH1){
				return 'B';
			}
			else if (blackFH1<whiteFH1){
				return 'W';
			}
			else if (blackFH1==whiteFH1){
				for (int i=0; i<5; i++){
					if (blackInt[4-i]>whiteInt[4-i]){
						return 'B';
					}
					else if (blackInt[4-i]<whiteInt[4-i]){
						return 'W';
					}
				}
				return 'S';	
			}
		}
		else{
			return 'B';
		}
	}
	else if( white ){
		return 'W';
	}

//find the winner if White or Black has Full House
	black=false; white=false;
	for (int i=0; i<3; i++){
		if( blackInt[i]==blackInt[i+1] ){
			if( blackInt[i+1]==blackInt[i+2] ){
				blackFH1=blackInt[i];
				if( (7-3*i)==7 ){
					if( blackInt[3]==blackInt[4] ){
						blackFH2=blackInt[3];
						black=true;
					}
				}
				else if ( (7-3*i)==4 ){
					if( blackInt[0]==blackInt[4] ){
						blackFH2=blackInt[4];
						black=true;
					}
				}
				else if ( (7-3*i)==1 ){
					if( blackInt[0]==blackInt[1] ){
						blackFH2=blackInt[0];
						black=true;
					}
				}
			}
		}
		if( whiteInt[i]==whiteInt[i+1] ){
			if( whiteInt[i+1]==whiteInt[i+2] ){
				whiteFH1=whiteInt[i];
				if( (7-3*i)==7 ){
					if( whiteInt[3]==whiteInt[4] ){
						whiteFH2=whiteInt[3];
						white=true;
					}
				}
				else if ( (7-3*i)==4 ){
					if( whiteInt[0]==whiteInt[4] ){
						whiteFH2=whiteInt[4];
						white=true;
					}
				}
				else if ( (7-3*i)==1 ){
					if( whiteInt[0]==whiteInt[1] ){
						whiteFH2=whiteInt[0];
						white=true;
					}
				}
			}
		}
	}
	if ( black ){
		if(white){
			if (blackFH1>whiteFH1){
				return 'B';
			}
			else if (blackFH1<whiteFH1){
				return 'W';
			}
			else if (blackFH1==whiteFH1){
				if(blackFH2>whiteFH2){
					return 'B';
				}
				if(blackFH2<whiteFH2){
					return 'W';
				}
				if(blackFH2==whiteFH2){
					return 'S';
				}
			}
				
		}
		else{
			return 'B';
		}
	}
	else if( white ){
		return 'W';
	}



//find the winner if White or Black has Flush
	black=true; white=true;
	for (int i=0; i<4; i++){
		if( blackChar[i]!=blackChar[i+1] ){
			black=false;
		}
		if( whiteChar[i]!=whiteChar[i+1] ){
			white=false;
		}
	}
	if ( black ){
		if(white){
			for (int i=0; i<5; i++){
				if (blackInt[4-i]>whiteInt[4-i]){
					return 'B';
				}
				else if (blackInt[4-i]<whiteInt[4-i]){
					return 'W';
				}
			}
			return 'S';	
		}
		else{
			return 'B';
		}
	}
	else if( white ){
		return 'W';
	}


//find the winner if White or Black has Straight
	black=true; white=true;
	for (int i=0; i<4; i++){
		if( blackInt[i]+1!=blackInt[i+1] ){
			black=false;
		}
		if( whiteInt[i]+1!=whiteInt[i+1] ){
			white=false;
		}
	}
	if ( black ){
		if(white){
			for (int i=0; i<5; i++){
				if (blackInt[4-i]>whiteInt[4-i]){
					return 'B';
				}
				else if (blackInt[4-i]<whiteInt[4-i]){
					return 'W';
				}
			}
			return 'S';	
		}
		else{
			return 'B';
		}
	}
	else if( white ){
		return 'W';
	}



//find the winner if White or Black has Three of a Kind
	black=false; white=false;
	for (int i=0; i<3; i++){
		if( blackInt[i]==blackInt[i+1] ){
			if( blackInt[i+1]==blackInt[i+2] ){
				blackFH1=blackInt[i];
				black=true;
			}
		}
		if( whiteInt[i]==whiteInt[i+1] ){
			if( whiteInt[i+1]==whiteInt[i+2] ){
				whiteFH1=whiteInt[i];
				white=true;
			}
		}
	}
	if ( black ){
		if(white){
			if (blackFH1>whiteFH1){
				return 'B';
			}
			else if (blackFH1<whiteFH1){
				return 'W';
			}
			else if (blackFH1==whiteFH1){
				for (int i=0; i<5; i++){
					if (blackInt[4-i]>whiteInt[4-i]){
						return 'B';
					}
					else if (blackInt[4-i]<whiteInt[4-i]){
						return 'W';
					}
				}
				return 'S';	
			}				
		}
		else{
			return 'B';
		}
	}
	else if( white ){
		return 'W';
	}


//find the winner if White or Black has Two Pairs
	black=false; white=false;
	for (int i=0; i<4; i++){
		if( blackInt[i]==blackInt[i+1] ){
			for (int j=1+i; j<4; j++){
				if( blackInt[j]==blackInt[j+1] ){
					blackFH2=blackInt[j];
					black=true;
				}
			}
		}
		if( whiteInt[i]==whiteInt[i+1] ){
			for (int j=1+i; j<4; j++){
				if( whiteInt[j]==whiteInt[j+1] ){
					whiteFH2=whiteInt[j];
					white=true;
				}
			}
		}
	}
	if ( black ){
		if(white){
			if (blackFH1>whiteFH1){
				return 'B';
			}
			else if (blackFH1<whiteFH1){
				return 'W';
			}
			else if (blackFH1==whiteFH1){
				if (blackFH2>whiteFH2){
					return 'B';
				}
				else if (blackFH2<whiteFH2){
					return 'W';
				}
				else if (blackFH2==whiteFH2){
					for (int i=0; i<5; i++){
						if (blackInt[4-i]>whiteInt[4-i]){
							return 'B';
						}
						else if (blackInt[4-i]<whiteInt[4-i]){
							return 'W';
						}
					}
					return 'S';
				}	
			}				
		}
		else{
			return 'B';
		}
	}
	else if( white ){
		return 'W';
	}


//find the winner if White or Black has Pair
	black=false; white=false;
	for (int i=0; i<4; i++){
		if( blackInt[i]==blackInt[i+1] ){
			blackFH1=blackInt[i];
			black=true;
		}
		if( whiteInt[i]==whiteInt[i+1] ){
			whiteFH1=whiteInt[i];
			white=true;
		}
	}
	if ( black ){
		if(white){
			if (blackFH1>whiteFH1){
				return 'B';
			}
			else if (blackFH1<whiteFH1){
				return 'W';
			}
			else if (blackFH1==whiteFH1){
				for (int i=0; i<5; i++){
					if (blackInt[4-i]>whiteInt[4-i]){
						return 'B';
					}
					else if (blackInt[4-i]<whiteInt[4-i]){
						return 'W';
					}
				}
				return 'S';	
			}				
		}
		else{
			return 'B';
		}
	}
	else if( white ){
		return 'W';
	}



//find the winner if White and Black are both High Card
	for (int i=0; i<5; i++){
		if (blackInt[4-i]>whiteInt[4-i]){
			return 'B';
		}
		else if (blackInt[4-i]<whiteInt[4-i]){
			return 'W';
		}
	}
	return 'S';	
}
