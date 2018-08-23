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

   //populate_array variables
   char** words;

	//open files for execution and perform error checks
	input_text = fopen("Harry Potter and the Sorcerer's Stone.txt", "r");
	if(input_text == NULL) {
		printf("Failed to open input file...\nExiting...\n");
		exit(0);
	}

	formatted_text = fopen("tmp/formatted.txt", "w");
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

   //execute populate_array
   words = populate_array(formatted_text);

      



	//close sample text

	fclose(input_text);
	fclose(formatted_text);
	
	//remove tmp files after execution
	//system("exec rm -r tmp/*");
}
