# include <stdio.h>
# include <sys/types.h> 
# include <sys/ipc.h>
# include <sys/sem.h> 
# include <unistd.h>
# include <stdlib.h>
# include <time.h>
# define NS 3

union semun {
	int val; 
	struct semid_ds *buf; 
	ushort *array;
};

int main(void) {
	/*
	* sem_id: the semaphore set identifier
	* sem_value: the semaphore value:
	* 		set will be iterated over and each
	*		semaphore value will be temporarily
	*		stored in this variable for printing
	* i: iterator for for loop
	*
	*/
	int sem_id, sem_value, i; 

	/*
	* Will store the (unique) IPC key
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

	/*
	* This array will later be assigned to the union
	* arg.array address pointer, and is utilized in GETALL
	* and SETALL operations
	*
	*/
	static ushort sem_array[NS] = {3, 1, 4}; 

	// Holds values utilized by semctl() method
	union semun arg;

	// Generate IPC key using "." as path and "S" as project_id
	ipc_key = ftok(".", 'S');
	
	/* 
	* Create semaphore:
	* Try to create semaphore set of size NS (3)
	* with permissions `0666`: 
	* user/group/world ea. with read & write permissions
	*
	* If the call to semget(), indicated by a return value of -1
	* error is printed and the program exits with exit code 1.
	*
	* Note: execute permissions are not meaningful for semaphores
	*
	*/
	if ((sem_id = semget(ipc_key, NS, IPC_CREAT | 0666)) == -1) {
		perror ("semget: IPC | 0666");
		exit(1); 
	}
	
	printf ("Semaphore identifier %d\n", sem_id);
	
	/* 
	* Set arg (the union) to the address of the storage location for
	* returned semid_ds value 
	*/
	arg.buf = &sem_buf;
	
	/*
	* Initiate the control operation IPC_STAT on the semaphore set sem_id. 
	* 
	* If the IPC_STAT operation is successful, the kernel
	* data structure associated with sem_id will be copied to the
	* semid_ds data structure pointed to by arg.buf
	*
	* If the IPC_STAT operation fails, indicated by a return value of -1
	* error is printed and program exits with exit code 2
	* 
	* Note: with the IPC_STAT command the argument
	* semnum (0 in this case) is ignored, and the calling process (this)
	* must have read permissions on the semaphore set.
	*
	*/
	if (semctl(sem_id, 0, IPC_STAT, arg) == -1) {
		perror ("semctl: IPC_STAT");
		exit(2); 
	}
	
	/*
	* Print the last change time (in this case the creation time)
	* of the semaphore set
	*
	*/
	printf ("Create %s", ctime(&sem_buf.sem_ctime));
	
	// Set arg (the union) to the address of the initializing vector
	arg.array = sem_array;
	
	/*
	* Initiate the control operation SETALL on the semaphore set sem_id.
	*
	* If the SETALL operation is successful, sets the value for each semaphore
	* in the set sem_id (starting with 0) to the values in arg.array.
	*
	* If the SETALL operation fails, indicated by a return value of -1
	* error is printed and program exits with exit code 3
	* 
	*/
	if (semctl(sem_id, 0, SETALL, arg) == -1) { 
		perror("semctl: SETALL");
		exit(3);
	}
	
	/*
	* Loop through the semaphore set sem_id (starting with semaphore 0)
	* and get the value of semaphore number i.  
	*
	* If the call to semctl is successful, print the semaphore number and value
	*
	* If the call to semctl fails, print error and exit program with error code 4.
	*
	* Note: the calling process (this) must have read permission on the semaphore set
	*/
	for (i=0; i<NS; ++i) {
		if ((sem_value = semctl(sem_id, i, GETVAL, 0)) == -1) { 
			perror("semctl : GETVAL");
			exit(4);
		}
		printf ("Semaphore %d has value of %d\n",i, sem_value);
	}
}
