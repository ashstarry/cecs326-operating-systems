/*
* PROGRAM: child_a.c 
* 
* This is a child program that is called via the execl() command
* from within the lab5_a.c parent program. The program sleeps for
* a random time in the range [0,4] seconds.
* 
* It then checks if the provided argument variable, which is the
* child process number in range [0,3), mod 2 is zero:
*   - If it is not, the child PID and signal code is printed to 
*     the console and the child process is terminated with the kill command 
*     passing signal "009".
*   - Else, the child PID and return_value are printed to the console and the 
*     child process is exited passing the return_value which is the child's pid mod 256.
*
*/
# include <stdio.h>
# include <sys/types.h>
# include <unistd.h>
# include <sys/wait.h>
# include <stdlib.h>

main(int argc, char *argv[])
{
	// Declare variables
	pid_t pid;
	int ret_value;

	// Initialize variables
	pid = getpid();
	ret_value = (int) (pid %256);
	
	// Seed random number generator with PID
	srand((unsigned) pid);

	// Sleep for random time modulo 5
	sleep(rand() % 5);

	if(atoi(*(argv + 1)) % 2){
		// argv[1] / child number % 2 != 0
		printf("Child %d is terminating with signal 009\n", pid);
		kill(pid, 9);
	} else {
		// argv[1] / child number % 2 == 0
		printf("Child %d is terminating with exit(%04X)\n", pid, ret_value);
		exit(ret_value);
	}
}
