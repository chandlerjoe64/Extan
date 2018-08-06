/*
Joseph A. Chandler
email: chandlerjoe64@gmail.com
*/

/*
EXTAN (tEXT_ANanlysis) is a program which iterates over a given body of text to derive repeated patters of words. Its purpose is to identify phrases or other 
repeating patterns to aid in the creation of password dictionaries.
*/

#include<stdio.h>
#include<stdlib.h>

int main(int argc, char*argv[]) {
   	
   	//open sample text for reading
   	FILE *fp;
   	fp = fopen("Harry Potter and the Sorcerer's Stone.txt", "r");
	


	//close sample text
	fclose(fp);
	}
