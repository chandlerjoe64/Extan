#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include<time.h>
#include "extan.h"


char** format_text(int* formatted_text_count) {
	int i;
	int j;
	int k = 0;
	int counter = 0;
	char buffer[255];	//each word is read in to this buffer
	char output[255];	//after the word is formated, it is fed to this buffer
	memset(&output[0], 0, sizeof(output));	//initialize output buffer

	//perform error checking on source file
	FILE* input_text = fopen(file_name, "r");
	if(input_text == NULL) {
		printf("Failed to open input file...\nExiting...\n");
		exit(0);
	}

	//iterate over file to get count
	while(fscanf(input_text, "%s", buffer) != EOF) {
		//ensure string isn't exclusivley illegal characters
		j = 0;
		for (i = 0; i < strlen(buffer); i++) {	//iterate over every character in the word
			if(!(ispunct(buffer[i]))) {
				j++;
			}
		}	
		if(j == 0) continue;
		//if string is legal, iterate counter
		counter++;
	}
	
	//initialize array
	char** formatted_text = malloc(sizeof(char*) * (counter));

	//reset file pointer
	rewind(input_text);

	//format strings and prepare for writing
	printf("formatting source document...\n");
	while(fscanf(input_text, "%s", buffer) != EOF) {	//iterated over every word in the text
		j = 0;
		for (i = 0; i < strlen(buffer); i++) {	//iterate over every character in the word
			if(!(ispunct(buffer[i]))) {
				output[j] = (tolower(buffer[i]));			//format and output each character into the output buffer
				j++;
			}
		}	
		if(j == 0) continue;		//if string is empty, do not write to output file

		//write to array
		formatted_text[k] = malloc(sizeof(output));
		strcpy(formatted_text[k], output);
		k++;

		memset(&output[0], 0, sizeof(output));	//clear output buffer for next iteration
	}
	formatted_text[counter] = NULL; //null terminate array
	*formatted_text_count = counter;
	fclose(input_text);
	printf("finished...\n");
	return formatted_text;
}


void generate_check_strings(char** formatted_text, char** check_strings) {
	int length;
	int end;
	int i;
	int counter = 0;
	int arrayTracker = 0;
	char stringToCheck[64 * lengthToCheck]; //string to be compared -- buffer size 64 char * length 

	printf("generating candidate strings...\n");
	while(formatted_text[arrayTracker] != NULL) {
		length = 2;
		while(length <= lengthToCheck)	{	//generate all strings between the min and max word count
			end = 0;
			for(i=0; i < length; i++) {
				if(formatted_text[arrayTracker+i] != NULL) {
					strcat(stringToCheck, formatted_text[arrayTracker+i]);
				if(print_space == 1) {
					strcat(stringToCheck, " ");
				}
				}else {
					end = 1;
					break;
				}			
			}
			if(!end) {
				check_strings[counter] = (char*)malloc(sizeof(stringToCheck));	//allocate mem for each string
				strcpy(check_strings[counter], stringToCheck);	//copy line into array
				counter++;
			}

			memset(stringToCheck, 0, sizeof(stringToCheck));	//reinitialze stringToCheck
			length++;	
		}
		arrayTracker++;
	}
	printf("finished...\n");
	return;
}

void free_array(char** array, int count) {
	int i =0;
	for(i=0;i<count;i++) {
		free(array[i]);
	}
	return;
}

