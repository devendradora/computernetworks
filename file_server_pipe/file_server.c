//receives the file name from client and serves the contents of the file
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_BUF 1024

int main()
{  
	//mkfifo("s2c_pipe",0666);		

    char buf[MAX_BUF],fname[50];	
	
	int fd=open("c2s_pipe", O_RDONLY);
	read(fd, fname, 50);
	close(fd);
 	
 	int fd2=open("c2s_pipe", O_WRONLY);
 	int file =open(fname,O_RDONLY);
    read(file,buf,MAX_BUF);
    write(fd2,buf,sizeof(buf));
    printf("File contents served .......\n");
    close(file);
    close(fd2);

	return 0;
}

