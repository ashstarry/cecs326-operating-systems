#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <string.h> 

// Define macro for the buffer size 
// to be used in UNIX read() system call
#define BUFSIZE 256

/* 
* Unnamed pipe pipe.c
*  
* Usage: ./pipe.out message_to_be_written
* 
* Parent writes a message to child process
*  
*/ 
void main(int argc, char *argv[])
{
	// Declare variables
	int f_des[2]; 
	static char message[BUFSIZE]; 
	char buffer[MAX_CANON]; 
	char *c; 
	int i,k, n; 
	pid_t childpid;

	// Validate arg count
	if (argc !=2) { 
		printf ("\n Usage: %s message\n", *argv); 
		exit(1);
	} 

	// Create unnamed pipe
	if (pipe(f_des) == -1) { 
		// If fail..
		// Print error and exit with non-zero failure status
		perror ("Pipe"); 
		exit(2); 
	} 

	// Fork the current process
	switch (fork()) {
		case -1: 
			// If fail..
			// Print error and exit with non-zero failure status
			perror ("Fork"); 
			exit(3); 
		case 0: 
			// If child...
			// Close file descriptor f_des[1]
			close(f_des[1]); 

			// Read BUFSIZE bytes from file descriptor f_des[2]
			// And store into message[] buffer
			if (read(f_des[0], message, BUFSIZE) != -1) { 
				// If success...
				// Print message that was read
				printf ("Message received by child: *%s*\n", message);
				// Flush buffered data 
				fflush(stdout); 
			} else { 
				// If fail..
				// Print error and exit non-zero failure status
				perror ("Read"); 
				exit(4);
			} 
			break; // Break from switch statement
		default: 
			// If parent...
			// Close file descriptor f_des[0]
			close(f_des[0]);

			// Write BUFSIZE bytes from argv[1] (message provided via command line)
			// to the file descriptor f_des[1]
			if (write(f_des[1], argv[1], strlen(argv[1])) != -1) {
				// If success...
				// Print message that was written
				printf ("Message sent by parent: [%s]\n", argv[1]); 
				// Flush buffered data
				fflush(stdout); 
			} else { 
				// If faile...
				// Print error and exit non-zero failure status
				perror ("Write"); 
				exit(5); 
			}
	}
	exit(0); // Program exits normally
}