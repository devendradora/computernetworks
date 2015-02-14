//connection oriented client server program - TCP/IP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#define max_connection 10

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main()
{    
     char buffer[256];
     int sockfd, newsockfd, portno,n;     
     printf("Enter port number : ");
     scanf("%d",&portno);  

     struct sockaddr_in serv_addr, cli_addr;    
    
     sockfd = socket(AF_INET, SOCK_STREAM, 0);     
     if (sockfd < 0) 
        error("ERROR opening socket");
    
     bzero((char *) &serv_addr, sizeof(serv_addr));
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
         error("ERROR on binding");
     
     listen(sockfd,max_connection);
     socklen_t cli_len = sizeof(cli_addr);
     newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr,&cli_len);
     if (newsockfd < 0) 
          error("ERROR on accept");
     write(newsockfd,"Connection accepted",20);
     bzero(buffer,256);

     int c=fork();
    while(1){
     
         if(c > 0){
           close(newsockfd);
         }
         else{
           close(sockfd);
           while(read(newsockfd,buffer,255)<=0);   
           printf("MSG Recieved: %s\n",buffer);
         }
     }
    
     return 0; 
}
