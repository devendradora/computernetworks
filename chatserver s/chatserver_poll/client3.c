#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>

int main()
{
	int wfd,rfd,c;
	mkfifo("c5",0666);
	mkfifo("c6",0666);
	char buf[30];
	c=fork();
	if(c>0)
	{
		while(1)
		{
			wfd=open("c5",O_WRONLY);
			read(1,buf,30);
			write(wfd,buf,sizeof(buf));
		}
	}
	else
	{
		while(1)
		{
			rfd=open("c6",O_RDONLY);
			read(rfd,buf,sizeof(buf));
			printf("%s\n",buf);
		}
	}
	return 0;
}
