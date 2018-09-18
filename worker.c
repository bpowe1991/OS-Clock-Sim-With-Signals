/*
Programmer: Briton A. Powe          Program Homework Assignment #1
Date: 9/7/18                        Class: Operating Systems
File: powe_proj1.c
------------------------------------------------------------------------
Program Description:
Takes in integer command line parameters for option -n and -c to output
a s length string generated from stdin for n processes.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 

int is_pos_int(char test_string[]);

int main(int argc, char *argv[]){ 
    return 0;


}

int is_pos_int(char test_string[]){
	int is_num = 0;
	for(int i = 0; i < strlen(test_string); i++)
	{
		if(!isdigit(test_string[i]))
			is_num = 1;
	}

	return is_num;
}        