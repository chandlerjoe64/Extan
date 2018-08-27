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
   	
	//initialize file pointers
	FILE *input_text;
	FILE *formatted_text;
	FILE *checked_list;
	FILE *found_list;

	//TODO create file opener function
	//open files for execution and perform error checks
	input_text = fopen("Harry Potter and the Sorcerer's Stone.txt", "r");
	if(input_text == NULL) {
		printf("Failed to open input file...\nExiting...\n");
		exit(0);
	}

	formatted_text = fopen("tmp/formatted.txt", "rw");
	if(formatted_text == NULL) {
		printf("Failed to initialize output file...\nExiting...\n");
		exit(0);
	}

	checked_list = fopen("tmp/checked.txt", "w");
	if(checked_list == NULL) {
		printf("Failed to initialize checked file...\nExiting...\n");
	}

	found_list = fopen("tmp/found.txt", "w");
	if(found_list == NULL) {
		printf("Failed to initialize found file...\nExiting...\n");
	}

	//execute format_text to sanatize input text
   format_text(input_text, formatted_text);

   //determine word count of formatted.txt
	int count;
	FILE* wc = popen("wc -w < tmp/formatted.txt", "r");	//open stream to system for wc call
	fscanf(wc, "%d", &count);	//read command output from the stream and store it in count
	pclose(wc);	//close stream     

	//initialize array
	char* words[count];
	populate_array(words, formatted_text);

	//call check_duplicates
	int lengthToCheck = 5;
	check_duplicates(lengthToCheck, words, checked_list, found_list);

	//close sample text
	fclose(input_text);
	fclose(formatted_text);
	
	//remove tmp files after execution
	//system("exec rm -r tmp/*");
}
