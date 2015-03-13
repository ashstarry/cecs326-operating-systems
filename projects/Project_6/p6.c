/* 
* 	COURSE:			CECS 326
*	SECTION:		TR 7:30pm
*	INSTRUCTOR:		Haney Williams
*	ASSIGNMENT:		Project 6
*	GROUP:			Ariel Katz
*				Michael Williams
*				Kristin Peterson
*
*	USAGE:
*	./p6.out [n|u: pipe-type] [pipe-name] [p|c: direction of message] [message]
*
*	This program creates a pipe to pass a message between a parent and
*	it's child process.  Depending on user input at the command line the
*	pipe will either be named/unnamed and the direction the message travels
*	will either be from parent->child (if 'p' is chosen direction) or from
*	child->parent (if 'c' is chosen direction).  The message will be printed
*	by the sender as is, and the receiver will print the message in opposite case.
*	The child PID is printed by the receiver when printing the opposite case message.
*
*/
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <string.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <limits.h>
#include <errno.h>

// Define macro for the buffer size 
// to be used in UNIX read() system call
#define BUFSIZE 256

// Function prototype
void reverseCase(char *message);

void main(int argc, char *argv[])
{
	/*
	* f_des[2]:	for unnamed pipe, r/w file descriptor
	* fifo_mode:	for named pipe, indicates fifo mode
	* fd:		for named pipe, holds the file descriptor returned by UNIX open()
	* buffer:	holds message to pass through pipe
	* child:	holds the pid of the forked child process
	* direction:	indicates the direction to send the message
	*		p: parent to child || c: child to parent
	* pipeName:	name of the paipe
	* pipeType:	the type of pipe
	*		n: named || u: unnamed
	* message:	the message to send
	* strsize:	holds size of message for use in UNIX write()
	*/
	int f_des[2];
	mode_t fifo_mode = S_IRUSR | S_IWUSR;
	int fd, status;
	char buffer[BUFSIZE];
	int child;
	const char *direction;
	const char *pipeType;
	const char *pipeName;
	const char *message;
	unsigned strsize;

	/* ******************* */
	/* ARGUMENT VALIDATION */
	/* ******************* */

	// Validate arg count
	if(argc != 4 && argc != 5) {  
		printf("Usage: ./p6.out [n|u: pipe-type] [pipe-name] [p|c: direction] [message]\n"); 
		exit(1);
	} 

	// Validate argv[1]
	if(strcmp(argv[1], "n") != 0 && strcmp(argv[1], "N") != 0 
			&& strcmp(argv[1], "u") != 0  && strcmp(argv[1], "U") != 0 ) {
		printf("Usage: ./p6.out [n|u: pipe-type] [pipe-name] [p|c: direction] [message]\n"); 
		exit(1);
	}

	pipeType = argv[1];

	// Assign direction, depends on whether u|n option is chosen
	if(strcmp(pipeType, "n") == 0 || strcmp(pipeType, "N") == 0) {
		// if n option chosen:
		pipeName = argv[2];
		direction = argv[3];
		// ensure message was entered, if not exit
		if(argc == 5) {
			message = argv[4];
		} else {
			printf("Usage: ./p6.out [n|u: pipe-type] [pipe-name] [p|c: direction] [message]\n");
			exit(1);
		}
	} else {
		// else, u option chosen:
		direction = argv[2];
		message = argv[3];
	}

	// Validate direction
	if(strcmp(direction, "p") != 0 && strcmp(direction, "P") != 0 
			&& strcmp(direction, "c") != 0  && strcmp(direction, "C") != 0 ) {
		printf("Argument Error: message direction must be either p (parent->child) or c (child->parent)\n");
		printf("Usage: ./p6.out [n|u: pipe-type] [pipe-name] [p|c: direction] [message]\n"); 
		exit(1);
	}

	/* ************************************************ */
	/* Create pipe, fork process, pass & print          */
	/* message based on command line arguments provided */
	/* ************************************************ */
	if(strcmp(pipeType, "u") == 0 || strcmp(pipeType, "U") == 0) {
		/* ******************* */
		/* UNNAMED PIPE OPTION */
		/* ******************* */
		// Create unnamed pipe using f_des as file descriptor
		if(pipe(f_des) == -1) { 
			// If fail..
			// Print error and exit with non-zero failure status
			perror ("Pipe"); 
			exit(2); 
		}

		if(strcmp(direction, "c") == 0 || strcmp(direction, "C") == 0) {
			/* *************** */
			/* UNNAMED PIPE    */
			/* CHILD -> PARENT */
			/* *************** */
			// Child writes a message to parent process
			// Fork the current process
			child = fork();
			switch (child) {
				case -1: 
					// If fail..
					// Print error and exit with non-zero failure status
					perror ("Fork"); 
					exit(3); 
				case 0: 
					// If child...
					// Close file descriptor f_des[0]
					close(f_des[0]);

					printf("child %ld is about to send the message [%s] to FIFO\n", (long)getpid(), message); 

					// Write BUFSIZE bytes from the message provided via command line
					// to the file descriptor f_des[1]
					if (write(f_des[1], message, strlen(message)) != -1) {
						// If success...
						// Print message that was written
						printf ("message sent\n"); 
						// Flush buffered data
						fflush(stdout); 
					} else { 
						// If fail...
						// Print error and exit non-zero failure status
						perror ("Write"); 
						exit(5); 
					}
					break; // Break from switch statement
				default: 
					// If parent...
					// Close file descriptor f_des[1]
					close(f_des[1]); 

					printf("parent is about to read the message from FIFO\n");

					// Read BUFSIZE bytes from file descriptor f_des[2]
					// And store into buffer[]
					if (read(f_des[0], buffer, BUFSIZE) != -1) { 
						// If success...
						// Reverse case of buffer
						reverseCase(buffer);
						// Print message that was read
						printf ("parent receives the message *%s* from child %d\n", buffer, child);
						// Flush buffered data 
						fflush(stdout); 
					} else { 
						// If fail..
						// Print error and exit non-zero failure status
						perror ("Read"); 
						exit(4);
					} 
					
			}
		} else {
			/* *************** */
			/* UNNAMED PIPE    */
			/* PARENT -> CHILD */
			/* *************** */
			// Parent writes a message to child process
			// Fork the current process
			child = fork();
			switch (child) {
				case -1: 
					// If fail..
					// Print error and exit with non-zero failure status
					perror ("Fork"); 
					exit(3); 
				case 0: 
					// If child...
					// Close file descriptor f_des[1]
					close(f_des[1]); 

					printf("child %ld is about to read the message from FIFO\n", (long)getpid());

					// Read BUFSIZE bytes from file descriptor f_des[2]
					// And store into buffer[]
					if (read(f_des[0], buffer, BUFSIZE) != -1) { 
						// If success...
						// Reverse case of message
						reverseCase(buffer);
						// Print message that was read
						printf ("child %ld receives the message *%s* from parent\n", (long)getpid(), buffer);
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

					printf("parent is about to send the message [%s] to FIFO\n", message); 

					// Write BUFSIZE bytes from the message provided via command line
					// to the file descriptor f_des[3]
					if (write(f_des[1], message, strlen(message)) != -1) {
						// If success...
						// Print message that was written
						printf ("message sent\n"); 
						// Flush buffered data
						fflush(stdout); 
					} else { 
						// If faile...
						// Print error and exit non-zero failure status
						perror ("Write"); 
						exit(5); 
					}
			}
		}
	} else {
		/* ***************** */
		/* NAMED PIPE OPTION */
		/* ***************** */
		// Create named pipe with user level read/write permissions
		if((mkfifo(pipeName,fifo_mode) == -1) && (errno != EEXIST)) { 
			// If fail..
			// Print error and exit with non-zero failure status
			perror ("Pipe"); 
			exit(1); 
		}

		if(strcmp(direction, "c") == 0 || strcmp(direction, "C") == 0) {
			/* *************** */
			/* NAMED PIPE      */
			/* CHILD -> PARENT */
			/* *************** */
			// Child writes a message to parent process
			// Fork the current process
			if((child = fork()) == -1) { 
				// If fail..
				// Print error and exit with non-zero failure status
				perror ("Fork"); 
				exit(1); 
			} else if(child == 0) { 
				// If child...
				// Print message indicating that the child is about to open 
				// named pipe, including child pid in message
				printf("child %ld is about to send the message [%s] to %s\n", (long)getpid(), message, pipeName );

				// Open named pipe  for writing only (O_WRONLY)
				if((fd = open(pipeName, O_WRONLY)) == -1) {
					// If fail..
					// Print error and exit with non-zero failure status
					perror("Child cannot open pipe"); 
					exit(1);
				} 

				// Get message size for use in write() system call
				strsize = strlen(message);

				// Write strsize bytes from message
				// to the file descriptor fd
				if(write(fd, message, strsize) != strsize) { 
					// If fail..
					// Print error and exit with non-zero failure status
					printf("Child write to pipe failed\n"); 
					exit(1); 
				} 
				// Print success message and child pid
				printf ("message sent\n");
			} else { 
				// If parent...
				// Print message indicating that the parent is about to open
				// named pipe, including parent pid in message
				printf ("parent is about to read the message from %s\n", pipeName);

				// Open non-blocking FIFO pipe named argv[2] for reading only (O_RDONLY)
				if((fd = open(pipeName, O_RDONLY | O_NONBLOCK)) == -1) { 
					// If fail..
					// Print error and exit with non-zero failure status
					perror("Parent cannot open pipe");
					exit(1); 
				}

				// Wait for child to finish writing
				while((wait(&status) == -1) && (errno == EINTR));

				// Once child has finished writing
				// Parent reads up to BUFSIZE bytes from file descriptor fd into buffer message
				if(read(fd, buffer, BUFSIZE) <=0) { 
					// If fail..
					// Print error and exit with non-zero failure status
					perror("Parent read from pipe failed\n");
					exit(1); 
				}
				
				// Reverse case of message
				reverseCase(buffer);

				// If read success...
				// Print message read by parent (message)
				printf ("parent receives the message *%s* from child %d\n", buffer, child);
			}
		} else {
			/* *************** */
			/* NAMED PIPE      */
			/* PARENT -> CHILD */
			/* *************** */
			// Parent writes a message to child process
			// Fork the current process
			if ((child = fork()) == -1) { 
				// If fail..
				// Print error and exit with non-zero failure status
				perror ("Fork"); 
				exit(1); 
			} else if (child == 0) { 
				// If child...
				// Print message indicating that the child is about to open
				// named pipe,  including child pid in message
				printf ("child %ld is about to read the message from %s\n", (long) getpid(), pipeName);

				// Open non-blocking pipe named argv[2] for reading only (O_RDONLY)
				if ((fd = open(pipeName, O_RDONLY | O_NONBLOCK)) == -1) { 
					// If fail..
					// Print error and exit with non-zero failure status
					perror("Child cannot open pipe");
					exit(1); 
				}

				// Wait for parent to finish writing
				while ((wait(&status) == -1) && (errno == EINTR));

				// Once parent has finished writing
				// Child reads up to BUFSIZE bytes from file descriptor fd into buffer message
				if (read(fd, buffer, BUFSIZE) <=0) { 
					// If fail..
					// Print error and exit with non-zero failure status
					perror("Child read from pipe failed\n");
					exit(1); 
				} 

				// Reverse case of message
				reverseCase(buffer);

				// Print message read by child (message)
				printf ("child %ld receives the message *%s* from parent\n", (long)getpid(), buffer);
			} else { 
				// If parent...
				// Print message indicating that the parent is about to open 
				// named pipe, including parent pid in message
				printf ("parent is about to send message [%s] to %s\n", message, pipeName);

				// Open FIFO named pipe for writing only (O_WRONLY)
				if ((fd = open(pipeName, O_WRONLY)) == -1) {
					// If fail..
					// Print error and exit with non-zero failure status
					perror("Parent cannot open pipe"); 
					exit(1);
				} 

				// Get message size for use in write() system call
				strsize = strlen(message);

				// Write strsize bytes from message
				// to the file descriptor fd
				if (write(fd, message, strsize) != strsize) { 
					// If fail..
					// Print error and exit with non-zero failure status
					printf("Parent write to pipe failed\n"); 
					exit(1); 
				} 
				// Print success message and parent pid
				printf ("message sent\n");
			}
		}
	}

	exit(0); // Program exits normally
}

// Reverses case of an array of characters
void reverseCase(char message[]) {
	
	unsigned size, i;
	
	size = strlen(message);
	
	for(i = 0; i < size; i++) {
		if(isalpha(message[i])){
			if(isupper(message[i])){
				message[i] = tolower(message[i]);
			} else {
				message[i] = toupper(message[i]);
			}
		}
	}

}
