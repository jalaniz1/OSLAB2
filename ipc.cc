// 
// 
//
// Procrastinators
//

using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string>
#include <limits.h>
#include <sys/time.h>

#include "helper-routines.h"

/*Define Global Variables*/
pid_t   childpid;
timeval t1, t2;
int numtests;
double elapsedTime;


void sigusr1_handler(int sig) 
{	
	
}

void sigusr2_handler(int sig) 
{ 
	
}

void sigint_handler(int sig)
{

	//kill(-)
}


//
// main - The main routine 
//
int main(int argc, char **argv){
	//Initialize Constants here
	
    //variables for Pipe
	int fd1[2],fd2[2], nbytes;	
	//byte size messages to be passed through pipes	
	char    childmsg[] = "1";
	char    parentmsg[] = "2";
	char    quitmsg[] = "q";
    
    /*Three Signal Handlers You Might Need
     *
     *I'd recommend using one signal to signal parent from child
     *and a different SIGUSR to signal child from parent
     */
    Signal(SIGUSR1,  sigusr1_handler); //User Defined Signal 1
    Signal(SIGUSR2,  sigusr2_handler); //User Defined Signal 2
    Signal(SIGINT, sigint_handler);
    
    //Default Value of Num Tests
    int numtests;
    if (argv[2])
    	numtests= atoi(argv[2]); // Replaced with the command line arg for number of tests to run
   	else
   		numtests = 1000; // Else set the default number
    //Determine the number of messages was passed in from command line arguments
    //Replace default numtests w/ the commandline argument if applicable 
    if(argc<2){
		printf("Not enough arguments\n");
		exit(0);
	}
    
    printf("Number of Tests %d\n", numtests);
    // start timer
	gettimeofday(&t1, NULL); 
	if(strcmp(argv[1],"-p")==0){
		//code for benchmarking pipes over numtests
		
		// stop timer
		gettimeofday(&t2, NULL);

		// compute and print the elapsed time in millisec
		elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
		elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
		printf("Elapsed Time %f\n", elapsedTime);
	}
	if(strcmp(argv[1],"-s")==0){
		//code for benchmarking signals over numtests
		
		
		// stop timer
		gettimeofday(&t2, NULL);

		// compute and print the elapsed time in millisec
		elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
		elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
		printf("Elapsed Time %f\n", elapsedTime);
	}
	
}
  










