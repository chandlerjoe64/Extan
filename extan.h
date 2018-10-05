//typedef struct for use in check_for_duplicates
typedef struct Check_String {
	char** array;
	char* check_string;
	int occurences;
	int array_size;
} check_string;

//global variables for command line options
extern int prefix_count;
extern int lengthToCheck;
extern int threshold;
extern char* file_name;
extern char* outfile_name;
extern int out_flag;
extern int print_space;
extern int thread_count;


/*
This functions takes a referenced pointer to an instantiated int (formatted_text_count),
utilizes the global variable file_name, and returns an array of character 
pointers. This function opens the file supplied by the global variable file_name 
with error checking, determines the number of legal strings in the file and 
stores the value in formatted_text count to be accesibly to the calling function, reads
in strings line by line, strips those strings of spaces, punctuation, mixed cases, 
and other illegal formats/chars, and adds them sequentially to the formatted_text array.
The functions returns the array formatted_array, loaded with the sanitized strings and 
sets the value of formatted_text_count so that the calling function can know the size of 
the array.
*/
char** format_text(int* formatted_text_count);

/*
This function takes two arrays of character pointers, one (formatted_text) 
containing the sanitized strings generated my format_text, and the other 
initialized but empty. This function makes us of the global variable 
length_to_check which is set by a command line  argument. The function 
returns void, but fills the empty array with candidate strings during its
execution to be utilized by the calling function. This function sequentially
reads in strings from formatted_text, and generates canditate strings
beginning at a length of 2 strings, and incrimenting to a max length of 
length_to_check. Each candidate string is sequentially loaded into check_strings
to be utilized by the calling function.
*/
void generate_check_strings(char** formatted_text, char** check_strings);

/*
This function iteratively frees memory from arrays that have had their indiividual
elements malloced. The function takes a pointer to an array and the size of the array,
and frees every element individually. This function does NOT free the array itself.
*/
void free_array(char** array, int count);

/*
This function takes an array of char pointers and the size of the given array.
This function controls the execution of the multithreaded check_for_duplicates 
algorithm with a variable number of threads and calls generate_check_array to 
supply the check_for_duplicates function. This function also estimates the 
execution time for the complete processing of the text, and prints the results 
to the console as an ascending percentage bar and a descendign execution timer.
after every candidate string has been checked for duplicate occurences, the structs
in check_array are iterated over and the candidate string and number of occurences 
are extracted, mutated into a printable string, and added to a temporary array if
they meet the minimum number of occurences. After the entirety of check_array is 
analyzed, the resultant array of printable strings is deduplicated, and any candidate 
strings which met the minimum threshold for occurences are fed into a second array,
storing final results. Finally, the resultant printable strings are either written
to an output file or printed to stdout, dependant on a command line option.  
*/
void check_for_duplicates_manager(char** check_strings, unsigned int count);

/*
This functions takes an array of character pointers (check_strings) and the size
of the array as arguments, accesses the global variable lengthToCheck, and returns
and array of check_string structs. The check_string struct allows all relevant
data to be passed to multithreaded functions in a containerized manner. This function
iterates over each candidate string in check_strings and for each string, creates a 
check_string struct. This function then sets the count property to the number of candidate
strings, sets the array property to the pointer to the check_strings array, sets the 
check_string proprety to the value of the array at the current index, and adds the loaded
struct to the check_string array which is returned after execution. 
*/
check_string* generate_check_array(char** check_strings, unsigned int count);

/*
this function is designed to be run in a POSIX thread (pthread). To accomodate this, 
this function returns a void pointer and accepts a void pointer its single argument.
The void pointer arg is a check_string struct which must be tpecast as such to be usable 
in this function. The provided struct, with its embedded candidate string, is checked
against the array to count duplicate occurences. The number of occurences is stored in
the occurences property of the struct for later processing. This functions exits with the 
pthead_exit(0) call, as the function is only executed in a pthread thread.
*/
void* check_for_duplicates(void* arg);

/*
This funtion prints a standard usage message for the user, indicating the proper
way to execute the application, and the usage of command line options. This function
is displayed automatically if the applications is executed with no arguments, or if 
the -h option is supplied.
*/
void print_standard_usage(char* command);