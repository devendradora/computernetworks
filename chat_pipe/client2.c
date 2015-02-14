#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


int main()
{  
	char buffer[50];

//c2s  client to server  
	//mkfifo("c2s_pipe2",0666);
	//mkfifo("s2c_pipe2",0666); 
    
	int wfd,rfd;
	int c=fork();

	if(c >0 ){
		//parent process
		while(1){
			wfd=open("c2s_pipe2",O_WRONLY);
			read(0,buffer,sizeof(buffer));
		    write(wfd,buffer,sizeof(buffer));
	    //close(wfd);
		}

	}
	else if( c==0 ){
        //child process
			while(1){
				rfd=open("s2c_pipe2",O_RDONLY);
				read(rfd,buffer,sizeof(buffer));
	  		    //write(1,buffer,sizeof(buffer));
	  		    printf("%s\n",buffer);
	           //close(fd);
		     }
			
	}
	else
		perror("fork()");   
	//unlink("c2s_pipe");	
	return 0;
}


