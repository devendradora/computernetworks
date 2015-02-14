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
void sig_handler2(int sig);


int main()
{
 pid=getpid();
 printf("pid process 2 %d\n",pid );
 pkey= ftok(".",'p');
 xkey= ftok(".",'x');
 ykey= ftok(".",'y');

 if((sem_pid= semget(pkey,1,IPC_CREAT | 0666)) == -1) perror("semget()");
   semctl(sem_pid,0,SETVAL,1);   // initailize

  //shared memory for process id
   if((shm_pid= shmget(pkey,sizeof(pid_t),IPC_CREAT | 0666 )) == -1 ) perror("shmget()");
   pid_ptr=(pid_t*) shmat(shm_pid,NULL,0); 
  // *pid_ptr=pid;
   
   //shared memory for x and y
   if((shm_xid=shmget(xkey,1,IPC_CREAT | 0666))== -1) perror("shmget()");
   x_ptr=(int*) shmat(shm_xid,NULL,0);
   *x_ptr=0; 

   if((shm_yid=shmget(ykey,1,IPC_CREAT | 0666))== -1) perror("shmget()");
   y_ptr=(int*) shmat(shm_yid,NULL,0);
   *y_ptr=0;

   signal(SIGUSR2,sig_handler2);
 printf("before wait %d\n",pid );
   P(sem_pid,0);  // the process is not blocked as value is reduced from 1 to 0
   kill(*pid_ptr,SIGUSR1);
   printf("after kill pid %d\n",pid );
   *pid_ptr=pid;
   V(sem_pid,0);

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

 void sig_handler2(int sig) {
  
   if(sig == SIGUSR2)
   {
     *y_ptr = *x_ptr + 1;
     printf("Process 2 : x_ptr : %d and y_ptr : %d \n", *x_ptr, *y_ptr);

     P(sem_pid,0);
     kill(*pid_ptr,SIGUSR1);
     *pid_ptr = pid;
     V(sem_pid,0);

     sleep(1);

     
   }
   
 }
