#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>

int main()
{
	int wfd1,rfd1,wfd2,rfd2,wfd3,rfd3;
	char buf[30];int i;
	mkfifo("c1",0666);
	mkfifo("c2",0666);
	mkfifo("c3",0666);
	mkfifo("c4",0666);
	mkfifo("c5",0666);
	mkfifo("c6",0666);
	wfd1=open("c2",O_WRONLY);
	rfd1=open("c1",O_RDONLY);
	wfd2=open("c4",O_WRONLY);
	rfd2=open("c3",O_RDONLY);
	wfd3=open("c6",O_WRONLY);
	rfd3=open("c5",O_RDONLY);
	while(1)
	{
		read(rfd1,buf,sizeof(buf));
			write(wfd2,buf,sizeof(buf));
			write(wfd3,buf,sizeof(buf));
		//for(i=0;i<sizeof(buf);i++)
			//buf[i]=" ";
		
		read(rfd2,buf,sizeof(buf));
			write(wfd1,buf,sizeof(buf));
			write(wfd3,buf,sizeof(buf));
		//for(i=0;i<sizeof(buf);i++)
			//buf[i]="";
			
		read(rfd3,buf,sizeof(buf));
			write(wfd1,buf,sizeof(buf));
			write(wfd2,buf,sizeof(buf));
	}
}
