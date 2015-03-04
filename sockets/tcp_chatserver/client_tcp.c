#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main()
{
    int sockfd, portno;
    struct sockaddr_in serv_addr;   
    char buffer[256],hostname[16]="localhost";

    printf("server ip is %s, Enter portno : ",hostname);
    scanf("%d",&portno);    
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");

    struct hostent *server;
    server = gethostbyname(hostname);
    bzero((char *) &serv_addr, sizeof(serv_addr));
    bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_family = AF_INET;
    //serv_addr.sin_addr.s_addr=inet_addr(hostname);    
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
  
 int c=fork();
 if(c>0){
          while(1){
            bzero(buffer,256);
            printf("Enter the message: ");
            read(0,buffer,255);
                //write(sockfd,buffer,strlen(buffer)); 
            send(sockfd,buffer,strlen(buffer),0);
         }
  }
  else
  {   
        while(1){
            bzero(buffer,256);
            recv(sockfd,buffer,256,0); 
           // read(sockfd,buffer,255);
            printf("%s\n",buffer);
        } 

  }

close(sockfd);
return 0;
}
