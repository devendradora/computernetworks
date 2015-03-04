#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <pthread.h>
#include <iostream>
#include <vector>
#include <sys/select.h>
#define SERV_PORT 9889
#define SA struct sockaddr
#define LISTENQ 100
#define noofprocesses 3
#define noofthreads 3
using namespace std;
int listenfd;
void* client_handle(void *ptr);

int main()
{
  struct sockaddr_in servaddr;	
  listenfd = socket(AF_INET,SOCK_STREAM,0);	
  bzero(&servaddr,sizeof(servaddr));	
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(SERV_PORT);
  bind(listenfd,(SA*) &servaddr,sizeof(servaddr));
  listen(listenfd,LISTENQ);
  int f;
  for(int i=0;i<noofprocesses;i++)
  {
     f=fork();
     if(f==0)
     {
       pthread_t threads[noofthreads];
       for(int j=0;j<noofthreads;j++){
       pthread_create(&threads[i],NULL,client_handle,NULL);
       pthread_join(threads[i],NULL);}
     }
  }
  while(1);
  return 0;
}

void* client_handle(void *ptr)
{
  vector<int> connfds;
  int connfd,maxfd=0;
  fd_set rfds;
  struct timeval selecttime;
  selecttime.tv_sec=0;
  selecttime.tv_usec=60;
  char buffer[30];
  struct sockaddr_in cliaddr;
  socklen_t clilen;
  clilen = sizeof(cliaddr);
  while(1)
  {
    FD_ZERO(&rfds);
    maxfd=listenfd;
    FD_SET(listenfd,&rfds);
    select(maxfd+1,&rfds,NULL,NULL,&selecttime);
    if(FD_ISSET(listenfd,&rfds))
    {
      connfd = accept(listenfd,(SA*) &cliaddr, &clilen);
      connfds.push_back(connfd);
    }
    FD_ZERO(&rfds);
    for(int i=0;i<connfds.size();i++){
    if(connfds[i]>maxfd)maxfd=connfds[i];
    FD_SET(connfds[i],&rfds);}
    select(maxfd+1,&rfds,NULL,NULL,&selecttime);
    for(int i=0;i<connfds.size();i++)
    {
      if(FD_ISSET(connfds[i],&rfds))
      {
         int n=recv(connfds[i],buffer,30,0);
         send(connfds[i],buffer,n,0);
      }
    }
  }
}