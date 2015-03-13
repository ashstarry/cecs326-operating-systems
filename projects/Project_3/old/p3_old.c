/*
* PROGRAM: p3.c
*
* Usage:
* ./p3.out <filename> <num_tries> <sleep_time>
*
* This program creates 3 child processes with child numbers [0,2]
* each child process attempts to create a lock file num_tries times.
*  - If successful, the contents of the lock file are displayed
*    and the child process exits with a return value equal to the 
*    least significant 8 bits of its process id
*  - If it is not successful after num_tries attempts, “Unable to 
*    obtain lock file” is displayed and the child process kills itself 
*    with a signal value equal to its child number k
*
* The parent process waits for each child process to complete and
* once complete prints "Wait on PID: childpid returns status of (status)” 
* where status is the status of the return in hexadecimal format.
*
*/
# include <stdio.h>
# include <sys/types.h>
# include <unistd.h>
# include <sys/wait.h>
# include <stdlib.h>  
# include <stdbool.h>

int main(int argc, char *argv[])
{
	pid_t pid, w;
	int k, i, num_tries, sleep_time, status;
	const char *filename = argv[1];
	bool lock_success = false;

	// Validate arg amount
	if(argc != 4) 
	{
		printf("\nArgument Error: Incorrect number of arguments.\nUsage: %s [filename] [num_tries] [sleep_time]\n\n", argv[0]);
		exit(1);		
	}

	// Check if num_tries and sleep_time command line arguments are integers
	// If they are initialize, if not print error and exit(1)
	if( (sscanf(argv[2], "%i", &num_tries) != 1)
	    || (sscanf(argv[3], "%i", &sleep_time) != 1)) {
        	printf("\nArgument Error: num_tries and sleep_time arguments must be integers\n");
		printf("\nUsage: %s [filename] [num_tries] [sleep_time]\n", argv[0]);
        	exit(1); 
	}

	// Check that num_tries and sleep_time are positive integers
	// If they are not, print error and exit(1)
	if(num_tries < 0 || sleep_time < 0) {
		printf("\nArgument Error: num_tries and sleep_time must be positive integers.\n");
		printf("\nUsage: %s [filename] [num_tries] [sleep_time]", argv[0]);
		exit(1);
	}

	for(k = 0; k < 3; ++k){
		// Create child process
		if((pid = fork()) == 0) {
			// if child...

			/* TODO: INSERT CHILD LOGIC HERE */

			// break from child process creation loop
			break;
		} else {
			// if parent...

			/* TODO: REMOVE ANY LEFTOVER LOCK FILES THAT MAY HAVE EXISTED FROM PREVIOUS INVOCATIONS */
		}
	}

	// Wait for children
	while((w = wait(&status)) && w != -1) {
		// when child has finished processing, print that childs info
		// including the returned status
		printf("Wait on PID: %d returns status of: %04X\n", w, status);
	}
	exit(0);
}
