#include <stdio.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <signal.h>

void user_sig_handler2(int);
pid_t *shmptr;
pid_t pid;
int shmid;
int sem1;
int *x,*y;
int main()
{
	key_t key=ftok(".", 's');
	pid=getpid();
	shmid=shmget(key,sizeof(pid_t),IPC_CREAT|0666);
	shmptr=(pid_t*)shmat(shmid,NULL,0);
	sem1=semget(key,1,IPC_CREAT|0666);
	semctl(sem1,0,SETVAL,1);
	int xid,yid;
	xid = shmget(123,1,IPC_CREAT | 0666);
	x=(int*)shmat(xid,NULL,0);
	yid = shmget(456,1,IPC_CREAT | 0666);
	y=(int*)shmat(yid,NULL,0);
	*x=0;*y=0;
	signal(SIGUSR2,user_sig_handler2);
	struct sembuf buf;
	buf.sem_num=0;
	buf.sem_flg=0;
	buf.sem_op=-1;
	semop(sem1,&buf,1);

	kill(*shmptr, SIGUSR1);
	
	*shmptr=pid;
	buf.sem_op=1;
	semop(sem1,&buf,1);
	
	while(1)
		sleep(1);
}
void user_sig_handler2(int sig)
{
	if(sig==SIGUSR2)
	{
		*y=*x+1;
		printf("process2 x= %d  y= %d\n",*x,*y);
		struct sembuf buf;
		buf.sem_num=0;
		buf.sem_flg=0;
		buf.sem_op=-1;
		semop(sem1,&buf,1);
		kill(*shmptr,SIGUSR1);
		*shmptr=pid;
		buf.sem_op=1;
		semop(sem1,&buf,1);
		sleep(1);
	}
}

