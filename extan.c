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
#include<ctype.h>
#include<string.h>
#include "extan.h"

int main(int argc, char*argv[]) {
   	
   	//open sample text for reading
   	FILE *fp;
   	fp = fopen("Harry Potter and the Sorcerer's Stone.txt", "r");
	
   	FILE* formatted_text = format_text(fp);

	//close sample text
	fclose(fp);
	}

FILE * format_text(FILE* input_text) {
	int i;
	char buffer[255];	//each word is read in to this buffer
	char output[255];	//after the word is formated, it is fed to this buffer

	while(fscanf(input_text, "%s", buffer) != EOF) {	//iterated over every word in the text
		for (i = 0; i < strlen(buffer); i++) {	//iterate over every character in the word
			if(!(ispunct(buffer[i]))) {

			output[i] = (tolower(buffer[i]));			//format and output each character into the output buffer
			if(strcmp(output,"")) continue;		//if string is empty, do not write to output file
			}

		}	

		printf("%s\n", output);	//TODO print output buffer to formated text file
		memset(&output[0], 0, sizeof(output));	//clear output buffer for next iteration

		//printf("%s\n", buffer);
	}

	FILE *return_file;
	return return_file;

}