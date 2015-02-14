#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>
#include<signal.h>
#include<sys/shm.h>

int wfd1,rfd1,wfd2,rfd2;	//wfd3,rfd3;

void sig_handler1(int signo)
{
	char buf[30];
	printf("signal received from client1\n");
			read(rfd1,buf,sizeof(buf));
				write(wfd2,buf,sizeof(buf));
	signal(SIGUSR1,sig_handler1);
}

void sig_handler2(int signo)
{
	char buf[30];
	printf("signal received from client2\n");
			read(rfd2,buf,sizeof(buf));
				write(wfd1,buf,sizeof(buf));
	signal(SIGUSR2,sig_handler2);
}

int main()
{

	int signum;
	
	key_t shmkey=ftok(".",12);
	int shmid=shmget(shmkey,1*(sizeof(int)),IPC_CREAT|0660);
	int *shared=(int*)shmat(shmid,NULL,0);
	*shared=getpid();
	
	char buf[30];int i;
	mkfifo("c1",0666);
	mkfifo("c2",0666);
	mkfifo("c3",0666);
	mkfifo("c4",0666);
	//mkfifo("c5",0666);
	//mkfifo("c6",0666);
	wfd1=open("c2",O_WRONLY);
	rfd1=open("c1",O_RDONLY);
	wfd2=open("c4",O_WRONLY);
	rfd2=open("c3",O_RDONLY);
	
	signal(SIGUSR1,sig_handler1);
	signal(SIGUSR2,sig_handler2);
	
}
