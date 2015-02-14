#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>
#include<sys/shm.h>
#include<signal.h>

int main()
{
	int wfd,rfd,c;
	mkfifo("c1",0666);
	mkfifo("c2",0666);

	key_t shmkey=ftok(".",12);
	int shmid=shmget(shmkey,1*(sizeof(int)),IPC_CREAT|0660);
	int *shared=(int*)shmat(shmid,NULL,0);
	int sid=*shared;
	//printf("%d\n",sid);
  	
  //signal(SIGUSR1,sig_handler);
	char buf[30];
	
	c=fork();
	if(c>0)
	{
		while(1)
		{
			wfd=open("c1",O_WRONLY);
			read(0,buf,30);
			write(wfd,buf,sizeof(buf));
			kill(sid,SIGUSR1);
		}
	}
	else
	{
		while(1)
		{
			rfd=open("c2",O_RDONLY);
			read(rfd,buf,sizeof(buf));
			printf("%s\n",buf);
		}
	}
	return 0;
}
