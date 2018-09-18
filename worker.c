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

struct clock{
    int sec;
    int millisec;
};

int main(int argc, char *argv[]){ 
    int shmid, n;
    struct clock *clockptr;
    key_t key = 3670402;

    fprintf(stderr, "In child. Value of n is %s", argv[1]);

    //Finding shared memory segment.
    if ((shmid = shmget(key, sizeof(struct clock), 0666|IPC_CREAT)) < 0) {
        perror(strcat(argv[0],"Failed shmget allocation"));
        exit(-1);
    }

    //Attaching to memory segment.
    if ((clockptr = shmat(shmid, NULL, 0)) == (void *) -1) {
        perror(strcat(argv[0],"Failed shmat attach"));
        exit(-1);
    }
    
    //Testing shared memory manipulation.
    clockptr->millisec = 999;
    clockptr->sec = 7;

    //Detaching from memory segment.
    if (shmdt(clockptr) == -1) {
      perror(strcat(argv[0],"Failed shmdt detach"));
      clockptr = NULL;
      exit(-1);
   }


    return 0;


}
   