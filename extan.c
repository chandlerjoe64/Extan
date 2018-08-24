#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include "extan.h"


void format_text(FILE* input_text, FILE* formatted_text) {
	int i;
	int j;
	char buffer[255];	//each word is read in to this buffer
	char output[255];	//after the word is formated, it is fed to this buffer
	memset(&output[0], 0, sizeof(output));	//initialize output buffer


	while(fscanf(input_text, "%s", buffer) != EOF) {	//iterated over every word in the text
		j = 0;
		for (i = 0; i < strlen(buffer); i++) {	//iterate over every character in the word
			if(!(ispunct(buffer[i]))) {
				output[j] = (tolower(buffer[i]));			//format and output each character into the output buffer
				j++;
			}
		}	
		if(j == 0) continue;		//if string is empty, do not write to output file

		output[j] = '\n';			//append newline to each word 
		fputs(output, formatted_text);	//write each word to formatted_text

		memset(&output[0], 0, sizeof(output));	//clear output buffer for next iteration
	}
	return;
}

char** populate_array(FILE* formatted_text) {
	//determine word count of formatted.txt
	int count;
	FILE* wc = popen("wc -w < tmp/formatted.txt", "r");	//open stream to system for wc call
	fscanf(wc, "%d", &count);	//read command output from the stream and store it in count
	pclose(wc);	//close stream 

	//create character array of size count
	char** words = malloc(count*sizeof(char*));

	//read in words from formatted_text line by line
	char line[255];
	int counter =0;

	rewind(formatted_text);	//move pointer back to beginning of file
	while(fscanf(formatted_text, "%s", line) != EOF) {	//iterated over every word in the text
		//printf("%s\n",line);	//DEBUG
		//TODO feed words into array
		counter++;
	}
	//printf("%s\n", words[999]);

	return words;
}