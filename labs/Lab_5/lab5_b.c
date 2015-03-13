/*
* PROGRAM: lab5_b.c 
*
* Usage: 
* ./lab5_b.c <N-number-of-children> <T-sleep-time>
*
* argv limitations:
* 1 <= N < 20
* 0 <= T < 50
*
* This is a parent program which calls the fork()
* system call to create N children, each child process
* uses the execl call to call the child.out program
* passing the child number (k) and sleep time as arguments. 
* For each forked child the parent prints the forked child's PID.
*
* After the 3 child processes are iniated, the parent process
* waits for each child to complete and as they each complete
* prints the child PID and return value (status)
* 
*/
# include <stdio.h>
# include <sys/types.h>
# include <unistd.h>
# include <sys/wait.h>
# include <stdlib.h>  

# define MAX_SLEEP 50
# define MAX_CHILDREN 20

main(int argc, char *argv[])
{
	pid_t pid, w;
	int k, status, N, T;
	char value[3];

	// Validate arg amount
	if(argc != 3) 
	{
		printf("\nERROR: Incorrect number of arguments.\nUsage: %s <number-of-children> <sleep-time>\n\n", argv[0]);
		exit(1);		
	}

	/* Error check the command line arguments */
	/* If they’re all valid initialize */
	if( (sscanf(argv[1], "%i", &N) != 1)
	    || (sscanf(argv[2], "%i", &T) != 1)) {
        	printf("Argument Error: All arguments must be integers\n");
		printf("Usage: %s <number-of-children> <sleep-time>\n", argv[0]);
        	exit(1); 
	}

	// Ensure argv's are within pre-defined boundary
	if(N >= MAX_CHILDREN || N < 1) {
		printf("\nNumber of children must be an integer less than 20 and greater than 0\n");
		exit(1);
	} else if (T >= MAX_SLEEP || T < 0) {
		printf("\nSleep time must be a positive integer less than 50.\n");
		exit(1);
	}

	for(k = 0; k < N; ++k){
		if((pid = fork()) == 0){
			// if child, assign k to value which is passed as
			// argv to child.out which is run via execl()
			sprintf(value, "%d", k);
			execl("child_b.out", "child_b.out", value, argv[2], (char *) 0);
		} else {
			// if parent, print forked child pid
			printf("Forked child %d\n", pid);
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
