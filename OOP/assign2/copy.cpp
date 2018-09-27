#include <iostream>
#include <iomanip>
#include <string.h>
#include <stdio.h>
using namespace std;

void change(int &a, int &b)
{
	int temp;
	temp=a;
	a=b;
	b=temp;
}

void change(char &a, char &b)
{
	char temp;
	temp=a;
	a=b;
	b=temp;
}

int main()
{
int ncases;
cin >> ncases;
cout<<endl;

for (int count=ncases; count>0; count--)
        {
	
//input
	int intInput[100][3]={};
	char charInput[100]={};
	int count=0;//count the number of inputs

	for(int r=0; r<100; r++)// r is row of 2 dimensional array " input"
		{
		cin>>intInput[r][0];
                cin>>intInput[r][1];    
                cin>>intInput[r][2];    
		cin>>charInput[r];
		}

                for(int r=0; r<100; r++)
		{
		cout<<intInput[r][0]<<" " <<intInput[r][1]<<" "<<intInput[r][2]<<" "<<charInput[r]<<endl;
		}
		

//count the number of inputs
	        for(int r=0; r<100; r++)// r is row of 2 dimensional array " input"
                {
                        if (intInput[r][0]!=0)
                                {
				count+=1;
                                }

                }

	count=count;
	cout << count<<endl;


        int increInt[count][3];//incrementing int input
        char increChar[count];//incrementing char input

        for(int r=0; r<count; r++)//input Input into incre
                {
                increInt[r][0]=intInput[r][0];
                increInt[r][1]=intInput[r][1];
                increInt[r][2]=intInput[r][2];
                increChar[r]=charInput[r];
                }


                for(int r=0; r<count; r++)
                {
                cout<<increInt[r][0]<<" " <<increInt[r][1]<<" "<<increInt[r][2]<<" "<<increChar[r]<<endl;
                }

	cout<<endl;


	int i,ii,tempI;
	char tempC;
	for(i=0; i<count-1; i++)
		{
		for (ii=0; (ii+i)<count-1; ii++)
			{
				if(increInt[ii][0]>increInt[ii+1][0])
				{
				change(increInt[ii][0],increInt[ii+1][0]);
                                change(increInt[ii][1],increInt[ii+1][1]);
                                change(increInt[ii][2],increInt[ii+1][2]);
				change(increChar[ii],increChar[ii+1]);
				}	
			
			}

		}

	        for(int r=0; r<count; r++)
                {
                cout<<increInt[r][0]<<" " <<increInt[r][1]<<" "<<increInt[r][2]<<" "<<increChar[r]<<endl;
                }
		cout<<endl;


	
        for(i=0; i<count-1; i++)
                {
   	        int cInt=increInt[0][0];//set the count int of the matrix
                for (ii=0; ii<count-1; ii++)
                        {
			if(cInt==increInt[ii+1][0])
				{
                                if(increInt[ii][1]>increInt[ii+1][1])
                                {
                                change(increInt[ii][0],increInt[ii+1][0]);
                                change(increInt[ii][1],increInt[ii+1][1]);
                                change(increInt[ii][2],increInt[ii+1][2]);
                                change(increChar[ii],increChar[ii+1]);
                                }
				}
			else
				{
                                cInt=increInt[ii][0];
				continue;
				}

                        }

                }


	 for(int r=0; r<count; r++)
                {
                cout<<increInt[r][0]<<" " <<increInt[r][1]<<" "<<increInt[r][2]<<" "<<increChar[r]<<endl;
                }
                cout<<endl;


        int output[10][3]={};//arrays for final output (problems solved, penalty time,check for print)
        int cf=increInt[0][0];// set the count int for the row of first column
        int cs=increInt[0][1];// set the count int for the row of second column

        for(int r=0; r<count; r++)
                {
		if(cf==increInt[r][0])
			{
                        if(cs==increInt[r][1])
                                {
                                if(increChar[r]=='C')
                                        {
                                        output[cf][0]+=1;
                                        output[cf][1]+=increInt[r][2];
                                        cs+=1;
                                        }
                                }
                        else
                        {
			cs=increInt[r][1];
                                if(increChar[r]=='C')
                                        {
                                        output[cf][0]+=1;
                                        output[cf][1]+=increInt[r][2];
                                        cs+=1;
                                        }
			}
                        }
                else
                {
		cf=increInt[r][0];
		cs=increInt[0][1];
                       {
			if(cs==increInt[r][1])
                                {
                                if(increChar[r]=='C')
                                        {
                                        output[cf][0]+=1;
                                        output[cf][1]+=increInt[r][2];
                                        cs+=1;
                                        }
                                }
                        else
                        {
			cs=increInt[r][1];
			if(increChar[r]=='C')
                                        {
                                        output[cf][0]+=1;
                                        output[cf][1]+=increInt[r][2];
                                        cs+=1;
                                        }
			}
			}
		}
                }





        for (int r=0; r<10; r++)
                {
                cout<<output[r][0]<<" "<<output[r][1]<<endl;
                }


         cf=increInt[0][0];// set the count int for the row of first column
         cs=increInt[0][1];// set the count int for the row of second column


        for(int r=0; r<count; r++)
                {
                if(cf==increInt[r][0])
                        {
      		        if (increChar[r]=='I')
   			             {
					output[cf][1]+=20;}
                                        output[cf][2]++;
                        }
                else
                {
		cf=increInt[r][0];
                        if (increChar[r]=='I')
                                     {
                                        output[cf][1]+=20;}
                                        output[cf][2]++;
		}
                }
		
	cout << endl;

	for (int r=0; r<10; r++)
		{
		cout<<output[r][0]<<" "<<output[r][1]<<endl;
		}

	cout<< endl;


	int newcount=0;//count for actual row of output[][]

        for (int r=0; r<10; r++)
                {
                if(output[r][2]!=0)
                        {
			newcount++;
                        }
                }


	int fOut[newcount][3];//array for final output
	int ir=0;//int for putting values for fOut

	for (int r=0; r<10; r++)
                {		
                if(output[r][2]!=0)
			{
				fOut[ir][0]=r;
				fOut[ir][1]=output[r][0];
				fOut[ir][2]=output[r][1];
				ir++;
			}
                }


	cout<<endl;


	for (int r=0; r<newcount; r++)
		{
		cout<<fOut[r][0]<<" "<<fOut[r][1]<<" "<<fOut[r][2]<<endl;
		}


        for(i=0; i<newcount-1; i++)
                {
                for (ii=0; (ii+i)<newcount-1; ii++)
                        {
                                if(fOut[ii][1]<fOut[ii+1][1])
                                {
                                change(fOut[ii][0],fOut[ii+1][0]);
                                change(fOut[ii][1],fOut[ii+1][1]);
                                change(fOut[ii][2],fOut[ii+1][2]);
                                }

                        }
		}


	cout<<endl;
        for (int r=0; r<newcount; r++)
                {
                cout<<fOut[r][0]<<" "<<fOut[r][1]<<" "<<fOut[r][2]<<endl;
                }


        for(i=0; i<newcount-1; i++)
                {
		int cInt=fOut[0][0];
                for (ii=0; ii<newcount-1; ii++)
                        {if(cInt==fOut[ii+1][0])
				{
                                if(fOut[ii][2]>fOut[ii+1][2])
                                {
                                change(fOut[ii][0],fOut[ii+1][0]);
                                change(fOut[ii][1],fOut[ii+1][1]);
                                change(fOut[ii][2],fOut[ii+1][2]);
                                }
				}
             		else
			{
                        cInt=fOut[ii][0];
			continue;
			}
                        }
                }

        cout<<endl;
        for (int r=0; r<newcount; r++)
                {
                cout<<fOut[r][0]<<" "<<fOut[r][1]<<" "<<fOut[r][2]<<endl;
                }


	

	cout << endl;

	}
}
