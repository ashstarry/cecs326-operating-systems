/*
* PROGRAM: lab4a.c 
* 
* This program executes 10 fork() statements in succession, if the fork is successful the child process 
* makes an exec call to the echo command passing an element from an array holding numbers [0,9]
* (this is done in order from array index 0 to 9). The parent process does nothing and continues through 
* the for loop in main() (thus creating more children). At the end of the program, the process sleeps for 2 seconds.
*
* If the fork fails, a 'Fork failure' message is thrown and the program continues. 
* If the child processes exec call to the echo command fails an 'Exec failure' message is thrown and the 
* process continues (thus continuing through the for loop and creating children of children...and so on). 
*
*/
# include <stdio.h> 
# include <unistd.h> 

/*
* 
* Function: main
* ---------------
*
* Iterates through array of integers [0, 9] (as char's) 
* and call display function for each element in the array
* Sleeps for 2 seconds after iterating through the array. 
*
*/
void main()
{
	// declare variables, assign values to mesg[] array
	static char *mesg[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
	int display(char *), i;
	
	// iterate through the mesg[] array and call display(element) for each element of msg[]
	for (i=0; i<10; ++i) 
		display(mesg[i]);
	sleep(2); 
}

/*
*
* Function: display
* -----------------
* Forks the current process creating a child process that executes the echo command with 
* the given character as the argument, the parent process returns 0.
*
* Prints an error message if the exec fails, and returns 1
* Prints an error message if the fork fails, and returns 2 
*
* m: the character to echo 
*
* returns: 1 if the call to exec fails
*	       2 if the fork fails
*	       0 if the parent
*
*/
int display(char *m) {
	char err_msg[25]; 
	switch (fork())
	{	
		case 0:
			// if child: run the echo program via the execlp command passing m as argv
			execlp("/bin/echo", "echo", m, (char *) NULL); 
			// if exec fails: print failure message and return 1
			printf(err_msg, "%s Exec failure", m); 
			perror(err_msg); 
			return(1);
		case -1:
			// if fork fails: print failure message and return 2
			perror ("Fork failure");
			return(2);
		default: 
			// if parent: return 0
			return(0);
	}
}
