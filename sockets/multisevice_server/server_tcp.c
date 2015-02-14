//Multi service server
//connection oriented client server program - TCP/IP
//Each Forked intance of server is responsible for serving a specific purpose.
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
	fd_set newsockfds;

	int sockfd, newsockfd[max_connection], portno,connc=0;     
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

	struct timeval timeout;
	timeout.tv_sec=0;
	timeout.tv_usec=0;
	int ret,max=-1;

	while(1){

		socklen_t cli_len = sizeof(cli_addr);
		newsockfd[connc] = accept(sockfd,(struct sockaddr *) &cli_addr,&cli_len);
		if (newsockfd[connc] < 0) 
			error("ERROR on accept");
		write(newsockfd[connc],"Connection accepted",20);

		int c=fork();
		if(c > 0){
		  close(newsockfd[connc]);
		}
		else{
close(sockfd);
		}

		FD_SET(newsockfd[connc],&newsockfds);
		if(newsockfd[connc]>max) 
			max=newsockfd[connc];        

		ret=select(max+1,&newsockfds,NULL,NULL,&timeout);

		if(ret > 0){
			for(int i=0;i<=connc;i++){
				if (FD_ISSET(newsockfd[i],&newsockfds)) { 
					bzero(buffer,256);
					int c=fork();
					while(1){

						
							
							while(read(newsockfd[i],buffer,255)<=0);   
							printf("MSG Recieved: %s\n",buffer);
						}
					} 
				}
			}
		}
	}


		return 0; 
}
