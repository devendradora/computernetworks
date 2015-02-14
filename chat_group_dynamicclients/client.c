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



int shm_pid,shm_client,shm_group;
pid_t *pid_ptr,*group_ptr;
char  *client_ptr;


struct message{
    char buf[MAX_BUF];
    int client_num;
    int group_num;
};

char client_name[2],group_name[2];
int  client_num=0,group_num=0;


int main()
{  
	printf("client_num < 1 - 20 > : ");
	scanf("%s",client_name);
    
    printf("Group_num <1 - 5 > : ");
	scanf("%s",group_name);

	client_num=atoi(client_name);
	group_num=atoi(group_name);

	printf("client_%d logged in group_%d \n",client_num,group_num);
    char buffer[MAX_BUF];     

    struct message msg;     
 

    if((shm_pid=shmget(ftok(".",'p'),sizeof(pid_t),IPC_CREAT|0666)) == -1) perror("shmget()");
    else pid_ptr=shmat(shm_pid,NULL,0);
    
    printf("Connected server pid : %d\n",*pid_ptr);

    if((shm_client=shmget(ftok(".",'c'),MAX_BUF,IPC_CREAT|0666)) == -1) perror("shmget()");
    else client_ptr=shmat(shm_client,NULL,0);

    if((shm_group=shmget(ftok(".",'g'),MAX_BUF,IPC_CREAT|0666)) == -1) perror("shmget()");
    else group_ptr=shmat(shm_group,NULL,0);
    
    *group_ptr=group_num;

    char s2cpipe[]="s2cpipe" ;
    strcat(s2cpipe,client_name);
    strcpy(client_ptr,s2cpipe); //storing the name of  server 2 client pipe created in shared memory
    mkfifo(s2cpipe,0666);

    int rfd_c,wfd_c;
      wfd_c=open("c2s_pipe",O_WRONLY);
      rfd_c=open(s2cpipe,O_RDONLY| O_NONBLOCK);
  
    kill(*pid_ptr,SIGUSR1);
 //fflush(stdout);
    
	int c=fork();

	if(c >0 ){
		//parent process -Writing to pipe
	      msg.client_num=client_num;
	      msg.group_num=group_num; 
		while(1){
           
			read(0,msg.buf,MAX_BUF);
		    write(wfd_c,&msg,sizeof(struct message));
	 
		}

	}
	else if( c==0 ){
        //child process -Reading from pipe
           msg.client_num=client_num;  
			while(1){
				
				 while(read(rfd_c,&msg,sizeof(struct message)) <= 0);                 

				if(msg.client_num != client_num)	  		 
	  		      printf("From %d : %s\n",msg.client_num,msg.buf);
	         
		     }
			
	}
	else
		perror("fork()");  

	

 shmdt(pid_ptr); 
 shmdt(client_ptr);
 shmdt(group_ptr);
	//unlink("c2s_pipe");	
	return 0;
}


