#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

/* 
* PROGRAM: project2-child.c
* 
* Usage: ./project2-child.out [sleep-time] [operand-1] [operand-2] [child-num]

* Sleeps for a random time modulo sleep-time, then prints to the console
* the child number, it's PID, and an arithmatic operation based on child number
* if child number is not in range [0,3] prints message warning that there are 
* no operations assigned to that child number. If the second operand is zero,
* a divide-by-zero error message is printed when child number = 2 (division).
*
* child number 0: operand 1 (argv[2]) + operand 2 (argv[3]) = sum
* child number 1: operand 1 (argv[2]) - operand 2 (argv[3]) = difference
* child number 2: operand 1 (argv[2]) / operand 2 (argv[3]) = quotient 
*		& operand 1 (argv[2]) % operand 2 (argv[3]) = remainder
* child number 3: operand 1 (argv[2]) * operand 2 (argv[3]) = product
*
*
*/
int main(int argc, char *argv[])
{
	// Declare variables
	int child_num, sleep_time, op1, op2;

	// Validate arg amount
	if(argc != 5) 
	{
		printf("\nERROR: Incorrect number of arguments\nUsage: %s [sleep-time] [operand-1] [operand-2] [child-num]\n\n", argv[0]);
		exit(1);		
	}

	/* Error check the command line arguments */
	/* If they’re all valid initialize */
	if( (sscanf(argv[1], "%i", &sleep_time) != 1)
	    || (sscanf(argv[2], "%i", &op1) != 1)
	    || (sscanf(argv[3], "%i", &op2) != 1)
	    || (sscanf(argv[4], "%i", &child_num) != 1)) {
        	printf("Argument Error: All arguments must be integers\n");
			printf("Usage: %s [sleep-time] [operand-1] [operand-2]", argv[0]);
        	exit(1); 
	}	
	
	// Initialize Random Number Generator (using pid as seed)
	srand(getpid());
	
	// Randomizing sleep time
	sleep_time = rand() % sleep_time;

	// Child sleeps for random sleep_time
	sleep(sleep_time);

	/* Print out the child number, PID and it's associated arithmetic operation */
	switch(child_num) {
		case 0:
			printf("I am child number %i with PID %ld, the sum is %i\n", child_num, (long)getpid(), op1 + op2);
			break;
		case 1:
			printf("I am child number %i with PID %ld, the difference is %i\n", child_num, (long)getpid(), op1 - op2);
			break;
		case 2:
			// Handle divide by zero error
			if(op2 != 0) {
				printf("I am child number %i with PID %ld, the quotient is %i with a remainder of %i\n", child_num, (long)getpid(), op1 / op2, op1 % op2);
			} else {
				printf("ERROR: Cannot divide by 0\n");
			}
			break;
		case 3:
			printf("I am child number %i with PID %ld, the product is %i\n", child_num, (long)getpid(), op1 * op2);
			break;
		default:
			printf("There are no assigned operations for child number %d", child_num);
			break;
	}	
//end main
}
