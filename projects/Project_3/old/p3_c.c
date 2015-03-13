#include <stdio.h> 
#include <stdlib.h>
#include <fcntl.h> 
#include <errno.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

int main(int argc, char* argv[])
{
   
   int tries = 0,num_tries,lock_try,sleeptime, childNum;
   pid_t pid = getpid();
   char* filename = argv[1];
   char cmd[20] = "cat "; 
   
   //seed random for sleeptime modulo
   srand((unsigned int)getpid());
   
   //while unsuccessful in creating lock
   while(lock_try = creat("lock",0) == -1 && errno == EACCES)
   {
      //if still have tries
      if(++tries < num_tries)
      {
         sleep(rand()%sleeptime);
      }
      //else if run out of tries
      else
      {
         printf("Child%d failed to acquire lock file.\n",childNum);
         kill(pid,childNum);
      }
   }
   
   
   //concatenate 'cat' and filiename and pass it to system command for execution in the environment
   strcat(cmd, filename);
   system(cmd);
   
   //clean up lock
   unlink("lock");
   //exit passing lower bits using bitwise operation 
   exit(getpid() & 0xFF);
   return 0;
}
