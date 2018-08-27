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

void populate_array(char* words[], FILE* formatted_text) {
	//read in words from formatted_text line by line
	char line[128];
	int counter =0;

	rewind(formatted_text);	//move pointer back to beginning of file

	while(fscanf(formatted_text, "%s", line) != EOF) {	//iterated over every word in the text
		words[counter] = malloc(sizeof(line));	//allocate mem for each string
		strcpy(words[counter], line);	//copy lint into array
		counter++;
	}
	return;
}

void check_duplicates(int lengthToCheck, char* words[], FILE* checked_list, FILE* found_list) {
	int length;
	int i;
	int arrayTracker = 0;
	char stringToCheck[512]; //string to be compared -- buffer size 512 char 

	while(words[arrayTracker] != NULL) {
		length = 2;
		while(length <= lengthToCheck)	{	//generate all strings between the min and max word count
			for(i=0; i < length; i++) {
				if(words[arrayTracker+i] != NULL) {
					strcat(stringToCheck, words[arrayTracker+i]);
				}else continue;
			}
			//printf("%d: %s\n",length, stringToCheck);	//DEBUG
			
			//TODO search text for stringToCheck
			stringToCheck[0] = '\0';	//reinitialze stringToCheck
			length++;	
		}
		arrayTracker++;
	}
	return;
}