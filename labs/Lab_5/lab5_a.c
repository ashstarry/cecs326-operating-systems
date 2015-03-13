/*
* PROGRAM: lab5_a.c 
*
* This is a parent program which calls the fork()
* system call to create 3 children, each child process
* uses the execl call to call the child.out program
* passing the child number (k) as an argument. For each
* forked child the parent prints the forked child's PID.
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

main(void)
{
	pid_t pid, w;
	int k, status;
	char value[3];

	for(k = 0; k < 3; ++k){
		if((pid = fork()) == 0){
			// if child, assign k to value which is passed as
			// argv to child.out which is run via execl()
			sprintf(value, "%d", k);
			execl("child_a.out", "child_a.out", value, (char *) 0);
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
