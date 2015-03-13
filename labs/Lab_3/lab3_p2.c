#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

/*
*
* This program executes the lab1 program 
* (stored in /home/ubuntu/projects/cecs326-os/Labs/Lab_1/) 
* with argv[1] given as a single command line argument.  
* 
* If the exec call fails, "execfailure" error is printed to console and 
* program exits with failure.
*
* If argc is not > 1, usage message printed to console and program exits with failure.
*
*/
void main(int argc, char *argv[]) {
	if (argc > 1) {
		execlp("/home/ubuntu/projects/cecs326-os/Labs/Lab_1/lab1", "lab1", argv[1], (char *) NULL);
		perror ("exec failure");
		exit(1);
	}
	printf("\n Usage: %s text_file\n", *argv); 
	exit(1);
}
