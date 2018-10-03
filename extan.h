/*
takes as arguments pointers to files for input_text and 
output_text which have already been opened. The function
iterates over every word in input_text and santizes it 
by converting to lower case, removing punctuation, and 
terminating with a newline character. After each word is
sanatized, it is writed to output_text to await further 
operation.
*/
void format_text(FILE* formatted_text);

int populate_array(char** words, FILE* formatted_text);

void generate_check_strings(char** words, char** check_strings);

void free_array(char** array, int count);

void check_for_duplicates(char** check_strings, unsigned int count);

void print_standard_usage(char* command);

//global variables for command line options
extern int prefix_count;
extern int lengthToCheck;
extern int threshold;
extern char* file_name;
extern char* outfile_name;
extern int out_flag;
extern int print_space;