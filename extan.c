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
	printf("Finished executing format_text\n");	//DEBUG
	return;
}

void populate_array(char* words[], FILE* formatted_text) {
	//read in words from formatted_text line by line
	char line[128];
	int counter =0;

	while(fscanf(formatted_text, "%s", line) != EOF) {	//iterated over every word in the text
		words[counter] = malloc(sizeof(line));	//allocate mem for each string
		strcpy(words[counter], line);	//copy lint into array
		counter++;
	}
	printf("Finished executing populate_array\n");	//DEBUG
	return;
}

void generate_check_strings(int lengthToCheck, char* words[], char** check_strings) {
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
				check_strings[counter] = (char*)malloc(sizeof(stringToCheck));	//allocate mem for each string
				strcpy(check_strings[counter], stringToCheck);	//copy lint into array
				counter++;
			}

			memset(stringToCheck, 0, sizeof(stringToCheck));	//reinitialze stringToCheck
			length++;	
		}
		arrayTracker++;
	}
	printf("Finished executing generate_check_strings\n");	//DEBUG
	return;
}

void free_array(char* array[], unsigned int count) {
	int i;
	for(i=0;i<count;i++) {
		free(array[i]);
	}
	return;
}

void check_for_duplicates(char* check_strings[], unsigned int count, int threshold, FILE* found) {
	int i;
	int j;
	int k = 0;
	int l = 0;
	int percentage = 0;
	int occurences = 0;
	int match;
	char* dirty_array[count];
	char* clean_array[count];
	char* print_string = (char*)malloc(sizeof(char) * 128);	//seg fault here???

	//iterate over each word in check_strings array
	for(i=0;i<count;i++) {
		//check each string against the array for matches
		for(j=0;j<count;j++) {
			//if string matches, increase occurence count
			if(!(strcmp(check_strings[i],check_strings[j]))) {
				occurences++;
			}
		}
		//if occurences of string is >= threshold, generate and store formatted string
		if(occurences >= threshold) {
			sprintf(print_string, "%d: %s", occurences, check_strings[i]);
			//print string to dirty array
			dirty_array[k] = (char*)malloc((sizeof(char)) * 128);
			strcpy(dirty_array[k],print_string);
			k++;
		}
		//reset for next execution
		occurences = 0;
		memset(print_string, 0, sizeof(print_string));
		
		//print progress

		//TODO create timer which estimates time remaining
		int divisor = count / 100;
		if((i % divisor) == 0) {
			printf("%d%% complete...\n",percentage);
			percentage++;
		}
	}

	printf("Beginning to deduplicate array\n");	//DEBUG
	//deduplicate array
	for(i=0;i<k;i++){
		match = 0;
		for(j=0;j<l;j++) {
			if(!strcmp(dirty_array[i],clean_array[j])) {
				match = 1;
			}
		}
		if(match == 0) {
			//write to clean array
			clean_array[l] = (char*)malloc(sizeof(char) * 128);
			strcpy(clean_array[l], dirty_array[i]);
			l++;
		}

	}

	//print clean array to file
	for(i=0;i<l;i++) {
		fprintf(found, "%s\n", clean_array[i]);
	}

	//cleanup
	//free(print_string);
	free_array(dirty_array, k);
	free_array(clean_array, l);
	printf("Finished executing check_for_duplicates\n");	//DEBUG
}	