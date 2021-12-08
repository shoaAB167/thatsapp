#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <sys/stat.h>
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#define PORT 5000
#define SIZE 1028 

int sockfd,len,n,cnt=0,i=0,ifd;
ssize_t cin,cout;
struct sockaddr_in servaddr,clinaddr;
void creatSock();

typedef struct serStruct
{
int fd;
char buff[1024];
}serStruct;

serStruct *block;

int main() 
{
	creatSock();
	block=(serStruct *)malloc(sizeof(serStruct));
	len=sizeof(clinaddr);
	while(1)
	{
		n=recvfrom(sockfd,block,SIZE,0,(struct sockaddr*)&clinaddr,&len);
		if(block->fd==0)
		{
        	fprintf (stdout,"\nrequest for %s file",block->buff);
			ifd=open(block->buff,O_RDONLY);			
			if (ifd == -1)
   		 	{	
        		printf("\nerror open for reading");
				sendto(sockfd,block,SIZE,0,(struct sockaddr*)&clinaddr,len);	
  		    	return -1;
   			}
			block->fd=ifd;
		}
		if(block->fd>0)
		{
			cin=read(block->fd,&block->buff,1024);
			if(cin>=0)
			{
				sendto(sockfd,block,cin+4,0,(struct sockaddr*)&clinaddr,len);
			}
		}
		else
			close(block->fd);
	}
	return 0;
}

void creatSock()
{
	sockfd=socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd<0) {
		perror("socket");
		exit(0);
	}
	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=INADDR_ANY;
	servaddr.sin_port=htons(PORT);
	if((bind(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr)))<0) {
	perror("socket");
		exit(0);
	}
}
