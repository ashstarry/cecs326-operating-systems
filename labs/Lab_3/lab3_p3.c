#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

/*
* 
* This program runs the execvp function passing argv[1]
* as the parameter.
*
* If the exec call fails, "exec failure" is printed to console
* and program exits with failure.
*
*/
void main(int argc, char *argv[]) {
	execvp(argv[1], &argv[1]);
	perror ("exec failure");
	exit(1);			
}
