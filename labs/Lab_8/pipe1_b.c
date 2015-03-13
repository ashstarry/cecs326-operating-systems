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

/* 
* Unnamed pipe pipe1.c
*  
* Usage: ./pipe1.out pipename
* 
* Parent writes message to child
*  
*/ 
void main(int argc, char *argv[]) {
	// Declare variables
	mode_t fifo_mode = S_IRUSR | S_IWUSR; 
	int fd, status, child; 
	char buf[BUFSIZE]; 
	unsigned strsize;

	// Validate arg count
	if (argc !=2) { 
		printf ("\n Usage: %s pipename\n", argv[0]); 
		exit(1); 
	}

	// Create named pipe with user level read/write permissions
	if ((mkfifo(argv[1],fifo_mode) == -1) && (errno != EEXIST)) { 
		// If fail..
		// Print error and exit with non-zero failure status
		perror ("Pipe"); 
		exit(1); 
	}

	// Fork the current process
	if ((child = fork()) == -1) { 
		// If fail..
		// Print error and exit with non-zero failure status
		perror ("Fork"); 
		exit(1); 
	} else if (child == 0) { 
		// If child...
		// Print message indicating that the child is about to open
		// FIFO pipe named argv[1], including child pid in message
		printf ("Child %ld is about to open FIFO %s\n", (long) getpid(), argv[1]);

		// Open non-blocking FIFO pipe named argv[1] for reading only (O_RDONLY)
		if ((fd = open(argv[1], O_RDONLY | O_NONBLOCK)) == -1) { 
			// If fail..
			// Print error and exit with non-zero failure status
			perror("Child cannot open FIFO");
			exit(1); 
		}

		// Print message indicating that the child is about to read, and print child pid
		printf ("Child %ld is about to read\n", (long)getpid());

		// Wait for parent to finish writing
		while ((wait(&status) == -1) && (errno == EINTR));

		// Once parent has finished writing
		// Child reads up to BUFSIZE bytes from file descriptor fd into buffer buf
		if (read(fd, buf, BUFSIZE) <=0) { 
			// If fail..
			// Print error and exit with non-zero failure status
			perror("Child read from FIFO failed\n");
			exit(1); 
		} 

		// If read success...
		// Print message read by child (buf)
		printf ("Child %ld received: %s\n", (long)getpid(), buf);
	} else { 
		// If parent...
		// Print message indicating that the parent is about to open 
		// FIFO pipe named argv[1], including parent pid in message
		printf ("\n Parent %ld is about to open FIFO %s\n", (long)getpid(), argv[1]);

		// Open FIFO pipe named argv[1] for writing only (O_WRONLY)
		if ((fd = open(argv[1], O_WRONLY)) == -1) {
			// If fail..
			// Print error and exit with non-zero failure status
			perror("Parent cannot open FIFO"); 
			exit(1);
		} 

		// Store formatted message "This was written by parent [parent pid]"
		// into buffer buf[]
		sprintf (buf, "This was written by parent %ld\n", (long)getpid()); 

		// Get buf[] size for use in write() system call
		strsize = strlen(buf) + 1;

		// Write strsize bytes from buf (message generated by the preceeding sprintf() call)
		// to the file descriptor fd
		if (write(fd, buf, strsize) != strsize) { 
			// If fail..
			// Print error and exit with non-zero failure status
			printf("Parent write to FIFO failed\n"); 
			exit(1); 
		} 
		// Print success message and parent pid
		printf ("Parent %ld is done\n", (long)getpid());
	}
	exit(0); // Program exits normally
}