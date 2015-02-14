#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/sem.h>
#include<sys/shm.h>
#define KEY_C1 555


int shm_id,*shm_c1;
int sem_id;
pid_t pid,*shmptr;

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

int init_sem(int key, int nsem , int flg)
{
  if((shmget(key,nsem, flg)) == -1)
  {
    printf("semaphore error");
    perror("semget");
    exit(1);
  }
}

void create_shared_objects()
{
int c1 = init_shm(KEY_C1, 1, IPC_CREAT | 0666);
   
   shm_c1 = (int*)shmat(c1, NULL, 0);
}

void sig_handler(int signal)
{
   if(signal == SIGUSR1)
   {
     printf("Server accessed Client 1\n");
     printf("Client 1 Received message  %d from Client 2 via server \n",*shm_c1);
     printf("Enter Number to send from c1 to c2 :  ");
   
   scanf("%d",shm_c1);
     P(sem_id);
     kill(*shmptr,SIGUSR1);
     *shmptr = pid;
     V(sem_id);
    sleep(rand() % 3);
   }
}


int main()
{
   pid = getpid();
   int key  = ftok(".",'a');
   shm_id = init_shm(key,sizeof(pid_t),IPC_CREAT | 0666);
   
   
   shmptr = (pid_t*)shmat(shm_id,NULL,0);

   sem_id= init_sem(key,1,IPC_CREAT|0666);

   semctl(sem_id,0,SETVAL,1);
   create_shared_objects();
   
   signal(SIGUSR1, sig_handler);
   printf("Enter Number to send from c1 to c2 :  ");
   
   scanf("%d",shm_c1);
   P(sem_id);
   kill(*shmptr,SIGUSR1);
   *shmptr = pid;
   V(sem_id);

   while(1)
     sleep(1);
   
   
}


