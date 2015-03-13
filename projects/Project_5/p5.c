/* 
* 	COURSE:			CECS 326
*	SECTION:		TR 7:30pm
*	INSTRUCTOR:		Haney Williams
*	ASSIGNMENT:		Project 5 
*	GROUP:			Ariel Katz
*					Michael Williams
*					Kristin Peterson
*
*	USAGE:
*	./p5.out [N - number of child processes] [s/n - use semaphores?] [delay]
*
*	This program creates N child processes each of which access a critical
* 	section to store information about themself.  If the s option is chosen
* 	semaphores will be utilized to manage the critical section where this 
* 	information is stored.  If the n option is chosen, semaphores WILL NOT
* 	be used, and the printing of process information will appear out of order.
*	The delay option determines how much of a delay there will be between
* 	each character print.
*
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <limits.h>
#include <errno.h>

// Create a memory location that can contain an int, 
// a pointer to a struct of type semid_ds, and/or a pointer to an array
union semun {
	int val;
	struct semid_ds *buf;
	ushort *array;
};

// Prototype for creation of semwait & semsignal sembufs
void set_sembuf_struct(struct sembuf *s, int num, int op, int flg)
{
	s-> sem_num = (short) num;
	s-> sem_op = op;
	s-> sem_flg = flg;
	return;
} 

int main(int argc, char *argv[])
{
	/*
	* children:		argv[1], the number of child processes to create in range (1, 10)
	* semaphore:	utilized as boolean to indicate whether semaphore protection should be used
	*				if argv[2] == s, semaphore protection will be used, boolean value == 1
	*				else if argv[2] == n, semaphore protection will not be used, boolean value == 0
	* delay:		argv[3], delay adjustment parameter
	* i: 			iterator for for child creation loop
	* sem_id: 		the semaphore set identifier
	* semop_ret: 	holds the semop() method return value 	
	* counter:		outer loop counter for delay
	* counter2:		inner loop counter for delay
	* status:		waitpid status holder
	*
	*/
	int children, semaphore, delay, i, sem_id, semop_ret, sem_input, counter, counter2, status;
	// Holds childpid
	pid_t childpid;
	// The original parent's pid
	pid_t parent = getpid();
	// Holds buffer array, and pointer to print char by char
	char buff[1000], *c_ptr;

	// Assign pointer to memory location of zero index of buffer
	c_ptr = buff;

	// Implement wait and signal
	struct sembuf semwait[1]; 
	struct sembuf semsignal[1];

	// Initialize semaphore element to 1
	set_sembuf_struct(semwait, 0, -1, 0); 
	set_sembuf_struct(semsignal, 0, 1, 0);

	/*
	* Stores the (unique) IPC key
	* that is used to identify a semaphore set
	*
	*/
	key_t ipc_key;

	/*
	* The address to this structure will later be
	* assigned to the union arg.buf attribute,
	* it is a buffer for IPC_STAT, IPC_SET control operations
	*
	*/
	struct semid_ds sem_buf;

	// Holds values utilized by semctl() method
	union semun arg;

	// Generate IPC key with a reference to current directory
	ipc_key = ftok(".",'S');
	
	//**VALIDATION**
	// Check for valid number of arguments
	if(argc < 4)
	{
		printf("Usage: %s <number_of_children n> <n | s> <delay>\n", argv[0]);
		exit(1);
	}
	// Validate option argument (argv[2])
	if(strcmp(argv[2],"s") != 0 && strcmp(argv[2],"S") != 0 && strcmp(argv[2],"N") != 0 && strcmp(argv[2],"n") != 0) 
	{
		printf("Error: second parameter must be either 's' or 'n'\n");
		exit(1);
	} else {
		semaphore = (strcmp(argv[2],"s") == 0 || strcmp(argv[2],"S") == 0);
	}
	
	// Validate child count argument (N, argv[1]) and delay (argv[3])
	if( (sscanf(argv[1], "%i", &children)) != 1 || (sscanf(argv[3], "%i", &delay) != 1) )
  	{
		printf("Error: children and delay arguments must be integers\n");
		exit(1);
  	}
  	if(children < 1 || children > 10)
  	{
  		printf("Error: number of children must between 1 and 10\n");
		exit(1);
  	}
  	if(delay < 1 || delay > 300)
  	{
  		printf("Error: Delay should be between 1 and 300\n");
		exit(1);
  	}
	
	if(semaphore) { 
		// If semaphore option chosen	
		/* 
		* Try to create semaphore set containing one semaphore to manage
		* the critical section, with permission `0666` 
		* (user/group/world ea. with read & write permissions)
		*
		* If the call to semget() fails, indicated by a return value of -1
		* error is printed and the program exits with exit code 1.
		*
		* Note: execute permissions are not meaningful for semaphores
		*
		*/
		if ((sem_id = semget(ipc_key, 1, IPC_CREAT | 0666)) == -1) {
			perror ("semget: IPC | 0666");
			exit(1); 
		}

		// Increment semaphore - V(s)
		if (semop(sem_id, semsignal, 1) == -1) {
			printf ("%ld: semaphore increment failed - %s\n", (long)getpid(), strerror(errno));
			if (semctl(sem_id, 0, IPC_RMID) == -1)
				printf ("%ld: could not delete semaphore - %s\n", (long)getpid(), strerror(errno));
			exit(1); 
		}
	}

	// Create children
	for (i=1; i<children; i++){
		if(childpid = fork())
			break;	// If parent, break from loop
	}

	if(semaphore) { 
		// If semaphore option chosen	
		// Decriment semaphore - P(s)
		while (((semop_ret = semop(sem_id, semwait, 1) ) == -1) && (errno ==EINTR)); 
			if (semop_ret == -1)
				printf ("%ld: semaphore decrement failed - %s\n", (long)getpid(), strerror(errno));	
	}

	// Add message to buffer
	sprintf(buff,"i: %d process ID: %6ld parent ID: %6ld child ID: %6ld\n", i, (long)getpid(), (long)getppid(), (long)childpid);

	// Print character by characer, with delay between each character print
	while (*c_ptr != '\0'){
		fputc(*c_ptr, stderr);
		// Sleeps based off delay argument
		// This method used to avoid context switch
		for(counter = 0; counter<delay; counter++){
			for(counter2=0; counter < 50000000; counter++){
			}
		}
		c_ptr++;				
	}
	fputc('\n', stderr);

	if(semaphore) { 
		// If semaphore option chosen	
		// Increment semaphore - V(s)
		if (semop(sem_id, semsignal, 1) == -1) {
			printf ("%ld: semaphore increment failed - %s\n", (long)getpid(), strerror(errno));
			if (semctl(sem_id, 0, IPC_RMID) == -1)
				printf ("%ld: could not delete semaphore - %s\n", (long)getpid(), strerror(errno));
			exit(1); 
		}
	}

	// Wait for child process(es)
	waitpid(childpid, &status, 0);

	if(semaphore && parent == getpid()) { 
		// If semaphore option chosen & this is the original parent process, 
		// try to remove semaphore set
	    if(semctl(sem_id, 0, IPC_RMID) == -1) {
            printf("%ld: couldn't delete semaphore - %s\n", (long)getpid(), strerror(errno));
            exit(1);
        }
    }
	
//end main
}
