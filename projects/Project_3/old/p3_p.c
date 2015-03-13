#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/stat.h>

#define MAX_ARG 10
#define MAX_CHILD 3

//function prototype
int file_exists (char *);


int main(int argc, char* argv[])
{
  int k = 0,w = 0, status, childNum, sleeptime, num_tries;
  pid_t pid;
  char *filename;
  
  //clear possible lock
  unlink("lock");
  //validation
  //check num args
  if(argc != 4)
  {
      printf("incorrect number of args\n");
      exit(1);
  }
  
  //check int type for argv[2] and argv[3]
  if( (sscanf(argv[2], "%i", &num_tries)) != 1 || (sscanf(argv[2], "%i", &sleeptime)) != 1 )
  {
	printf("argv[2] & argv[3] must be of type integer\n");
	exit(1);
  }
  //if valid, assign
  sleeptime = atoi(argv[2]);
  num_tries  = atoi(argv[3]);
  //check arg values
  if(sleeptime < 1 || sleeptime > MAX_ARG)
  {
	printf("argv[2] must be a positive integer less than or equal to 10\n");
	exit(1);
  }
  
  if(num_tries < 1 || num_tries > MAX_ARG)
  {
	printf("argv[3] must be a positive integer less than or equal to 10\n");
	exit(1);
  }
  
  //check that filename exists
  if(!(file_exists(argv[1])))
  {
      printf("%s does not exist\n",argv[1]);
      exit(1);
  }

  for (k = 0; k < MAX_CHILD; k++)
  {
  	//if child	
  	if((pid = fork()) == 0)
      {
      	//
         childNum = k + '1'; /* converted to option based 1 */
         execlp("./p3_c","p3_c",argv[1],argv[2],argv[3],&childNum, (char*)NULL);
         perror("exec() failed");
         exit(1);
      }
  }
  //wait for children
   while ((w = wait(&status)) && w != - 1)
   { 
      if (w != - 1)
      printf("Wait on PID: %d returns status of: %04X\n", w, status); 
   }
   exit(0);
}


int file_exists (char * fileName)
{
   FILE *file;
   //if file not null
   if(file = fopen(fileName, "r"))
   {
   	//close file handler
   	fclose(file);
   	//and return true
   	return 1;
   }
   //else return false
   return 0;   
}


