#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/sem.h>
#include <sys/shm.h>


int sem_pid;
int shm_pid,shm_xid,shm_yid;
pid_t pid,*pid_ptr;
int *x_ptr,*y_ptr;

key_t pkey,xkey,ykey;


void P(int sem_id,int sem_num); //for wait operation
void V(int sem_id,int sem_num); //for signalpassing operation
void sig_handler1(int sig);


int main()
{
 pid=getpid();
 printf("pid process 1 %d\n",pid );
 pkey= ftok(".",'p');
 xkey= ftok(".",'x');
 ykey= ftok(".",'y');

 if((sem_pid= semget(pkey,1,IPC_CREAT | 0666)) == -1) perror("semget()");
   semctl(sem_pid,0,SETVAL,0);   // initailize

  //shared memory for process id
   if((shm_pid= shmget(pkey,sizeof(pid_t),IPC_CREAT | 0666 )) == -1 ) perror("shmget()");
   pid_ptr=(pid_t*) shmat(shm_pid,NULL,0); 
   *pid_ptr=pid;
   
   //shared memory for x and y
   if((shm_xid=shmget(xkey,1,IPC_CREAT | 0666))== -1) perror("shmget()");
   x_ptr=(int*) shmat(shm_xid,NULL,0);
   *x_ptr=0; 

   if((shm_yid=shmget(ykey,1,IPC_CREAT | 0666))== -1) perror("shmget()");
   y_ptr=(int*) shmat(shm_yid,NULL,0);
   *y_ptr=0;

   signal(SIGUSR1,sig_handler1);



   while(1)
     sleep(1);
   
   
 }

 void P(int sem_id,int sem_num){

   struct sembuf buf;
   buf.sem_num = 0;
   buf.sem_op = -1;
   buf.sem_flg = 0;

   semop(sem_id, &buf , 1);
 }

 void V(int sem_id,int sem_num) {

   struct sembuf buf;
   buf.sem_num = 0;
   buf.sem_op = 1;
   buf.sem_flg = 0;  

   semop(sem_id, &buf, 1);
 }

 void sig_handler1(int sig) {

   if(sig == SIGUSR1)
   {
     *x_ptr = *y_ptr + 1;
     printf("Process 1 : x_ptr : %d and y_ptr : %d \n", *x_ptr, *y_ptr);

     P(sem_pid,0);
     kill(*pid_ptr,SIGUSR2);
     *pid_ptr = pid;
     V(sem_pid,0);

     sleep(1);


   }

 }
