//Student Number: 20131674
//Name: Gho Jeong Wan
//Class Number: CSE24102

#include <iostream>
using namespace std;

short getNum(char (*array)[257]);
short getLength(char *array);
short getAvgLength(char (*array)[257]);
void copyChar(char* output, char* array1, char* array2);
void calculateSolution(char* output, char (*array)[257]);



int main()
{
//declaration
	short numTestCases;
	short i=0;
	short j=0;
	short k=0;
	char charInput;
        char array[290][257];
	char output[257];


//initialization
	for(i=0; i<290; i++){
		array[i][0]='\0';
	}
	i=0;
	output[0]='\0';

//input the number of test cases, and file fragments
	cin>>numTestCases;
	while(i<=numTestCases){
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
				i++;
				calculateSolution(output,array);	//get the solution
				printf("%s\n\n",output);		//print out the solution
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

        return 0;
}




short getNum(char (*array)[257]){
	short resut=0;

	while(array[resut][0]!='\0'){
		resut++;
	}
	
	return resut;
}


short getLength(char *array){
	short resut=0;

	while(array[resut]!='\0'){
		resut++;
	}
	
	return resut;
}


short getAvgLength(char (*array)[257]){
	short resut=0;
	short num=getNum(array);

	for (short j=0; j<num; j++){
		resut+=getLength(array[j]);
	}
	resut*=2;
	
	if(num!=0){
		resut/=num;
	}

	return resut;
}


void copyChar(char* output, char* array1, char* array2){
        strcpy(output, array1);
        strcat(output, array2);
}


void calculateSolution(char* output, char (*array)[257]){
	char sample1[257];
	char sample2[257];
	short numOfIdenticalPairs=0;
	short avgLength=getAvgLength(array);
	short num=getNum(array);


//put the sample1 char array at output char array as the solution, if it is possible to concatenate fragments together in pairs to make N copies of the identical output string
	for(short i=0; i<num; i++){
		for(short j=i+1; j<num; j++){
			if( (getLength(array[i])+getLength(array[j]))==avgLength ){
				numOfIdenticalPairs=0;
				copyChar(sample1, array[i], array[j]);
			        for(short k=0; k<num; k++){
			                for(short l=k+1; l<num; l++){
			 			if( (getLength(array[k])+getLength(array[l]))==avgLength ){
       			 	                        copyChar(sample2, array[k], array[l]);
		
							if( strcmp(sample1, sample2)==0 ){
								numOfIdenticalPairs++;
							}
							else{
	                                                	copyChar(sample2, array[l], array[k]);

	                                                	if( strcmp(sample1, sample2)==0 ){
        	                                                	numOfIdenticalPairs++;
                	                                	}
							}
							if( numOfIdenticalPairs>=((short) num/2) ){
								strcpy(output, sample1);
							}
                		        	}
	               		 	}
	       			}
			}
		}
	}
}
