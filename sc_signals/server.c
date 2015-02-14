#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/sem.h>
#include<sys/shm.h>

#define KEY_C1 555
#define KEY_C2 666

int c1,c2;
int *shm_c1, *shm_c2;

int shm_id1,shm_id2;
int sem_id1,sem_id2;

pid_t pid,*shmptr1,*shmptr2;

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

    c1 = init_shm(KEY_C1, 1, IPC_CREAT | 0666);
    c2 = init_shm(KEY_C2, 1 , IPC_CREAT | 0666);
   shm_c1 = (int*)shmat(c1, NULL, 0);
   shm_c2 = (int*)shmat(c2, NULL, 0);
   *shm_c1 = 0;
   *shm_c2 = 0;

}

void sig_handler1(int signal)
{
   if(signal == SIGUSR1)
   {
     printf("Server Reached by Client 1\n");
     shm_c1 = (int*)shmat(c1, NULL, 0);
     *shm_c2 = *shm_c1;
     printf("Value Read by Client 1 %d",*shm_c1);
     P(sem_id2);
     kill(*shmptr2,SIGUSR2);
     *shmptr2= pid;
     V(sem_id2);
     sleep(rand() % 3);
   }
   
}

void sig_handler2(int signal)
{
  if(signal == SIGUSR2)
   {
      printf("Server Reached by Client 2\n");
       *shm_c1 = *shm_c2;
     printf("Value Read by Client 2 %d",*shm_c2);
     P(sem_id1);
     kill(*shmptr1,SIGUSR1);
     *shmptr1 = pid;
     V(sem_id1);
     sleep(rand() % 3);
   }
}

int main()
{
   pid = getpid();
   int key1  = ftok(".",'a');
   int key2 = ftok(".",'b');
   shm_id1 = init_shm(key1,sizeof(pid_t),IPC_CREAT | 0666);
   shm_id2 = init_shm(key2,sizeof(pid_t),IPC_CREAT | 0666);
   
   shmptr1 = (pid_t*)shmat(shm_id1,NULL,0);
   shmptr2 = (pid_t*)shmat(shm_id2,NULL,0);
   *shmptr1 = pid;
   sem_id1 = init_sem(key1,1,IPC_CREAT|0666);
   sem_id2 = init_sem(key2,1,IPC_CREAT|0666);

   semctl(sem_id1,0,SETVAL,0);
   semctl(sem_id2,0,SETVAL,0);

   create_shared_objects();
   signal(SIGUSR1, sig_handler1);
    signal(SIGUSR2, sig_handler2);

   while(1)
     sleep(1);  
}


