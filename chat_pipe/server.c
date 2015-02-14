#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
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



    while(1){
    	
    	read(rfd1,buffer,sizeof(buffer));
    	write(wfd2,buffer,sizeof(buffer));
    	write(wfd3,buffer,sizeof(buffer));

    	//printf("%s\n",buffer );
    	read(rfd2,buffer,sizeof(buffer));
    	write(wfd1,buffer,sizeof(buffer));
    	write(wfd3,buffer,sizeof(buffer));

    	read(rfd3,buffer,sizeof(buffer));
    	write(wfd1,buffer,sizeof(buffer));
    	write(wfd2,buffer,sizeof(buffer));

    }
	
	   
	//unlink("c2s_pipe");	
	return 0;
}


