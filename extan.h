/*
takes as arguments pointers to files for input_text and 
output_text which have already been opened. The function
iterates over every word in input_text and santizes it 
by converting to lower case, removing punctuation, and 
terminating with a newline character. After each word is
sanatized, it is writed to output_text to await further 
operation.
*/
void format_text(FILE* input_text, FILE* formatted_text);

void populate_array(char* words[], FILE* formatted_text);

void generate_check_strings(int lengthToCheck, char* words[], char* check_strings[]);

void free_array(char* array[], unsigned int count);
