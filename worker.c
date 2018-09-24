/*
Programmer: Briton A. Powe          Program Homework Assignment #2
Date: 9/23/18                       Class: Operating Systems
File: worker.c
------------------------------------------------------------------------
Program Description:
Worker program that reads an integer value from execvp and increments
the variables of a struct in shared memory based on the integer.
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

//Structure in shared memory.
struct clock{
    int sec;
    int millisec;
};

struct clock *clockptr;

void sigQuitHandler(int);

int main(int argc, char *argv[]){ 
    signal(SIGQUIT, sigQuitHandler);
    int shmid, n = atoi(argv[1]), i;
    key_t key = 3670402;

    //Finding shared memory segment.
    if ((shmid = shmget(key, sizeof(struct clock), 0666|IPC_CREAT)) < 0) {
        perror(strcat(argv[0],": Error: Failed shmget find"));
        exit(-1);
    }

    //Attaching to memory segment.
    if ((clockptr = shmat(shmid, NULL, 0)) == (void *) -1) {
        perror(strcat(argv[0],": Error: Failed shmat attach"));
        exit(-1);
    }
    
    //Incrementing clock in shared memory.
    for (i = 0; i < (n*1000000); i++) {

        clockptr->millisec += 1;
        
        if (clockptr->millisec > 999) {
            clockptr->sec += (clockptr->millisec/1000);
            clockptr->millisec = (clockptr->millisec%1000);
        }
    }

    //Detaching from memory segment.
    if (shmdt(clockptr) == -1) {
      perror(strcat(argv[0],": Error: Failed shmdt detach"));
      clockptr = NULL;
      exit(-1);
   }

    return 0;
}

//Handler for quit signal.
void sigQuitHandler(int sig) {
   abort();
}
   