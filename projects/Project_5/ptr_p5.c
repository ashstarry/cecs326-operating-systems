# include <stdio.h>
# include <stdlib.h>
# include <ctype.h>
# include <string.h>
# include <errno.h>
# include <sys/sem.h> 

//function prototypes 
void enter(int,int);          
void leave(int,int);
void printUsage();
//
struct sembuf release; //used for exit op
struct sembuf acquire; //used for enter op

int main(int argc, char* argv[])
{
   pid_t pid = 0,first_process;
   key_t sem_key;
   first_process = getpid();
   int n,k,sem_id,child_num,w,status,i;
   char opt;
   //buffer to print
   char buffer[100];
   /*****************************/
   /*TODO do validation for args*/
   /*****************************/
   n = atoi(argv[1]);
   opt = *argv[2];
   k = atoi(argv[3]);
   //generate key
   if((sem_key = ftok("/tmp", 'a')) == (key_t)-1)
   {
      perror("fail key: ");
      exit(1);
   }
   
   // create a semaphore set with n flags
   if((sem_id = semget(sem_key, n, 0666 | IPC_CREAT)) == -1)
   {
      perror("fail semget: ");
      exit(1);
   }
   //array for sem values
   int val[n];
   //initialize val to zero
   for(i = 0; i < n; i++)
   {
      val[i] = 0;
   }
   //inc first semaphore
   val[0] = 1;
   //set semaphore flags to val[]
   if(semctl(sem_id,0,SETALL,val) == -1)
   {
      perror("semctl failed: ");
      exit(1);
   }
   //fork children
   for(child_num = 0; child_num < n - 1; child_num++)
   {
      if(pid = fork())
      {
         break;
      }
   }
   //fill string buffer
   sprintf(buffer,"ChildNum = %d, Process_id = %d, Parent_id = %d, Child_id = %d\n",child_num,getpid(),getppid(),pid);
   //seed with different pid
   srand(getpid());
   //get random sleep
   int sleeptime = rand()%k;
   
   //check if mutex for crit section
   if (opt == 's') enter(sem_id,child_num);
   
   for(i = 0; i < strlen(buffer); i++)
   {
      //usleep is sleep in microseconds
      usleep(sleeptime);
      //array/pointer arithmetic to increment buffer
      //first arg 1 refers to stdout
      write(1,buffer+i,1);
   }
   // leaving critical section
   /* last child will have child_num == N-1, there is no next process */ 
   if ((opt == 's') && (child_num != n-1 )) leave(sem_id,child_num+1);

   //have process wait for child
   while ((w = wait(&status)) != -1);
   
   //first process does semaphore clean up
   if(getpid() == first_process)
   {
      if(semctl(sem_id, 0, IPC_RMID, 0) == -1) 
      {
         perror ("semctl: IPC_RMID"); 
         exit(1);
      }
   }

   return 0;
   
}

void leave(int id,int sem_num)
{
   release.sem_num = sem_num;
   release.sem_op = 1;
   release.sem_flg = 0;
   if((semop(id,(&release),1)) == -1)
   {
      printf ("%ld: semaphore leave failed - %s\n", (long)getpid(), strerror(errno));
      exit(1);
   }

}
//block 
void enter(int id,int sem_num)
{
   acquire.sem_num = sem_num;
   acquire.sem_op = -1;
   acquire.sem_flg = 0;
   if((semop(id,(&acquire),1)) == -1)
   {
      printf ("%ld: semaphore enter failed - %s\n", (long)getpid(), strerror(errno));
      exit(1);
   }
}


