#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

/*
* 
* This program assembles the argv's to be passed
* to the execvp function and calls lab1 with argv 5
*
* If the exec call fails, "exec failure" is printed
* to console and program exits with failure.
*
*/
void main(int argc, char *argv[]) {
	static char *new_argv[] = {"lab1", "5", (char *) 0};
	execvp("/home/ubuntu/projects/cecs326-os/Labs/Lab_1/lab1", new_argv);
	perror("exec failure");
	exit(1);
}
