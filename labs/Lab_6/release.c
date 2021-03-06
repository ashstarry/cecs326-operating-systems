/*
* PROGRAM: release.c
* 
* USAGE: ./release.out <lock-filename> <sleeptime> <num_tries>
*
* This program makes <num_tries> attempts to release a given <lock-filename> 
* sleeping for a random number of seconds mod <sleeptime> for each try.  
* Note: the random number generator is seeded with the process id.
*
* Success:	lock file is released, success message printed, program exits naturally
* Failure:	lock file is not released, error message printed, program exits with failure
*
*/
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <errno.h>
# include <sys/types.h>

void main(int argc, char *argv[]) {
	char *fname = argv[1]; 
	int fd, sleeptime, n_try, count=0; 
	pid_t pid = getpid();

	if(argc != 3) {
		printf("\nArgument Error: Invalid number of arguments \n");
		printf("\nUSAGE: %s <lock-filename> <sleeptime> <num_tries>\n", arg[0]);
		exit(-1);
	}

	if( (sscanf(argv[2], "%i", &sleeptime) != 1)
	    || (sscanf(argv[3], "%i", &n_try) != 1)) {
        	printf("\nArgument Error: n_tries and sleeptime arguments must be integers\n");
			printf("\nUSAGE: %s <lock-filename> <sleeptime> <num_tries>\n", arg[0]);
        	exit(1); 
	}

	srand((unsigned) pid);
	
	while (unlink(fname)!=0) {
		if (++count < n_try) 
			sleep(rand() % sleeptime);
		else { 
			printf ("\n Cannot release file\n"); 
			exit(-1); 
		}
	}

	printf ("\n File is released\n"); 

}