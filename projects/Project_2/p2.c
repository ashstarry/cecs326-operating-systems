#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#define MAX_SLEEP 50

int main(int argc, char *argv[])
{
	//validation
	if(argc != 4) 
	{
		printf("Usage: %s [sleep-time] [operand-1] [operand-2]", argv[0]);
		exit(1);		
	}
	
	//hold arg values
	int t, a, b;
	
	//hold child id values
	pid_t firstChild, secondChild, thirdChild, fourthChild;
	
	/* Error check the command line arguments */
	/* If they’re all valid initialize */
	if (sscanf (argv[1], "%i", &t)!=1) {
        	printf("Argument Error: The sleep argument is not an integer\n");
		printf("Usage: %s [sleep-time] [operand-1] [operand-2]", argv[0]);
        	exit(1); 
	}
	if (sscanf (argv[2], "%i", &a)!=1) {
        	printf("Argument Error: The integer a argument is not an integer\n");
		printf("Usage: %s [sleep-time] [operand-1] [operand-2]", argv[0]);
        	exit(1); 
	}
	if (sscanf (argv[3], "%i", &b)!=1) {
        	printf("Argument Error: The integer b argument is not an integer\n");
		printf("Usage: %s [sleep-time] [operand-1] [operand-2]", argv[0]);
        	exit(1); 
	}

	//Make sure t is within allowable range
	if(t < 0 || t > MAX_SLEEP)
	{
		printf("\nBad Parameter: sleep-time must  be a positive integer less than %d.\n", MAX_SLEEP);
		printf("Usage: %s [sleep-time] [operand-1] [operand-2]", argv[0]);	
		exit(1);
	}
	
	//intialize process args
	firstChild = 0;
	secondChild = 0;
	thirdChild = 0;
	fourthChild = 0;
	
	//begin functional requirement
	printf("I am the parent process, the maximum sleep time is %d and the two numbers are %d and %d.\n", t,a,b);

	//ensure parent process check and fork firstChild
	if((firstChild = fork()) == 0){
		//in child process
		//seed random
		srand(time(NULL) + getpid());
		int sleepy = rand() % t;
		printf("child 0 is sleepy for %d seconds\n", sleepy);
		sleep(sleepy);
		printf("I am child number 0 with PID %d, the sum is %d\n",getpid(),a + b);
	}
	else
	{
		//in parent process
		printf("Forked child %d\n", firstChild);
	}
	
	//ensure the parent process
	if(firstChild && (secondChild = fork()) == 0){
		//in child process
		//seed random
		srand(time(NULL) + getpid());
		int sleepy = rand() % t;
		printf("child 1 is sleepy for %d seconds\n", sleepy);
		sleep(sleepy);
		printf("I am child number 1 with PID %d, the difference is %d\n",getpid(),a - b);
	}
	else if(firstChild)
	{
		//in parent process
		printf("Forked child %d\n", secondChild);
	}
	
	//ensure the parent process
	if(firstChild && secondChild && (thirdChild = fork()) == 0){
		//in child process
		//seed random
		srand(time(NULL) + getpid());
		int sleepy = rand() % t;
		printf("child 2 is sleepy for %d seconds\n", sleepy);
		sleep(sleepy);
		printf("I am child number 2 with PID %d, the quotient is %d with a remainder of %d\n",getpid(),a/b,a%b);
	}
	else if(firstChild && secondChild)
	{
		//in parent process
		printf("Forked child %d\n", thirdChild);
	}	
	
	//ensure the parent process
	if(firstChild && secondChild && thirdChild && (fourthChild = fork()) == 0){
		//in child process
		//seed random
		srand(time(NULL) + getpid());
		int sleepy = rand() % t;
		printf("child 3 is sleepy for %d seconds\n", sleepy);
		sleep(sleepy);
		printf("I am child number 3 with PID %d, the product is %d\n",getpid(),a*b);
	}
	else if(firstChild && secondChild && thirdChild)
	{
		//in parent process
		printf("Forked child %d\n", fourthChild);
	}	
	
	
	wait();	
	return(0);
}
