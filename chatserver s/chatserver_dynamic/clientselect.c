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
        int fpi,fpo,h,fi,fo,result,n;
        char msg[MAX]="Connect: ",msgi[MAX],*name=itoa(getpid(),10),path[MAX]="/tmp/";
        //printf("enter chat name:"); gets(name);
        strcat(path,name);
        mkfifo(path,0666);
        mkfifo(myfifo,0666);
        fpo = open(myfifo,O_WRONLY);
        puts(path);
        write(fpo,path,sizeof(path));
        fpi = open(path,O_RDONLY);

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
                    gets(msg);//read(0,msg,sizeof(msg));
                    write(fpo,msg,sizeof(msg));
                }
            }
        }

    /*if((h=fork())>0)
    {
        close(fpi);
        while(1)
        {
            if(1)
            {
            gets(msg);//read(0,msg,sizeof(msg));
            write(fpo,msg,sizeof(msg));
            }
        }

    }
    else if(h==0)
    {

        close(fpo);
        while(read(fpi,msgi,sizeof(msg)) >0){
        printf("Message: "); puts(msgi);
        }
        exit(0);
    }
    else perror("child creation error");*/

        if(close(fpo) < 0)
        {
                perror("Error closing FIFO (Named Pipe)");
                exit(-3);
        }

        if(unlink(myfifo) < 0)
        {
                perror("Error deleting FIFO (Named Pipe)");
                exit(-4);
        }
        return 0;
}

