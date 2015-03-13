#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#define MAX_LVL 5

//Program creates binary tree of forked processes and prints the id of the current process, its parent's, its children's, and the current generation to the console
int main(int argc, char *argv[])
{
	//declare variables
	int current_level, max_level, status_ptr;
	pid_t lchildpid, rchildpid, parentpid;
	
	//validate arg amount
	if(argc != 2) 
	{
		printf("\nUsage: %s [max gen] Incorrect number of arguments.\n\n", argv[0]);
		exit(1);		
	}
	
	//cast arg	
	max_level = atoi(argv[1]);
	
	//validate arg content
	if(max_level < 1 || max_level > MAX_LVL) 
	{
		printf("\nUsage: %s [max levels]\n[max levels] must be a positve integer less than %d\n\n", argv[0], MAX_LVL);
		exit(1);		
	}
	
	//initialize children
	lchildpid = 0;
	rchildpid = 0;
	
	
	//Print out the header
	printf("\nLvl\tProc\tParent\tChild 1\tChild 2\n");
	printf("No.\tID\tID\tID\tID\n");
    
	//generate binary process tree
	while(current_level < max_level)
	{	
		//If child, gather parent id, iterate, then continue
		if((lchildpid = fork()) == 0 || (rchildpid = fork()) == 0) {
			parentpid = getppid();
			current_level++;
			continue;
		}
		
		//check for forking errors
		if(lchildpid == -1 || rchildpid == -1) 
		{
			perror("\n The fork failed\n");
			exit(1);
		}
		
		//If current fork has two children, print complete generation to console and exit while loop
		if(lchildpid && rchildpid){
			printf("%d\t%ld\t%ld\t%ld\t%ld\n", current_level, (long)getpid(), (long)getppid(), (long)lchildpid, (long)rchildpid);
			break;
		}	
	}
	//wait for children to end to ensure valid getppid() calls
	waitpid(lchildpid, &status_ptr, 0);
   	waitpid(rchildpid, &status_ptr, 0);
	exit(0);

	
//end main
}
