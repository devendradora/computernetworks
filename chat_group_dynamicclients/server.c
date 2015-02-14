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
#define MAX_GROUP 5


int shm_pid,shm_client,shm_group;
pid_t *pid_ptr,*group_ptr;;
char  *client_ptr;


int wfd[MAX_GROUP][MAX_CLIENT];
int num_of_groups=0;

struct group{
    int group_num;
    int num_of_clients;
};

struct message {
    char buf[MAX_BUF];
    int client_num;
    int group_num;
};

struct group grp[MAX_GROUP];

void process_sighandler(int sig);

int main()
{   
    struct message msg;
    char buffer[MAX_BUF]; 

    mkfifo("c2s_pipe",0666);   
    int rfd  = open("c2s_pipe",O_RDONLY | O_NONBLOCK);

    if((shm_pid=shmget(ftok(".",'p'),sizeof(pid_t),IPC_CREAT|0666)) == -1) perror("shmget()");
    else pid_ptr=shmat(shm_pid,NULL,0);

    if((shm_client=shmget(ftok(".",'c'),MAX_BUF,IPC_CREAT|0666)) == -1) perror("shmget()");
    else client_ptr=shmat(shm_client,NULL,SHM_RDONLY);

    if((shm_group=shmget(ftok(".",'g'),MAX_BUF,IPC_CREAT|0666)) == -1) perror("shmget()");
    else group_ptr=shmat(shm_group,NULL,SHM_RDONLY);

    *pid_ptr=getpid();
  
    signal(SIGUSR1,process_sighandler);
    int i,j;
    printf("pid of server : %d\n",*pid_ptr);
   
    while(1){
      
      while(read(rfd,&msg,sizeof(struct message)) <=0);
       printf("client %d typed from group %d \n",msg.client_num,msg.group_num);
            for(i=0;i< num_of_groups;i++){  
             if(grp[i].group_num == msg.group_num)
                break;
            }

            for(j=0;j<grp[i].num_of_clients;j++)
                write(wfd[i][j],&msg,sizeof(struct message));
        }

     
 shmdt(pid_ptr); 
 shmdt(client_ptr);
 shmdt(group_ptr);
    //unlink("c2s_pipe");   
return 0;
}

void process_sighandler(int sig){  
    if(sig == SIGUSR1){
         int j,flag=0,group_index;
         for(j=0;j<num_of_groups;j++){
             if(grp[j].group_num == *group_ptr)
                {flag =1 ; group_index =j;break;}
         }

        
        if(flag == 1) { // group already exists           
              grp[group_index].num_of_clients++;
        }
        else{      //group doesn't exist
            group_index=num_of_groups;
            grp[num_of_groups].group_num = *group_ptr;
            grp[num_of_groups++].num_of_clients=1;

        }

        //server to client pipe is opened in specific group
        wfd[group_index][grp[group_index].num_of_clients-1]=open(client_ptr,O_WRONLY);       
        memset(client_ptr,MAX_BUF,'\0');         
        }
        
  }



