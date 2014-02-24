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
pid_t   childpid, parentpid;
pid_t grpidc, grpidp;
timeval t1, t2, start, end;
int numtests;
double elapsedTime;



void sigusr1_handler(int sig) // Parent to child
{	
	// Child receives here...
	

		//printf("Parent pid is %d\n", getppid());
		//printf("Received SIGUSR1!\n");
		kill(getppid(), SIGUSR2);		
	
	
}

void sigusr2_handler(int sig) // Child to parent
{ 
	// Parent receives here
	

		//printf("Child pid is %d\n", childpid);	
		//printf("Received SIGUSR2!\n");
		kill(childpid, SIGUSR1);
	
	
}

void sigint_handler(int sig)
{
	//printf("Kill order\n");
	kill(getpid(),SIGCONT);
}

int Setgpid(pid_t pid, pid_t gid) // Wrapper for setpgid
{
	pid_t val = setpgid(pid, gid);
	if (val == -1)
	{
		printf("Error in Setpgid: %s\n", strerror(val));
	}
	return val;
}
double max_Num (double a, double b)
{
	double r = a;
	(r < b) && (r = b);
	return r;
}


// Started at 3:30pm 2/22/2014 - JA

//
// main - The main routine 
//
int main(int argc, char **argv){
	//Initialize Constants here
	
    //variables for Pipe
	int fd1[2],fd2[2];	
	//byte size messages to be passed through pipes	
	char    childmsg[] = "1";
	char    parentmsg[] = "2";
	char    quitmsg[] = "q";
	char readbuffer [2]; // Bytesized buffer with null terminator
	int nbytes = 0;
	struct sigaction mySigActions;
	//sigset_t block_set;
	double min =LONG_MAX, max = LONG_MIN, average = 0;
	

	
  
    /*Three Signal Handlers You Might Need
     *
     *I'd recommend using one signal to signal parent from child
     *and a different SIGUSR to signal child from parent
     */
    Signal(SIGUSR1,  sigusr1_handler); //User Defined Signal 1
    Signal(SIGUSR2,  sigusr2_handler); //User Defined Signal 2
    Signal(SIGINT, sigint_handler); // For signal benchmarking

    //sigfillset(&block_set);
    //sigdelset(&block_set, SIGUSR1);
    //sigdelset(&block_set, SIGUSR2);
    //sigdelset(&block_set, SIGINT);
     mySigActions.sa_handler = sigint_handler;
  	sigemptyset(&mySigActions.sa_mask);
  	mySigActions.sa_flags = 0;
  if (sigaction(SIGHUP, &mySigActions, NULL) < 0) {
    perror("Sigaction failed for SIGHUP");
    exit(1);
  }
  
    mySigActions.sa_handler = sigint_handler;
    sigemptyset(&mySigActions.sa_mask);
    mySigActions.sa_flags = 0;
    if (sigaction(SIGINT, &mySigActions, NULL) < 0)
    {
    	perror("Sigaction failed for SIGINT");
    	exit(1);
    }

    mySigActions.sa_handler = sigusr1_handler;
    sigemptyset(&mySigActions.sa_mask);
    mySigActions.sa_flags = 0;
    if(sigaction(SIGUSR1, &mySigActions, NULL) < 0)
    {
    		perror("Sigaction failed for SIGUSR1");
    		exit(1);
    }
    mySigActions.sa_handler = sigusr2_handler;
    sigemptyset(&mySigActions.sa_mask);
    mySigActions.sa_flags = 0; // No flags
    if(sigaction(SIGUSR2, &mySigActions, NULL) < 0)
    {
    		perror("Sigaction failed for SIGUSR2");
    		exit(1);
    }

     //mySigActions.sa_handler = sigusr2_handler;
    sigemptyset(&mySigActions.sa_mask);
    mySigActions.sa_flags = 0; // No flags
    if(sigaction(SIGCONT, &mySigActions, NULL) < 0)
    {
    		perror("Sigaction failed for SIGCONT");
    		exit(1);
    }




    
    //Default Value of Num Tests
    int numtests;
    if (argv[2])

    	numtests= atoi(argv[2]); // Replaced with the command line arg for number of tests to run
    else
   		numtests = 10000; // Else set the default number
   	const int MAX_TESTS = numtests;  // For calculating average

    //Determine the number of messages was passed in from command line arguments
    //Replace default numtests w/ the commandline argument if applicable 
   	if(argc<2){
   		printf("Not enough arguments\n");
   		exit(0);
   	}
   	if(pipe(fd1) < 0 || pipe(fd2) < 0) // Create our pipes, which will be inherited
   		{
   			printf("%s\n", strerror(errno));
   			exit(0);
   		}

   	printf("Number of Tests %d\n", numtests);
    // start timer
   	gettimeofday(&t1, NULL); // Elapsed timer 1, inherited
   	if(strcmp(argv[1],"-p")==0){
		//code for benchmarking pipes over numtests
		
   		
   		
   		if((childpid = fork()) == -1) // Fork a child process
   		{
   			perror("Fork Error");
   			exit(1);
   		}

   		if (childpid == 0) // Child
   		{
   			   			
   			for(;numtests > 0; numtests--)
   			{   

   				gettimeofday(&start, NULL);
   				if ((nbytes=read(fd2[0], readbuffer, sizeof(readbuffer)))) // Input from Parent
   				{
   				
   					close(fd1[0]); // Close read for parent
   					close(fd2[1]); // Close write from parent
   					//printf("C Received char %s\n", readbuffer);
   				}

   				if(write(fd1[1],childmsg, (strlen(childmsg)+1))) // Send reply
   				{
   					gettimeofday(&end, NULL);
   					double eTime = (end.tv_sec - start.tv_sec) * 1000.0;
   					eTime += (end.tv_usec - start.tv_usec) / 1000.0;   // us to ms
   					average+=eTime; // Average
   					//printf ("C Time %f\n", eTime);
   					
					if (min >= eTime)
						min = eTime;
					
   					max = max_Num(eTime,max);
   				}
   				/* Waits for parent to communicate first. Then follow this same order
   				on every subsequent iteration*/
   			}
   		}
   		else // Parent
   		{

   			for(;numtests > 0; numtests--)
   			{
   				/* Have the parent initiate the communication so we are not in
   				a deadlock.  It also said in the specs for this project to
   				have the parent contact the child first. */
   				gettimeofday(&start, NULL);
   				write(fd2[1], parentmsg,(strlen(parentmsg)+1)); // Parent initiates communic.
   				
   				if((nbytes = read(fd1[0], readbuffer, sizeof(readbuffer)))) //Input from Child
   				{
   					
   					close(fd1[1]); // Close write from child
   					close(fd2[0]); // Close read for child
   					//printf("P Received char %s\n", readbuffer);
   					
   				}
   					gettimeofday(&end, NULL);
   					double eTime = (end.tv_sec - start.tv_sec) * 1000.0;
   					eTime += (end.tv_usec - start.tv_usec) / 1000.0;   // us to ms
   					average+=eTime; // Average
   					//printf ("C Time %f\n", eTime);
   					
					if (min >= eTime)
						min = eTime;
					
   					max = max_Num(eTime,max);
   		

   			}

   			
   			wait(0); // Waits for child to queue/print it's message and exit it's process
   			// Before continuing.  Which takes alot of time in miliseconds... Interesting.
   		}

   		
   		

		// stop timer
   		gettimeofday(&t2, NULL); // Elapsed timer 2

   		if (childpid == 0) // Child
   		{
   			printf("Child's Results for Pipe IPC mechanisms\n");

   		}
   		else // Parent
   			printf("Parent's Results for Pipe IPC mechanisms\n");	
		printf("Process ID is %d, Group ID is %d\n",(int)getpid(), (int)getpgid(getpid()));

		// compute and print the elapsed time in millisec

		elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
		elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
	
		printf("Average %.6f\n",average/MAX_TESTS); // 6 floating point precision
		printf("Maximum %.6f\n", max);
		printf("Minimum %.6f\n", min);
		printf("Elapsed Time %f\n", elapsedTime);
	}
	else if(strcmp(argv[1],"-s")==0){
		//code for benchmarking signals over numtests
		
   		if((childpid = fork()) == -1) // Fork a child process
   		{
   			perror("Fork Error");
   			exit(1);
   		}

   		if (childpid == 0) // Child
   		{
   			   			
   			for(;numtests > 0; numtests--)
   			{   

   				gettimeofday(&start, NULL);
   				sigsuspend(&mySigActions.sa_mask); // always returns -1, careful
   				//kill(getppid(), SIGUSR2);
   	   				
   				gettimeofday(&end, NULL);
   				double eTime = (end.tv_sec - start.tv_sec) * 1000.0;
   				eTime += (end.tv_usec - start.tv_usec) / 1000.0;   // us to ms
   				average+=eTime; // Average
   					//printf ("C Time %f\n", eTime);
   					
				if (min >= eTime)
					min = eTime;
					
   				max = max_Num(eTime,max);
   				
   				/* Waits for parent to communicate first. Then follow this same order
   				on every subsequent iteration*/
   			}


   		}
   		else // Parent
   		{
   			kill(childpid, SIGUSR1); // Initial signal
   			for(;numtests > 0; numtests--)
   			{
   				/* Have the parent initiate the communication so we are not in
   				a deadlock.  It also said in the specs for this project to
   				have the parent contact the child first. */
   				gettimeofday(&start, NULL);
   				
   				sigsuspend(&mySigActions.sa_mask); // Always returns -1, careful
   				
   				gettimeofday(&end, NULL);
   				double eTime = (end.tv_sec - start.tv_sec) * 1000.0;
   				eTime += (end.tv_usec - start.tv_usec) / 1000.0;   // us to ms
   				average+=eTime; // Average
   					//printf ("C Time %f\n", eTime);
   					
				if (min >= eTime)
					min = eTime;
					
   				max = max_Num(eTime,max);

   		

   			}
   			//kill(childpid,SIGINT); // Continue
   			//int status;
   			//while(waitpid(-1, &status, WUNTRACED)< 0);
   			wait(0); // Waits for child to queue/print it's message and exit it's process
   			// Before continuing.  Which takes alot of time in miliseconds... Interesting.
   		}

   		

		
		// stop timer
		gettimeofday(&t2, NULL);
		if (childpid == 0) // Child
   		{
   			printf("Child's Results for Pipe IPC mechanisms\n");

   		}
   		else // Parent
   			printf("Parent's Results for Pipe IPC mechanisms\n");	
		printf("Process ID is %d, Group ID is %d\n",(int)getpid(), (int)getpgid(getpid()));

		// compute and print the elapsed time in millisec
		elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
		elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
		printf("Average %.6f\n",average/MAX_TESTS); // 6 floating point precision
		printf("Maximum %.6f\n", max);
		printf("Minimum %.6f\n", min);
		printf("Elapsed Time %f\n", elapsedTime);
	}
	exit(0);
	
}











