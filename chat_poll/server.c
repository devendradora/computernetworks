#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <poll.h>

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


    struct pollfd fds[3];
    int timeout_msecs = 1000;
    int ret;

    fds[0].fd = rfd1;                 
    fds[0].events = POLLIN;  

    fds[1].fd = rfd2;                 
    fds[1].events = POLLIN;  

    fds[2].fd = rfd3;                 
    fds[2].events = POLLIN;  




while(1){
        ret=poll(fds,3,timeout_msecs);

       if(ret > 0) {
              
             if (fds[0].revents && POLLIN) {           
                 printf("client 1 typed..... \n");
                 read(rfd1,buffer,sizeof(buffer));
                 write(wfd2,buffer,sizeof(buffer));
                 write(wfd3,buffer,sizeof(buffer));
              }

    	
             if (fds[1].revents && POLLIN) {   
                printf("client 2 typed.....\n");
                read(rfd2,buffer,sizeof(buffer));
                write(wfd1,buffer,sizeof(buffer));
                write(wfd3,buffer,sizeof(buffer));
            }

            if (fds[2].revents && POLLIN) { 
             printf("client 3 typed.....\n");  
             read(rfd3,buffer,sizeof(buffer));
             write(wfd1,buffer,sizeof(buffer));
             write(wfd2,buffer,sizeof(buffer));
           }
    }

}


	//unlink("c2s_pipe");	
return 0;
}


