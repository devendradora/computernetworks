#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/shm.h>
#include<sys/sem.h>

#define X_KEY 999
#define Y_KEY 888

pid_t pid, *shm_ptr;
void user_sig_handler1(int signal);
int sem_id;

int *X,*Y ; 


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


int init_shm(int key, int size , int flg)
{
  if((shmget(key,size, flg)) == -1)
  {
    printf("shared memory error");
    perror("shmget");
    exit(1);
  }
}

void create_shared_objects()
{
   int XID = init_shm(X_KEY, 1, IPC_CREAT | 0666);
   int YID = init_shm(Y_KEY, 1 , IPC_CREAT | 0666);
   X = (int*)shmat(XID, NULL, 0);
   Y = (int*)shmat(YID, NULL, 0);
   *X = 0;
   *Y = 0;
}



int  main()
{

  
int shm_id;
pid_t key = ftok(".",'v');
pid = getpid();
  
shm_id = init_shm(key, sizeof(pid_t) , IPC_CREAT | 0666);
if((sem_id = semget(key,1, IPC_CREAT | 0666)) == -1)
  {
    printf("semaphore error");
    perror("semget");
    exit(1);
  }
  
  semctl(sem_id,0,SETVAL,0);
   shm_ptr = (pid_t*)shmat(shm_id,NULL,0);
  
   create_shared_objects(); 
  
  signal(SIGUSR1,user_sig_handler1);
 *shm_ptr = pid;

 while(1)
   sleep(1);
}


void user_sig_handler1(int signal)
{
   if(signal == SIGUSR1)
   {
     *X = *Y + 1;
      printf("Process 1 \t");
      printf("Values of X : %d and Y : %d \n", *X , *Y);
      P(sem_id);
      kill(*shm_ptr,SIGUSR2);
*shm_ptr = pid; 
      V(sem_id);
      sleep(1);
      
      
   }
}

