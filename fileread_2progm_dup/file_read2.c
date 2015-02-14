
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
char buffer[1024];
void read_file(int fd);


int main()
{  
    



   
        printf("Reading by file_read2....... \n");       
        //  dup(fd,0) used in file_read1.c
       //  read_file(0); 
       int fd2= read(0,buffer,sizeof(buffer));
       
      printf("%s\n", buffer);
    

  return 0;
}

void read_file(int fd){
 
 int cnt=1;
 int temp=read(0,buffer,sizeof(buffer));
        
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
                    break;
                  } 
            }  

            temp=read(0,buffer,sizeof(buffer));       

       }

}