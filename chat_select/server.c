#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/select.h>
#include <error.h>

#define MAX_BUF 50


int main()
{  
	mkfifo("s2c_pipe1",0666);
	mkfifo("s2c_pipe2",0666);
	mkfifo("s2c_pipe3",0666);

	mkfifo("c2s_pipe1",0666);
	mkfifo("c2s_pipe2",0666);
	mkfifo("c2s_pipe3",0666);

    char buffer[MAX_BUF];	
    int rfd1,rfd2,rfd3,wfd1,wfd2,wfd3;
    
    rfd1=open("c2s_pipe1", O_RDONLY);
    rfd2=open("c2s_pipe2", O_RDONLY);
    rfd3=open("c2s_pipe3", O_RDONLY);

    wfd1=open("s2c_pipe1", O_WRONLY);
    wfd2=open("s2c_pipe2", O_WRONLY);
    wfd3=open("s2c_pipe3", O_WRONLY);



    int ret;
    
    fd_set rfds;
    

    struct timeval timeout;
    timeout.tv_sec=0;
    timeout.tv_usec=0;


while(1){

        FD_SET(rfd1,&rfds);          
        ret=select(rfd1+1,&rfds,NULL,NULL,&timeout);
 
       if(ret > 0) {
             
             if (FD_ISSET(rfd1,&rfds)) {           
                 printf("client 1 typed..... \n");
                 read(rfd1,buffer,sizeof(buffer));
                 write(wfd2,buffer,sizeof(buffer));
                 write(wfd3,buffer,sizeof(buffer));
              }
          }
         else if(ret ==-1)
            perror("select()");
            
    	FD_SET(rfd2,&rfds);          
        ret=select(rfd2+1,&rfds,NULL,NULL,&timeout);
           if(ret > 0) {
             if (FD_ISSET(rfd2,&rfds))  {   
                printf("client 2 typed.....\n");
                read(rfd2,buffer,sizeof(buffer));
                write(wfd1,buffer,sizeof(buffer));
                write(wfd3,buffer,sizeof(buffer));
            }
              }
         else if(ret ==-1)
            perror("select()");
            
        FD_SET(rfd3,&rfds);          
        ret=select(rfd3+1,&rfds,NULL,NULL,&timeout);
           if(ret > 0) {
            if (FD_ISSET(rfd3,&rfds) ) { 
             printf("client 3 typed.....\n");  
             read(rfd3,buffer,sizeof(buffer));
             write(wfd1,buffer,sizeof(buffer));
             write(wfd2,buffer,sizeof(buffer));
           }
            }
         else if(ret ==-1)
            perror("select()");       
    
     FD_ZERO(&rfds);

}


	//unlink("c2s_pipe");	
return 0;
}


