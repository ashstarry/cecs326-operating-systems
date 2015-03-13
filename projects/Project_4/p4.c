#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//create a memory location that can contain an int, a pointer to a struct of type semid_ds, or a pointer to an array
union semun {
	int val;
	struct semid_ds *buf;
	ushort *array;
};

int main(int argc, char *argv[])
{
	//ints for id, value, iterator, and user input
	int sem_id, sem_value, i, NS;
	
	//remove flag
	char remove_flag;
	
	//key type
	key_t ipc_key;
	
	//semid_ds is a system semaphore struct
	struct semid_ds sem_buf;
	
	//generate the array that the union semun will reference in the for loop
	
	//int sem_array[(argc - 3)] = {0};
	
	//temp_short
	int temp_short;
	union semun arg;
	
	//generate IPC key with a reference to current directory
	ipc_key = ftok(".",'S');
	
	//**VALIDATION**
	//check for minimum length
	if(argc < 4)
	{
		printf("Usage: %s <r | n> <number_of_semaphores n> <sem_value 1> ... <sem_value n>\n", argv[0]);
		exit(1);
	}

	//check for valid removal flag
	if(strcmp(argv[1],"r") != 0 && strcmp(argv[1],"n") != 0) 
	{
		printf("Error: first parameter must be either 'r' or 'n'\n");
		exit(1);
	}

	//validate NS (number of semaphores) before validating inputted values
	if( (sscanf(argv[2], "%i", &NS)) != 1)
  	{
		printf("Error: second argument must be an integer\n");
		exit(1);
  	}
  	if(NS < 1)
  	{
  		printf("Error: second argument must be a positive integer\n");
		exit(1);
  	}

  	//ensure correct number of arguments
  	if(argc - 3 != NS)
	{
  		printf("Error: Arguments not matched, number of sempahores is %d\n", NS);
		exit(1);
  	}

	//instantiate array of size NS to hold semaphores
  	ushort sem_array[NS];
  	
  	//validate semaphore value arguments
	//and build array of semaphore values
  	for(i = 3;i < argc; i++)
  	{
	  	//make sure arg is an int
	  	if( (sscanf(argv[i], "%i", &temp_short)) != 1 )
	  	{
			printf("Error: value arg not an integer\n");
			exit(1);
	  	}
	  	//make sure arg is positve
	  	if(temp_short < 0)
	  	{
			printf("Error: value arg less than 0\n");
			exit(1);
	  	}
	  	//assign int to array of values
	  	sem_array[i-3] = ((ushort)temp_short);
  	}
  	
	
	//**START BUSINESS LOGIC**
	//create the semaphore set, setting IPC_EXCL flag 
	//to ensure that the semaphore set isn't accessed if the
	//program is run again with the n flag
	if((sem_id = semget(ipc_key, NS, IPC_CREAT | IPC_EXCL | 0666))== -1)
	{
		perror("semget");
		exit(1);
	}

	//upon successful semget (semaphore creation), print the id
	printf("Semaphore identifier %d\n", sem_id);
	
	//Set arg (the union) to the address of the storage location for returned semid_ds value
	arg.buf = &sem_buf;

	//I assume this is here to ensure permission to the semid_ds structure of the created semaphore
	if(semctl(sem_id, 0, IPC_STAT, arg) == -1)
	{
		perror("semctl: IPC_STAT");
		exit(2);
	}
	
	//Had to add conversion of ctime to char*
	printf("Create %s\n", ctime(&sem_buf.sem_ctime));
	
	//Set arg (the union) to the address of the initializing vector 
	arg.array = sem_array;
	
	//set the values of the individual flags
	if(semctl(sem_id,0,SETALL, arg) == -1)
	{
		perror("semctl: SETALL\n");
		exit(3);
	} 
	
	//print the values of the individual flags
	for(i = 0; i < NS; ++i)
	{
		if((sem_value = semctl(sem_id, i, GETVAL, 0)) == -1)
		{
			perror("semctl: GETVAL\n");
			exit(4);
		}
		printf("Semaphore %d has a value of %u\n", i, sem_value);
	}
	
	//if remove option entered, try to remove the sempahore set
	//before creating another with the same IPC key
	if(strcmp(argv[1], "r") == 0) {
		semctl(sem_id, 0, IPC_RMID, 0);
	}
//end main
}
