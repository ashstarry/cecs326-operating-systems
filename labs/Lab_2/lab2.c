/**************************************************************************/
/* PROGRAM: lab2.c */
/* DESCRIPTION: This program generates a chain of processes
/* using fork(). The number of processes n is a command line argument
/* and should be less than 10. Each process sleeps for a random time 
/* less than m seconds, m being a command line argument that must
/* be less than 20, then prints out process ID, parent ID, and child ID
/**************************************************************************/
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <unistd.h>

void validate_args(int argc, char *argv[]);

int main(int argc, char *argv[])
{
	int i, m, n, sleeptime, seed;
	pid_t childpid;
	
	validate_args(argc, argv);
	
	n = atoi(argv[1]); 
	m = atoi(argv[2]);
	childpid = 0;
	
	for (i=0; i<n;i++) {
		if (childpid = fork()) 
			break;
		if (childpid == -1) {
			perror ("\n The fork failed\n");
			exit(1);
		}
	
		printf("\n %d: process ID:%6ld parent ID:%6ld child ID:%6ld",i, (long)getpid(), (long)getppid(), (long)childpid);
	
		seed = (int)(getpid() + childpid);
		srand(seed);
	
		/* since each process has a different childpid, using the childpid
		as the seed number will restart the random function.
		Therefore, each process will have a different sleeptime
		*/
		sleeptime = rand() % m;
		printf (" sleep = %d\n", sleeptime);
		sleep(sleeptime);
	}
	/* added wait so that the original process 
	does not quit before the child processes have finished 
	*/
        wait();
	exit(0);
}

/*
* Function: valdiate_args
* -----------------------
* Validates the command line arguments, exits program with warning if
* arguments are invalid.
*
* argc: number of command line arguments entered
* argv: array containing command line arguments
*/
void validate_args(int argc, char *argv[])
{
	/* Confirm that only one command line argument is given */
	if(argc != 3) {
		printf("Usage: ./lab2 <number-of-processes> <max-sleep-seconds>\n");
		exit(1);
	}
	
	int n = atoi(argv[1]);
	int m = atoi(argv[2]);

	if(n <= 0 || n >= 10) {
		printf("Usage: ./lab2 <number-of-processes> <max-sleep-seconds>\n");
   	 	printf("Erroneous input, number of processes must be an integer > 0 and < 10.\n");
		exit(1);
	}

    if(m < 0 || m >= 20) {
    	printf("Usage: ./lab2 <number-of-processes> <max-sleep-seconds>\n");
		printf("Erroneous input, max sleep seconds must be an integer >= 0 and < 20.\n");
        exit(1);
    }
}	
