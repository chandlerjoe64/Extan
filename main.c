/*
Joseph A. Chandler
email: chandlerjoe64@gmail.com
*/

/*
EXTAN (tEXT_ANanlysis) is a program which iterates over a given 
body of text to derive repeated patters of words. Its purpose 
is to identify phrases or other repeating patterns to aid in the 
creation of password dictionaries.
*/

#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include "extan.h"


//command line options global variables
// -t ...sets the threshold for repetition in check_duplicates
//defult is 5
int threshold = 5;
// -l ... sets the length of strings to check
//default is 6
int lengthToCheck = 5;
// -c ... preface count to found strings 
//default is disabled
int prefix_count = 0;
//-f ... input file
//non-optional
char* file_name;
//-o ... write results to outfile
//default is stdout
char* outfile_name;
int out_flag = 0;


int main(int argc, char*argv[]) {
	//get command line parameters
	if(argc ==1) {
		printf("not enough arguments\n");	//DEBUG
		//TODO print standard usage template
		exit(0);
	}
	int param;
	 while ((param = getopt (argc, argv, ":t:l:cf:o:")) != -1) {
	 	switch (param) {
	 		case 't':
	 			threshold = atoi(optarg);
	 			break;
	 		case 'l':
	 			lengthToCheck = atoi(optarg);
	 			break;
	 		case 'c':
	 			prefix_count = 1;
	 			break;
	 		case 'f' :
 				file_name = malloc(sizeof(char) * 256);
 				file_name = optarg;
 				break;
	 		case 'o' :
 				outfile_name = malloc(sizeof(char) * 256);
 				outfile_name = optarg;
 				out_flag = 1;
 				break;
	 			case ':' :
	 				printf("error: option %c requires an argument\n",optopt);
	 				//TODO print standard usage template
	 				exit(0);
		 	}
	 }

   	
	//initialize file pointers
	FILE *formatted_text;

	formatted_text = fopen("tmp/formatted.txt", "w+");
	if(formatted_text == NULL) {
		printf("Failed to initialize output file...\nExiting...\n");
		exit(0);
	}

	//execute format_text to sanatize input text
    format_text(formatted_text);

    //determine word count of formatted.txt
	unsigned int count;
	FILE* wc = popen("wc -w < tmp/formatted.txt", "r");	//open stream to system for wc call
	fscanf(wc, "%d", &count);	//read command output from the stream and store it in count
	pclose(wc);	//close stream     

	//reopen formatted_text in r mode to avoid overwriting when rewinding in format_text
	formatted_text = fopen("tmp/formatted.txt", "r");
	if(formatted_text == NULL) {
		printf("Failed to initialize output file...\nExiting...\n");
		exit(0);
	}

	//initialize words and populate array
	char** words = malloc(sizeof(char*) * count);
	int words_count = populate_array(words, formatted_text);
	words[count] = NULL;	//null terminate the array

	//formula to determine number of strings generated by generate_check_strings
	// (count*(length-1))-((((length-2)*(length-1))/2)+(length-1))
	//determine length of check_strings
	unsigned int check_count = (count*(lengthToCheck-1))-((((lengthToCheck-2)*(lengthToCheck-1))/2)+(lengthToCheck-1));

	//initialze check_strings
	char** check_strings = malloc(sizeof(char*) * check_count);
	//check_strings[check_count] = NULL;

	//populate check_strings with generate_check_strings
	generate_check_strings(words, check_strings);

	//check for duplicate strings
	//int threshold = 5;	//threshold for how many times a string must appear to be considered repeated
	check_for_duplicates(check_strings, check_count);


	//tidy up
	//close arrays
	free_array(words, words_count);
	// TODO free array? (broken))

	free_array(check_strings, (int)check_count);
	free(check_strings);

	//close file pointers
	fclose(formatted_text);	
	//remove tmp files after execution
	//system("exec rm -r tmp/*");
}
