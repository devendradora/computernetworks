#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>

int main()
{
   int sockfd, portno=5500,n;
    struct sockaddr_in serv_addr;   
    char sendline[256], recvline[256],hostname[16]="localhost";

   // printf("server ip is %s, Enter portno : ",hostname);
   // scanf("%d",&portno);    
    
    sockfd = socket(AF_INET,SOCK_DGRAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");

    // struct hostent *server;
    // server = gethostbyname(hostname);
    // bzero((char *) &serv_addr, sizeof(serv_addr));
    // bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");    
    serv_addr.sin_port = htons(portno);


   while(1)
   {  n=read(0,sendline,256);
      sendto(sockfd,sendline,n,0,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
      n=recvfrom(sockfd,recvline,256,0,NULL,NULL);
      recvline[n]=0;
      printf("%s\n",recvline);
   }
}
