#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include<time.h>
#include<pthread.h>
#include "extan.h"


char** format_text(int* formatted_text_count) {
	int i;
	int j;
	int k = 0;
	int MAX_SIZE = 64;
	int counter = 0;
	char buffer[512];	//each word is read in to this buffer
	char output[MAX_SIZE];	//after the word is formated, it is fed to this buffer
	memset(&output[0], 0, sizeof(output));	//initialize output buffer

	//perform error checking on source file
	FILE* input_text = fopen(file_name, "r");
	if(input_text == NULL) {
		printf("Failed to open input file...\nExiting...\n");
		exit(0);
	}

	//iterate over file to get count
	while(fscanf(input_text, "%s", buffer) != EOF) {
		//if buffer is greater than 64 characters, continue
		//assumes that the longest reasonable word is 64 chars
		//sanitizes long links or garbage text, preventing overflows
		//above fscanf can still be overflowed if word is greater than 512 chars
		if((strlen(buffer)) > MAX_SIZE) continue;

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
		//if buffer is greater than 64 characters, continue
		//assumes that the longest reasonable word is 64 chars
		//sanitizes long links or garbage text, preventing overflows
		//above fscanf can still be overflowed if word is greater than 512 chars
		if((strlen(buffer)) > MAX_SIZE) continue;

		//ensure string isn't exclusivley illegal characters
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
	memset(stringToCheck, 0, sizeof(stringToCheck));	//initialze stringToCheck

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

void check_for_duplicates_manager(char* check_strings[], unsigned int count) {
	//loop control variables
	int i, j;
	int k = 0;
	int l = 0;
	//timer and progress variables
	int percentage = 0;
	clock_t begin, end;
	double total_time;
	double time_remaining;
	char* progress_string = "##################################################";
	int progress_width = 50;
	//storage variables
	int match;
	char** dirty_array = malloc(sizeof(char*) * count);
	char** clean_array = malloc(sizeof(char*) * count);
	char* print_string = (char*)malloc(sizeof(char) * 128);


	printf("checking for duplicate strings...\n");

	//generate check_string array 	
	check_string* check_array = generate_check_array(check_strings, count);

	pthread_t tids[thread_count];	//create thread_count number of thread IDs
	//execute multithreaded check_for_duplicats algorithm with thread_count number of threads
	//prints progress bar and remaining time continuously during execution
	for(i=0;i<count;i+=thread_count) {
		//begin timer to estimate execution time
		if(percentage == 0) {
			begin = clock();
		}

		//spawn specified number of threads
		for(j=0;j<thread_count;j++) {
			pthread_create(&tids[j], NULL, check_for_duplicates, &check_array[i+j]);
		}
		//join specified number of threads
		for(j=0;j<thread_count;j++) {
			pthread_join(tids[j], NULL);
		}

		//measure execution time for 2% of the algorithm and derive total execution time for countdown timer
		if (percentage == 2) {
			end = clock();
			total_time = ((((double)(end - begin) / CLOCKS_PER_SEC) * (double)50) / thread_count);
		}

		//continuously print progress bar
		percentage = (int) ((((double)i/ (double)count) * 100) + 1);
		int val = (int) (percentage);
	    int lpad = (int) ((percentage / (double)100) * progress_width);
	    int rpad = progress_width - lpad;
	    printf ("\r%3d%% [%.*s%*s]", val, lpad, progress_string, rpad, "");
	    
	    //continuously print remaining time
	    time_remaining = (double)(total_time - (total_time * ((double)percentage / (double)100)));
	    if(percentage >= 2) {
			if(time_remaining > 300) {
				printf(" %.0f minutes remaining", (time_remaining / 60));
			} else {
				printf(" %.0f seconds remaining", (time_remaining + 1));
			}
		}
	    fflush (stdout);
	}
	printf("\n");	//newline after progress bar

	//handle results that are at or above threshold		
	for(j=0;j<count;j++) {
		//if occurences of string is >= threshold, generate and store formatted string
		if(check_array[j].occurences >= threshold) {
			if(prefix_count == 1) {
				sprintf(print_string, "%d: %s", check_array[j].occurences, check_array[j].check_string);
			}else {
				sprintf(print_string, "%s", check_array[j].check_string);
			}
			//print string to dirty array
			dirty_array[k] = (char*)malloc((sizeof(char)) * 128);
			strcpy(dirty_array[k],print_string);
			if(print_space == 1) dirty_array[k][strlen(dirty_array[k])-1] = 0;	//remove trailing space
			k++;
			//reset for next execution
			memset(print_string, 0, sizeof(print_string));
		}
	}

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

	// //TODO free check_array elements
}	

check_string* generate_check_array(char** check_strings, unsigned int count) {
	int i;
	check_string* check_array = malloc(sizeof(check_string) * count);

	for(i=0;i<count;i++) {
		check_array[i].array_size = count;
		check_array[i].array = check_strings;
		(&check_array[i])->check_string = malloc(sizeof(char) * (64 * lengthToCheck));
		check_array[i].check_string = check_strings[i];
	}
	return check_array;
}

void* check_for_duplicates(void* arg) {
	int i;
	int occurences = 0;
	check_string* check = (check_string*) arg;
	check->occurences = 0;

	int size = check->array_size;
	for(i=0;i<check->array_size;i++) {
		if(!strcmp(check->check_string, check->array[i])) {
			check->occurences++;
		}
	}
	//return;
	pthread_exit(0);
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

	//thread count
	printf("\t-d [INT]\n");
	printf("\t\tset the number of threads to be used by the duplicate check function\n");
	printf("\t\tit is recommended to set this value no higher than the number of logical processors available (default is 2)\n");
	printf("\t\texample: %s -d 8 -f [FILE]\n", command);

	//diplay this message
	printf("\t-h\n");
	printf("\t\tdisplay this help message and exit\n");
	printf("\t\texample: %s -h\n", command);

	//whitespace for formatting
	printf("\n");
}