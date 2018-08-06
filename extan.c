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
#include "extan.h"

int main(int argc, char*argv[]) {

}

int open_text(char* filename, FILE* text) {
	if((text = fopen(filename, "r")) == NULL) {
		return 1;		//return 1 if error
	}
	return 0;			//return 0 if no error

}