
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

char buffer[100];
void read_file(int fd);


int main()
{  
  
   int fd=open("sample.txt",O_RDONLY);
   char *arg[]={"f2"};
  
        dup2(fd,0);

   int c=fork();
   

   if(c>0){
        printf("Reading by file_read1 .......\n"); ;      
        
        read_file(fd);      

      //wait(); 
     // exit(1);     
      // printf("Control transferred to file 1\n");
   }
   else if(c==0){
     printf("Control transferred to file 1\n");
     // execve("objf2",arg,NULL);
     system("./f2");
    //perror("execve()");

   }
   else
   {
     printf("Alert dev fork err !\n");
   }
  
  return 0;
}

void read_file(int fd){
 
 int cnt=1;
 int temp=read(fd,buffer,sizeof(buffer));
        
       while(temp != EOF) { 
          int i=0;
              while(i < temp){
                  if(cnt <= 5){
                     if(buffer[i] != '\n')
                       {printf("%c",buffer[i]);i++;}
                    else
                     {printf("\n"); cnt++;i++;}
                  } 
                  else{
                    cnt=1;
                   // break;
                    exit(1);
                  } 
            }  

            temp=read(fd,buffer,sizeof(buffer));       

       }


}