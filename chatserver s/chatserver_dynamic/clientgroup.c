#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<pthread.h>
#include<sys/select.h>
#include<sys/time.h>
#define MAX 100
char* itoa(int val, int base){

	static char buf[32] = {0};

	int i = 30;

	for(; val && i ; --i, val /= base)

		buf[i] = "0123456789abcdef"[val % base];

	return &buf[i+1];

}
int main()
{
        fd_set readset;
        char *myfifo = "/tmp/myfifo";
        int fpi,fpo,h,fi,fo,result,n,gnum,fpog;
        char msg[MAX]="Connect: ",gname[MAX],*name=itoa(getpid(),10),path[MAX]="/tmp/",gpath[MAX]="/tmp/";
        printf("enter group name:"); scanf("%d",&gnum);//gets(gname);
        strcat(path,name); strcpy(msg,path); strcat(msg," ::group::/"); strcat(msg,itoa(gnum,10));
        mkfifo(path,0666);
        mkfifo(myfifo,0666);
        mkfifo(itoa(gnum,10),0666);
        fpo = open(myfifo,O_WRONLY);
        puts(msg);
        write(fpo,msg,sizeof(msg));
        fpi = open(path,O_RDONLY);
        fpog=open(itoa(gnum,10),O_WRONLY);

        while(1)
        {
            FD_ZERO(&readset);
            FD_SET(fpi, &readset);
            FD_SET(0, &readset);
            result=select(fpi+1,&readset,NULL,NULL,NULL);
            if(result>0)
            {
                if (FD_ISSET(fpi, &readset))
                {
                    n=read(fpi,msg,sizeof(msg)); //msg[n]='\0';
                    if(n==0) break;
                    printf("Message: "); puts(msg);
                }
                if(FD_ISSET(0,&readset))
                {
                    //scanf("%s",msg);//
                    n=read(0,msg,sizeof(msg)); msg[n-1]='\0';
                    write(fpog,msg,sizeof(msg));
                }
            }
        }


        if(close(fpo) < 0)
        {
                perror("Error closing FIFO (Named Pipe)\b");
                exit(-3);
        }

        if(unlink(myfifo) < 0)
        {
                perror("Error deleting FIFO (Named Pipe)\b");
                exit(-4);
        }
        return 0;
}


