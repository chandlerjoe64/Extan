/*
Joseph A. Chandler
email: chandlerjoe64@gmail.com
*/

/*
EXTAN (tEXT_ANanlysis) is a program which iterates over a given 
body of text to derive repeated patterns of words. Its purpose 
is to identify phrases or other repeating patterns to aid in the 
creation of password dictionaries.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>
#include "extan.h"


//command line options global variables
// -t ...sets the threshold for repetition in check_duplicates
//defult is 5
int threshold = 5;
// -l ... sets the length of strings to check
//default is 5
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
//-s ... toggles printing of spaces between words
//default is enabled
int print_space = 1;
//-h ... print usage message
//-r ... sort the results alphabetically or by occurence
//default is disabled
int sort_flag = 0;
//-d ... set number of cores to use for duplicate check
//default is 2
int thread_count = 2;

int main(int argc, char*argv[]) {
	//get command line options
	//if no options are supplied print standard usage
	if(argc ==1) {
		print_standard_usage(argv[0]);
		exit(0);
	}
	int param;
	 while ((param = getopt (argc, argv, ":t:l:f:o:d:hrcs")) != -1) {
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
	 		case 's':
	 			print_space = 0;
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
 			case 'h' :
 				print_standard_usage(argv[0]);
 				exit(0);
 				break;
 			case 'r' :
 				sort_flag = 1;
 				break;
 			case 'd' :
 				thread_count = atoi(optarg);
 				break;
	 			case ':' :
	 				printf("error: option %c requires an argument\n",optopt);
	 				print_standard_usage(argv[0]);
	 				exit(0);
		 	}
	 }
	 //check for option conficts
	 //ensure outfile is specified with sort option
	 if((out_flag ==0) && (sort_flag == 1)) {
	 	printf("the sort options requres that an output file be specified...\n");
	 	print_standard_usage(argv[0]);
	 	exit(0);
	 }

	//execute format_text to sanatize input text
	char** formatted_text;
	unsigned int formatted_text_count;
    formatted_text = format_text(&formatted_text_count);

	//formula to determine number of strings generated by generate_check_strings
	// (count*(length-1))-((((length-2)*(length-1))/2)+(length-1))
	//determine length of check_strings
	unsigned int check_count = (formatted_text_count*(lengthToCheck-1))-((((lengthToCheck-2)*(lengthToCheck-1))/2)+(lengthToCheck-1));

	//initialze check_strings
	char** check_strings = malloc(sizeof(char*) * check_count);

	//populate check_strings with generate_check_strings
	generate_check_strings(formatted_text, check_strings);

	//check for duplicate strings
	check_for_duplicates_manager(check_strings, check_count);

	//sort results if applicable
	if(sort_flag == 1 && prefix_count == 1) {
		char command[128];
		sprintf(command, "sort -hr %s > %s.tmp",outfile_name,outfile_name);
		system(command);
		//move tmp file into outfile
		sprintf(command, "mv %s.tmp %s", outfile_name, outfile_name);
		system(command);
	}
	//if count option isn't enabled, dont sort reverse
	else if(sort_flag == 1 && prefix_count == 0) {
		char command[128];
		sprintf(command, "sort -h %s > %s.tmp",outfile_name,outfile_name);
		system(command);
		//move tmp file into outfile
		sprintf(command, "mv %s.tmp %s", outfile_name, outfile_name);
		system(command);
	}


	//tidy up
	//close arrays
	free_array(formatted_text, formatted_text_count);
	free(formatted_text);

	free_array(check_strings, (int)check_count);
	free(check_strings);
}