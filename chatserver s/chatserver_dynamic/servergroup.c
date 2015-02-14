#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/select.h>
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
        int fp,fpo[100][100],no_clients=0,i,gnum,gcnt[100],fpi[100],max,result,j;
        char msg[MAX],cpath[MAX];

        printf("Creating Pipe...Waiting for receiver for Process...\n");

        mkfifo(myfifo,0666);

        fp = open(myfifo,O_RDONLY);
        for(i=0;i<100;i++,gcnt[i]=0) fpi[i]=-1;

while(1)
{

        //printf("client connected\n");
        FD_ZERO(&readset);
        FD_SET(fp,&readset); max=fp;
        for(i=0;i<100;i++) if(gcnt[i]>0) { FD_SET(fpi[i],&readset); if(max<fpi[i]) max=fpi[i];}
        result=select(max+1,&readset,NULL,NULL,NULL);
        while(result--)
        {
            if(FD_ISSET(fp,&readset))
            {
                read(fp,msg,sizeof(msg));
                printf("\nMessage : ");
                puts(msg);

                if(strncmp(msg,"/tmp/",5)==0)
                {
                sscanf(msg,"%s ::group::/%d",cpath,&gnum);
                fpo[gnum][gcnt[gnum]]=open(cpath,O_WRONLY);
                gcnt[gnum]++;
                    if(gcnt[gnum]==1)
                    {
                    fpi[gnum]=open(itoa(gnum,10),O_RDONLY);
                    printf("group %d created\n",gnum);
                    }
                if(fpo[gnum][gcnt[gnum]-1]>0) { printf("client added\n");} else { printf("could not add client\n");}

                }
            for(i=0;i<100;i++)
            for(j=0;j<gcnt[i];j++)
            if(write(fpo[i][j],msg,sizeof(msg)) > 0)
            { printf("Message has been sent to FIFO (Named Pipe)\n"); }
            }
            else
            for(i=0;i<100;i++)
            if(gcnt[i])
            if(FD_ISSET(fpi[i],&readset))
            {
                int n=read(fpi[i],msg,sizeof(msg)); if(n==0) break;
                printf("Group %d Mesage :",gnum); //write(0,msg,n);
                puts(msg);
                for(j=0;j<gcnt[i];j++) write(fpo[i][j],msg,sizeof(msg));
            }
        }
 }
        if(close(fp) < 0)
        {
                perror("Error closing FIFO (Named Pipe)\n");
                exit(-4);
        }
        return 0;
}
