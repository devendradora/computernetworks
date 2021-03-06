#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<signal.h>
#include<sys/shm.h>
#include<sys/sem.h>


int fd;
pid_t pid, *shm_ptr;
int sem_id;

void P(int sem_id)
{
   struct sembuf buf;
   buf.sem_op = -1;
   buf.sem_flg = 0;
   buf.sem_num = 0;

   semop(sem_id, &buf , 1);
}

void V(int sem_id)
{
   struct sembuf buf;
   buf.sem_op = 1;
   buf.sem_flg = 0;
   buf.sem_num = 0;

   semop(sem_id, &buf, 1);
}

void user_sig_handler2(int signal)
{
   if(signal == SIGUSR2)
   {
  printf("\n\n ----- Read By Process 2 :  -------- \n\n");
      char buf[1];
    int lines = 0;
     while(lines < 5)
    {
       int size = read(0,buf,1);
      if(size == 0)
      exit(1);
      if(buf[0] == '\n')
      lines++;
      write(1,buf,1);
    }
      P(sem_id);
      kill(*shm_ptr,SIGUSR1);
*shm_ptr = pid; 
      V(sem_id);
      sleep(1);
      
      
   }
}

int main()
{

int shm_id;
pid_t key = ftok(".",'z');
pid = getpid();
  if((shm_id = shmget(key,sizeof(pid_t), IPC_CREAT | 0666)) == -1)
  {
    printf("shared memory error");
    perror("shmget");
    exit(1);
  }

if((sem_id = semget(key,1, IPC_CREAT | 0666)) == -1)
  {
    printf("semaphore error");
    perror("semget");
    exit(1);
  }
   


semctl(sem_id,0,SETVAL,1);
   shm_ptr = (pid_t*)shmat(shm_id,NULL,0);
   
  signal(SIGUSR2,user_sig_handler2);
 P(sem_id);
      kill(*shm_ptr,SIGUSR1);
*shm_ptr = pid; 
      V(sem_id);

  while(1)
    sleep(1);
   
 
}
