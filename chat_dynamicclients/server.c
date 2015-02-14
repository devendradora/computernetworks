#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/shm.h>
#include <signal.h>
#include <string.h>

#define MAX_BUF 20
#define MAX_CLIENT 20


int shm_pid,shm_client;
pid_t *pid_ptr;
char  *client_ptr;


int wfd[MAX_CLIENT],num_of_clients=0;

struct message{
    char buf[MAX_BUF];
    int client_num;
};

void process_sighandler(int sig);
int rfd ;
int main()
{   
    struct message msg;
    char buffer[MAX_BUF]; 

    mkfifo("c2s_pipe",0666);   
     rfd = open("c2s_pipe",O_RDONLY | O_NONBLOCK);

    if((shm_pid=shmget(ftok(".",'p'),sizeof(pid_t),IPC_CREAT|0666)) == -1) perror("shmget()");
    else pid_ptr=shmat(shm_pid,NULL,0);

    if((shm_client=shmget(ftok(".",'c'),sizeof(pid_t),IPC_CREAT|0666)) == -1) perror("shmget()");
    else client_ptr=shmat(shm_client,NULL,0);

    *pid_ptr=getpid();
  
    signal(SIGUSR1,process_sighandler);
    int i;
    printf("pid of server : %d\n",*pid_ptr);
   
    while(1){
      
      while(read(rfd,&msg,sizeof(struct message)) <=0);
       printf("client %d typed \n",msg.client_num);
       for(i=0;i< num_of_clients;i++)
         {     printf("write to %d ",(i+1));
            write(wfd[i],&msg,sizeof(struct message));
        }

    }

 
 shmdt(pid_ptr);
 
 shmdt(client_ptr);
	//unlink("c2s_pipe");	
return 0;
}

void process_sighandler(int sig){  
    if(sig == SIGUSR1){
        //server to client pipe is opened
        wfd[num_of_clients++]=open(client_ptr,O_WRONLY);       
        memset(client_ptr,MAX_BUF,'\0');         
        
    }

}

