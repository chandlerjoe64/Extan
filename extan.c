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
	fclose(formatted_text);
	return;
}

void populate_array(char* words[], FILE* formatted_text) {
	//read in words from formatted_text line by line
	char line[128];
	int counter =0;

	//rewind(formatted_text);	//move pointer back to beginning of file

	while(fscanf(formatted_text, "%s", line) != EOF) {	//iterated over every word in the text
		words[counter] = malloc(sizeof(line));	//allocate mem for each string
		strcpy(words[counter], line);	//copy lint into array
		counter++;
	}
	return;
}

void generate_check_strings(int lengthToCheck, char* words[], char* check_strings[]) {
	int length;
	int end;
	int i;
	int counter = 0;
	int arrayTracker = 0;
	char stringToCheck[64 * lengthToCheck]; //string to be compared -- buffer size 64 char * length 

	while(words[arrayTracker] != NULL) {
		length = 2;
		while(length <= lengthToCheck)	{	//generate all strings between the min and max word count
			end = 0;
			for(i=0; i < length; i++) {
				if(words[arrayTracker+i] != NULL) {
					strcat(stringToCheck, words[arrayTracker+i]);
					strcat(stringToCheck, " ");	//TODO fix trailing space
				}else {
					end = 1;
					break;
				}			
			}
			if(!end) {
				check_strings[counter] = malloc(sizeof(stringToCheck));	//allocate mem for each string
				strcpy(check_strings[counter], stringToCheck);	//copy lint into array
				counter++;
			}

			memset(stringToCheck, 0, sizeof(stringToCheck));	//reinitialze stringToCheck
			length++;	
		}
		arrayTracker++;
	}
	return;
}

void free_array(char* array[], unsigned int count) {
	int i;
	for(i=0;i<count;i++) {
		free(array[i]);
	}
	return;
}