void check_for_duplicates(char* check_strings[], unsigned int count) {
	//loop control variables
	int i;
	int j;
	int k = 0;
	int l = 0;
	//timer and progress variables
	int percentage = 0;
	clock_t begin;
	clock_t end;
	double one_percent_time;
	double time_remaining;
	char* progress_string = "##################################################";
	int progress_width = 50;
	//tracking variables
	int occurences = 0;
	int match;
	//storage variables
	char** dirty_array = malloc(sizeof(char*) * count);
	char** clean_array = malloc(sizeof(char*) * count);
	char* print_string = (char*)malloc(sizeof(char) * 128);

	printf("checking for duplicate strings...\n");
	//iterate over each word in check_strings array
	for(i=0;i<count;i++) {
		//timer to estimate execution time remaining
		if(percentage == 0) {
			begin = clock();
		}
		if (percentage == 1) {
			end = clock();
			one_percent_time = (double)(end - begin) / CLOCKS_PER_SEC;
		}

		//check each string against the array for matches
		for(j=0;j<count;j++) {
			//if string matches, increase occurence count
			if(!(strcmp(check_strings[i],check_strings[j]))) {
				occurences++;
			}
		}
		//if occurences of string is >= threshold, generate and store formatted string
		if(occurences >= threshold) {
			if(prefix_count == 1) {
				sprintf(print_string, "%d: %s", occurences, check_strings[i]);
			}else {
				sprintf(print_string, "%s", check_strings[i]);
			}
			//print string to dirty array
			dirty_array[k] = (char*)malloc((sizeof(char)) * 128);
			strcpy(dirty_array[k],print_string);
			if(print_space == 1) dirty_array[k][strlen(dirty_array[k])-1] = 0;	//remove trailing space
			k++;
		}
		//reset for next execution
		occurences = 0;
		memset(print_string, 0, sizeof(print_string));
		
		//print progress bar as a percenage and remaining time		
		int divisor = count / 100;
		time_remaining = (one_percent_time * (double)100) - (one_percent_time * (double)percentage);

		//print progress bar
		int val = (int) (percentage);
	    int lpad = (int) ((percentage / (double)100) * progress_width);
	    int rpad = progress_width - lpad;
	    printf ("\r%3d%% [%.*s%*s]", val, lpad, progress_string, rpad, "");
	    //print time remaining
		if(percentage > 1) {
			if(time_remaining > 300) {
				printf(" %.0f minutes remaining", (time_remaining / 60));
			} else {
				printf(" %.0f seconds remaining", (time_remaining + 1));
			}
		}
		fflush (stdout);
		//iterate percentage
		if((i % divisor) == 0 && i > 0) {
			percentage++;
		}
	}
	printf("\n");	//newline after progress bar
	printf("finished...\n");

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

	//TODO optional sort

	//print clean array to appropriate location
	if(out_flag == 1) {
		printf("writing to file...\n");
		//open file for writing
		FILE* output_text = fopen(outfile_name, "w");
		if(output_text == NULL) {
			printf("Failed to open output file for writing...\nExiting...\n");
			exit(0);
		}

		for(i=0;i<l;i++) {
		fprintf(output_text, "%s\n", clean_array[i]);
		}
		fclose(output_text);
		printf("finished...\n");
	}else {
		for(i=0;i<l;i++) {
			printf("%s\n", clean_array[i]);
			}
	}
	

	//cleanup
	//free(print_string);
	free_array(dirty_array, k);
	free_array(clean_array, l);
}	

void print_standard_usage(char* command) {
	printf("usage: %s [OPTIONS] -f [FILE]\n", command);
	printf("options:\n");

	//input file
	printf("\t-f [FILE]\n");
	printf("\t\tfile of strings to be checked (required)\n");
	printf("\t\texample: %s -f infile.txt\n", command);

	//output file
	printf("\t-o [FILE]\n");
	printf("\t\touput results to [FILE] (default is stdout)\n");
	printf("\t\texample: %s -o outfile.txt -f [fILE]\n",command);

	//check length
	printf("\t-l [INT]\n");
	printf("\t\tmax number of words in word-group to check (default is 5)\n");
	printf("\t\texample: %s -l 4 -f [FILT]\n",command);

	//match threshold
	printf("\t-t [INT]\n");
	printf("\t\tthreshold of how many times a word-group must appear to be included in results (default is 5)\n");
	printf("\t\texample: %s -t 4 -f [FILE]\n",command);

	//preface count
	printf("\t-c\n");
	printf("\t\tenable prepending the number of occurences of each word-group to the results (default is disalbed)\n");
	printf("\t\texample: %s -c -f [FILE]\n", command);

	//spaces in results
	printf("\t-s\n");
	printf("\t\tstrip spaces from the resulting word-groups (default is disalbed)\n");
	printf("\t\texample: %s -s -f [FILE]\n", command);

	//sort
	printf("\t-r\n");
	printf("\t\tsort the results by occurence if [-c] is provided, otherwise alphabetically (BETA feaure, Debian only)\n");
	printf("\t\trequires an output file be specified with the [-o] option\n");
	printf("\t\texample: %s -o [OUTFILE] -r -f [FILE]\n", command);

	//diplay this message
	printf("\t-h\n");
	printf("\t\tdisplay this help message and exit\n");
	printf("\t\texample: %s -h\n", command);

	//whitespace for formatting
	printf("\n");
}