/*
Programmer: Briton A. Powe          Program Homework Assignment #1
Date: 9/7/18                        Class: Operating Systems
File: powe_proj1.c
------------------------------------------------------------------------
Program Description:
Takes in integer command line parameters for option -n and -c to output
a nchars length string generated from stdin for n processes.
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

struct clock{
    int sec;
    int millisec;
};

int main(int argc, char *argv[]){

    int opt, n = 0, i, nchars = 0, count = 0, character;
	pid_t childpid = 0;

    //Parsing options.
    while((opt = getopt(argc, argv, "n:c:hp")) != -1){
		switch(opt){
			
            //Option to enter number of processes.
            case 'n':
				if(is_pos_int(optarg) == 1){
					fprintf(stderr, "%s: Error: Entered illegal input for option -n\n",
							argv[0]);
					exit(-1);
				}
				else{
                    n = atoi(optarg);
                    if (n <= 0) {
                        fprintf(stderr, "%s: Error: Entered illegal input for option -n\n",
							argv[0]);
                        exit(-1);
                    }
				}
				break;
            
            //Option to enter nchars.
            case 'c':
				if(is_pos_int(optarg) == 1){
					fprintf(stderr, "%s: Error: Entered illegal input for option -c\n",
							argv[0]);
					exit(-1);
				}
				else{
                    nchars = atoi(optarg);
                    if (nchars <= 0) {
                        fprintf(stderr, "%s: Error: Entered illegal input for option -c\n",
							argv[0]);
                        exit(-1);
                    }
				}
				break;
            
            //Help option.
            case 'h':
                fprintf(stderr, "\nThis program creates n number of processes with the\n"\
                                "-n option. The program then reads characters from stdin\n"\
                                "and outputs the combined string for each process with the"\
                                " -c option.\n\n"\
                                "OPTIONS:\n\n"\
                                "-n Set the number of process to be entered. "\
                                "(i.e. \"executible name\" -n 4 creates 4 processes).\n"\
                                "-c Set the number of characters from stdin to be read. "\
                                "(i.e. -c 4 allows n processes to read 4 characters).\n"\
                                "-h Bring up this help message.\n"\
                                "-p Bring up a test error message.\n\n");
                exit(0);
                break;
            
            //Option to print error message using perror.
            case 'p':
                perror(strcat(argv[0], ": Error: This is a test Error message"));
                exit(-1);
                break;
            case '?':
                fprintf(stderr, "%s: Error: Unrecognized option \'-%c\'\n", argv[0], optopt);
                exit(-1);
                break;
			default:
				fprintf(stderr, "%s: Error: Unrecognized option\n",
					    argv[0]);
				exit(-1);
		}
	}

    //Checking if n and nchars have valid integer values.
    if (n <= 0 || nchars <= 0){
        perror(strcat(argv[0], ": Error: Illegal parameter for -n or -c"));
        exit(-1);
    }
    
    char mybuf[(nchars+1)];
    
    for (i = 0; i < n; i++){
        if ((childpid = fork())){
            break;
        }
    }

    wait(NULL);    
    
    //Loop to get input from stdin.
    fprintf(stderr, "Please enter %d characters:\n\n", nchars);
    while (count < nchars){
        while ((character = getchar()) != '\n'){
            mybuf[count] = character;
            count++;
        }
    }

    //Add string terminating char and output.
    mybuf[nchars] = '\0';
    fprintf(stderr, "\n%ld: %s\n\n", (long)getpid(), mybuf);
    
    return 0;         
}

//Function to check whether string is a positive integer
int is_pos_int(char test_string[]){
	int is_num = 0;
	for(int i = 0; i < strlen(test_string); i++)
	{
		if(!isdigit(test_string[i]))
			is_num = 1;
	}

	return is_num;
}        