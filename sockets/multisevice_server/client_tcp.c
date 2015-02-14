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
    struct hostent *server;
    char buffer[256],hostname[16];


    printf("Enter server ip , portno : ");
    scanf("%s%d",hostname,&portno);
    server = gethostbyname(hostname);
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    else{
       read(sockfd,buffer,255);
       printf("%s\n",buffer );
    }

    printf("start typing the message: ");
 while(1){
    bzero(buffer,256);
    read(0,buffer,255);   
    write(sockfd,buffer,strlen(buffer));  
    } 
    // bzero(buffer,256);
    // read(sockfd,buffer,255);
    // printf("%s\n",buffer);
    close(sockfd);
    return 0;
}
