//Handle Multiple clients using select
//connection oriented  TCP/IP and connectionless UDP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#define max_connection 10

void error(const char *msg){
	perror(msg);
	exit(1);
}

int main()
{    
	char buffer[256];
	fd_set read_fds;

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
	timeout.tv_sec=1;
	timeout.tv_usec=0;
	int ret,max_fd=sockfd,i;

	newsockfd[connc++]=sockfd;
	while(1){
		//printf("\tWaiting for next client ..........\n");
		FD_ZERO(&read_fds);
		for(i=0;i<connc;i++)
			FD_SET(newsockfd[i],&read_fds);
		ret=select(max_fd+1,&read_fds,NULL,NULL,&timeout);
       // printf("retun sel %d\n",ret );
		if(ret > 0){

			if(FD_ISSET(sockfd,&read_fds)){
				socklen_t cli_len = sizeof(cli_addr);
				newsockfd[connc] = accept(sockfd,(struct sockaddr *) &cli_addr,&cli_len);
				if (newsockfd[connc] < 0) 
					error("ERROR on accept");
				printf("connection accepted from client %d\n",connc);

				FD_SET(newsockfd[connc],&read_fds);
				if(newsockfd[connc]>max_fd) 
					max_fd=newsockfd[connc]; 
				connc++; 
			}
           //char temp_buf[256];
            //i=0 for sockfd :server to accept connections
			for(i=1;i<connc;i++){
				if (FD_ISSET(newsockfd[i],&read_fds)) { 
						// bzero(buffer,256);	
						// while(read(newsockfd[i],buffer,255)<=0); 
					if(recv(newsockfd[i],buffer,256,0) <0 ) error("Mesg recv failed");
					else{
						int j;
                        // itoa(i,temp_buf,10);
                        // strcat(temp_buf,": ");
                        // strcat(temp_buf,buffer);
						for(j=1;j<connc;j++)
							if(j!=i)
								send(newsockfd[j],buffer,strlen(buffer),0);						
							printf("MSG Recieved from %d : %s\n",i,buffer);

						}
					} 
				}
			}

		}
		return 0; 
	}
