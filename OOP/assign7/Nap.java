//Assignment #7
//Class Number: CSE24102
//Student Number: 20131674
//Name: Gho Jeong Wan

import java.util.Scanner;

public class Nap
{
	public static void main (String[] args)
	{
		int[] start;
		int[] end;
		int temp;
		int prevEnd=60*10;
		int tempStart=0;
		int tempEnd=0;
		int napStart=60*10;
		int napM=0;
		int d=1;
		int numAppointments=0;
		String str="";
		String napStartStr="";
		String napStr="";
		Scanner scan = new Scanner(System.in);

		while (scan.hasNextInt()){
			prevEnd=60*10;
			napM=0;
			str=scan.nextLine();
			if(str.equals(""))
				break;
			numAppointments= Integer.parseInt(str);
			start=new int[numAppointments];
			end=new int[numAppointments];

			for (int i=0; i<numAppointments; i++){
				str=scan.nextLine();
				if(str.equals(""))
					break;
				
				start[i]=60*( 10*charToInt(str.charAt(0))+charToInt(str.charAt(1)) )+10*charToInt(str.charAt(3))+charToInt(str.charAt(4));
				end[i]=60*( 10*charToInt(str.charAt(6))+charToInt(str.charAt(7)) )+10*charToInt(str.charAt(9))+charToInt(str.charAt(10));
			}


			for (int i=numAppointments-1; i>=0; i--){
				for (int j=0; j<i; j++){
					if(start[j]>start[j+1] || end[j]>end[j+1]){
						temp=start[j];
						start[j]=start[j+1];
						start[j+1]=temp;

						temp=end[j];
						end[j]=end[j+1];
						end[j+1]=temp;
					}
				}
			}


			for (int i=0; i<numAppointments; i++){
				tempStart=start[i];
				tempEnd=end[i];

				if( napM<(tempStart-prevEnd) ){
					napM=tempStart-prevEnd;
					napStart=prevEnd;
				}
				prevEnd=tempEnd;

				if( i==numAppointments-1 ){
					if( napM<(18*60-tempEnd) ){
						napM=18*60-tempEnd;
						napStart=tempEnd;
					}
				}
			}



			if( napStart%60==0 ){
				napStartStr=Integer.toString(napStart/60)+":00";
			}
			else{
				napStartStr=Integer.toString(napStart/60)+":"+Integer.toString(napStart%60);
			}

			if( napM<60 ){
				napStr=Integer.toString(napM)+" minutes.";
			}
			else{
				napStr=Integer.toString(napM/60)+" hours and "+Integer.toString(napM%60)+" minutes.";
			}

			System.out.println("Day #"+d+": the longest nap starts at "+napStartStr+" and will last for "+napStr);
			d++;
		}
		scan.close();
	}

	static int charToInt(char c){
		switch(c){
			case '0': return 0;
			case '1': return 1;
			case '2': return 2;
			case '3': return 3;
			case '4': return 4;
			case '5': return 5;
			case '6': return 6;
			case '7': return 7;
			case '8': return 8;
			case '9': return 9;
			default: return 0;
		}
	}
}
