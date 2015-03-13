#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#define MAX_SLEEP 50

/*
* PROGRAM: project2-parent.c
*
* Usage: ./project2-parent.out [sleep-time] [operand-1] [operand-2]
*
* Takes command line arguments for maximum sleep-time T which must be in the range
* 0 < sleep_time < 50 and two integer operands. The process then prints the two numbers
* and sleeps for a random time modulo T. Then the process is forked 4 times, if the fork
* is successfull, the child process makes a call to execlp to execute project2-child.out
* located in the same directory as project2-parent.out passing a unique child number in 
* the range [0,3], the operands and T. If the fork or exec calls fail the program 
* exits with failure.
*
*/
int main(int argc, char *argv[])
{
	// Declare variables
	int sleep_time, op1, op2, i, status_ptr;
        pid_t children[4];
	
	// Validate arg amount
	if(argc != 4) 
	{
		printf("\nERROR: Incorrect number of arguments.\nUsage: %s [sleep-time] [operand-1] [operand-2]\n\n", argv[0]);
		exit(1);		
	}

	/* Error check the command line arguments */
	/* If they’re all valid initialize */
	if( (sscanf(argv[1], "%i", &sleep_time) != 1)
	    || (sscanf(argv[2], "%i", &op1) != 1)
	    || (sscanf(argv[3], "%i", &op2) != 1)) {
        	printf("Argument Error: All arguments must be integers\n");
		printf("Usage: %s [sleep-time] [operand-1] [operand-2]", argv[0]);
        	exit(1); 
	}
	
	// Validate arg content
	if(sleep_time < 0 || sleep_time > MAX_SLEEP) 
	{
		printf("\nUsage: %s [sleep-time] [integer-operand-1] [integer-operand-2]\n[sleep-time] must be a positve integer less than or equal to %d\n\n", argv[0], MAX_SLEEP);
		exit(1);		
	}
	
	// Print out the parent info
	printf("\nI am the parent process, the maximum sleep time is %d and the two numbers are %d and %d\n", sleep_time, op1, op2);
    
	// Initialize Random Number Generator (using time as seed)
	srand((int) time(NULL));
	
	// Randomizing sleep time
	sleep_time = rand() % sleep_time;

	// Sleep for randomized sleep_time
	sleep(sleep_time);

	for (i = 0; i < 4; i++) {
		// fork() to create child process and have child process run the execlp command
		// to run project2-child.out, passing argv[1-3] and child_num  as arguments to child process
		if((children[i] = fork()) == 0) {
			char child_num[15];
			snprintf(child_num, sizeof(child_num), "%d", i);
			printf("Forked child %ld\n", (long)getpid());
			execlp("./project2-child.out", "project2-child.out", argv[1], argv[2], argv[3], child_num, (char *)NULL);
			perror("exec failure");
			exit(1);
		} else if(children[i] == -1) {
			perror("\n The fork failed\n");
			exit(1);
		}
	}

	//wait for children to end
	waitpid(children[0], &status_ptr, 0);
   	waitpid(children[1], &status_ptr, 0);
	waitpid(children[2], &status_ptr, 0);
	waitpid(children[3], &status_ptr, 0);
	exit(0);

//end main
}
