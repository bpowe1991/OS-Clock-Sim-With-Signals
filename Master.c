/*
Programmer: Briton A. Powe          Program Homework Assignment #2
Date: 9/23/18                       Class: Operating Systems
File: Master.c
------------------------------------------------------------------------
Program Description:
Takes in integer command line parameters for option -n and -s to fork
and execvp n number of processes, allowing only s to be run simultaneously.
Program terminates children and self after catching a signal from a 
2 sec alarm or user enters Ctrl-C. Before ending, program outputs integer
values in shared memory and deallocates the shared memory segment. 
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
#include <signal.h>
#include <assert.h>

//Structure to be used in shared memory.
struct clock {
    int sec;
    int millisec;
};

int flag = 0;
pid_t parent;

int is_pos_int(char test_string[]);
void alarmHandler();
void interruptHandler();

int main(int argc, char *argv[]){

    signal(SIGINT,interruptHandler);
    signal(SIGALRM, alarmHandler);
    signal(SIGQUIT, SIG_IGN);
    alarm(2);
    
    struct clock *clockptr;
    char * workerArgPtr;
    int opt, n = 0, s = 0, shmid, status = 0, count = 0, running = 0;    
    key_t key = 3670402;
	pid_t childpid = 0, wpid;

    //Parsing options.
    while((opt = getopt(argc, argv, "n:s:hp")) != -1){
		switch(opt){
			
            //Option to enter number of processes.
            case 'n':
				if(is_pos_int(optarg) == 1){
					fprintf(stderr, "%s: Error: Entered illegal input for option -n\n",
							argv[0]);
					exit(-1);
				}
				else{
                    workerArgPtr = optarg;
                    n = atoi(optarg);
                    if (n <= 0) {
                        fprintf(stderr, "%s: Error: Entered illegal input for option -n\n",
							argv[0]);
                        exit(-1);
                    }
				}
				break;
            
            //Option to enter s.
            case 's':
				if(is_pos_int(optarg) == 1){
					fprintf(stderr, "%s: Error: Entered illegal input for option -s\n",
							argv[0]);
					exit(-1);
				}
				else{
                    s = atoi(optarg);
                    if (s <= 0 || s > 20) {
                        fprintf(stderr, "%s: Error: Entered illegal input for option -s:"\
                                        " Cannot exceed 20\n", argv[0]);
                        exit(-1);
                    }
				}
				break;
            
            //Help option.
            case 'h':
                fprintf(stderr, "\nThis program creates n number of child processes with the\n"\
                                "-n option. The maximum number of child processes allowed to \n"\
                                "run concurrently is designated with the -s option. Each child\n"\
                                "increments a timer in shared memory\n\n"\
                                "OPTIONS:\n\n"\
                                "-n Set the number of process to be entered. "\
                                "(i.e. \"executible name\" -n 4 creates 4 children processes).\n"\
                                "-s Set the number of children allowed to run at the same time."\
                                "(i.e. -s 4 allows 4 child processes to run at the same time).\n"\
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

    //Checking if n and s have valid integer values.
    if (n <= 0 || s <= 0){
        perror(strcat(argv[0], ": Error: Illegal parameter for -n or -s"));
        exit(-1);
    }
    
    //Creating shared memory segment.
    if ((shmid = shmget(key, sizeof(struct clock), 0666|IPC_CREAT)) < 0) {
        perror(strcat(argv[0],": Error: Failed shmget allocation"));
        exit(-1);
    }

    //Attaching to memory segment.
    if ((clockptr = shmat(shmid, NULL, 0)) == (void *) -1) {
        perror(strcat(argv[0],": Error: Failed shmat attach"));
        exit(-1);
    }

    //Loop to fork children
    while (count < n && flag == 0){
        if (running == s) {
            wait(NULL);
            running--;
        }
        
        //Forking child.
        if ((childpid = fork()) < 0) {
            perror(strcat(argv[0],": Error: Failed to create child"));
        }
        else if (childpid == 0) {
            char *args[]={"./Worker", workerArgPtr, NULL};
            if ((execvp(args[0], args)) == -1) {
                perror(strcat(argv[0],": Error: Failed to execvp child program\n"));
                exit(-1);
            }
        }
        
        count++;
        running++;
    }
    
    //Sending signal to all children
    if (flag == 1) {
        if (kill(-parent, SIGQUIT) == -1) {
            perror(strcat(argv[0],": Error: Failed kill"));
            exit(-1);
        }
    }

    while ((wpid = wait(&status)) > 0);
    fprintf(stderr, "\n%d sec, %d ms\n", clockptr->sec, clockptr->millisec);

    //Detaching from memory segment.
    if (shmdt(clockptr) == -1) {
        perror(strcat(argv[0],": Error: Failed shmdt detach"));
        clockptr = NULL;
        exit(-1);
    }

    //Removing memory segment.
    if (shmctl(shmid, IPC_RMID, 0) == -1) {
        perror(strcat(argv[0],": Error: Failed shmctl delete"));
        exit(-1);
    }
    
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

//Signal handler for 2 sec alarm
void alarmHandler() {
    flag = 1;
}

//Signal handler for Ctrl-C
void interruptHandler() {
    flag = 1;
}
