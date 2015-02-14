
//sends file name
// run the client program first

#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>



int main()
{  
	mkfifo("c2s_pipe",0666);	
    char filename[]="sample.txt";
    int fd;	
	char buffer[1024];
	fd=open("c2s_pipe",O_WRONLY);
	write(fd,filename,sizeof(filename));
	close(fd);

	int c=fork();

	if(c >0 ){
		//parent process
		wait();
		//system("./s");
		int fd2=open("c2s_pipe",O_RDONLY);		
		read(fd2,buffer,1024);
	    write(1,buffer,sizeof(buffer));
	  	close(fd2);
	  	printf("\n\n\ndone\n");

	}
	else if( c==0 ){
        printf("Requesting server .......\n");
		//execve("s",NULL,NULL);
		
		system("gcc file_server.c -o s");
		
	}
	else
		printf("fork error \n");

   
	//unlink("c2s_pipe");	
	return 0;
}


