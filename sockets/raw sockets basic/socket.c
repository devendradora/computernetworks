#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <arpa/inet.h>




int main(int argc, char** argv)
{

	int s;
	struct sockaddr_in daddr;
	char packet[50];
	/* point the iphdr to the beginning of the packet */
	struct iphdr *ip = (struct iphdr *)packet;  

	int zero=1;
	const int *val=&zero;

	setsockopt(s, IPPROTO_IP, IP_HDRINCL, val, sizeof(zero));
	
	if ((s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) {
		perror("error:");
		exit(EXIT_FAILURE);
	}

	daddr.sin_family = AF_INET;
	daddr.sin_port = 0; /* not needed in SOCK_RAW */
	inet_pton(AF_INET, argv[2], (struct in_addr *)&daddr.sin_addr.s_addr);
	memset(daddr.sin_zero, 0, sizeof(daddr.sin_zero));
	memset(packet, 'A', sizeof(packet));   

	ip->ihl = 5;
	ip->version = 4;
	ip->tos = 0;
	ip->tot_len = htons(40);	
	ip->frag_off = 0;		
	ip->ttl = 64;			
	ip->protocol = IPPROTO_RAW;	
	ip->check = 0;	
	inet_pton(AF_INET, argv[1], (struct in_addr *)&ip->saddr);		
	
	ip->daddr = daddr.sin_addr.s_addr;

	int c = fork();
	if(c > 0)
	{
		while(1)
		{
			sleep(1);
			scanf("%s",packet+sizeof(struct iphdr));
			if (sendto(s, (char *)packet, sizeof(packet), 0, 
				(struct sockaddr *)&daddr, (socklen_t)sizeof(daddr)) < 0)
				error("packet send error:");
		}
	}
	else
	{
		while(1)
		{
			sleep(1);
			int fromlen = sizeof(daddr);
			struct sockaddr_in cli;
			if (recvfrom(s, (char *)packet, sizeof(packet), 0, 
				(struct sockaddr *)&cli, &fromlen) < 0)
				error("packet recv error:");
			int i = sizeof(struct iphdr);
			while (i < sizeof(packet)) {
				fprintf(stderr, "%c", packet[i]);
				i++;
			}
			printf("\n");
		}
	}
	exit(EXIT_SUCCESS);
}

