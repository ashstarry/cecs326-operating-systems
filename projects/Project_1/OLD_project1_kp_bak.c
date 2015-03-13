/**************************************************************************/
/* PROGRAM: project1.c */
/* DESCRIPTION:
/* This program creates a binary process tree and outputs
/* each processes level, pid, parent pid, first child pid
/* and second child pid.
/**************************************************************************/
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <unistd.h>

pid_t valid_fork(void);
void validate_args(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    /* Declare variables */
    int current_level, max_levels, user_input, status_ptr;
    pid_t first_childpid;
    pid_t second_childpid;
	
    /* validate command line arguments */
    validate_args(argc, argv);
    
    /* initialize variables */
    user_input = atoi(argv[1]);
    max_levels = 5; 

    /* if user_input > max_levels, set to max_levels */
    if (user_input > max_levels) {
        printf("\n\n*************************************************************\n\n");
        printf("Changing number of levels from %d to 5 (for your safety)", user_input);
        printf("\n\n*************************************************************\n\n");
        user_input = max_levels;
    }

    /* Print out the header */
    printf("\nLvl\tProc\tParent\tChild 1\tChild 2\n");
    printf("No.\tID\tID\tID\tID\n");
	
    /* Create binary process tree */
    for (current_level = 0; current_level <= user_input; current_level++) { 
        if ((first_childpid = valid_fork()) != 0) {
            // Parent - first child has been created
            if (first_childpid != 0) {
                // Parent - second child has been created
                second_childpid = valid_fork();
            }
        }
        if (first_childpid > 0 && second_childpid > 0) {
            // Parent - print out information and break from loop
            // as parent is only supposed to create 2 children and then stop forking
            printf("%d\t%ld\t%ld\t%ld\t%ld\n", current_level, (long)getpid(), (long)getppid(), (long)first_childpid, (long)second_childpid);
            break;
        }
    }
	
    /* 
    All parents must waits for each child to complete processing before exiting
    */
    waitpid(first_childpid, &status_ptr, 0);
    waitpid(second_childpid, &status_ptr, 0);
    exit(0);
}

/*
* Fork validator function
*/
pid_t valid_fork(void)
{
    pid_t pid;
    if ((pid = fork()) < 0)
    {
        perror ("\n The fork failed\n");
        exit(1);
    }
    return pid;
}

/*
* Function: valdiate_args
* -----------------------
* Validates the command line arguments, exits program with warning if
* arguments are invalid.
*
* argc: number of command line arguments entered
* argv: array containing command line arguments
*/
void validate_args(int argc, char *argv[])
{
    /* Confirm that only one command line argument is given */
    if(argc != 2) {
        printf("Usage: ./project1 <max-levels>\n");
	exit(1);
    }
	
    int user_input = atoi(argv[1]);	

    /* Exit program if input is <= 0 (ie. non-integer input, zero, or negative values) */
    if(user_input < 0) {
	printf("Usage ./project1 <max-levels>\n");
        printf("Erroneous input, max-levels must be > 0\n");
        exit(1);
    }
}	
