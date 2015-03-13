#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void main(int argc, char *argv[]) {
	
	if (argc > 1) {
		execlp("/bin/cat", "cat", argv[1], (char *) NULL); 
		perror("exec failure");
		exit(1);
	}

	printf("\n Usage: %s text_file\n", *argv);
	exit(1);
}
