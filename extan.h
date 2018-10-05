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
*/
void generate_check_strings(char** formatted_text, char** check_strings);

/*
*/
void free_array(char** array, int count);

/*
*/
void check_for_duplicates_manager(char** check_strings, unsigned int count);

/*
*/
check_string* generate_check_array(char** check_strings, unsigned int count);

/*
*/
void* check_for_duplicates(void* arg);

/*
*/
void print_standard_usage(char* command);