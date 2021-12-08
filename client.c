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

int sockfd, len, n,ifd,cin,ofd;
struct sockaddr_in servaddr;
void creatSock();

typedef struct clinStruct
{
int fd;
char buff[1024];
}clinStruct;
clinStruct *block;


int main(int argc,char** argv) {
	ssize_t cout;
	creatSock();
	block=(clinStruct*)malloc(sizeof(clinStruct));
	len=sizeof(servaddr);

	if(argc!=3)
	{
		printf ("Usage: client <dir1/fname> <dir2/fname>");
        return 1;	
	}

	block->fd=0;	
	strcpy(block->buff,argv[1]);
	sendto(sockfd,block,SIZE,0,(const struct sockaddr*) &servaddr,len);
	n=recvfrom(sockfd,block,SIZE,0,(struct sockaddr*) &servaddr,&len);
	if(block->fd==0)
	{
		printf("\nerror reciving file");
		return -1;
	}
	ifd=block->fd;
	printf("Client Id=%d File=%s\n",ifd,argv[1]);
	ofd=open(argv[2],O_WRONLY | O_CREAT, 0644);
	if(ofd==-1)
	{
		block->fd=-1;
		sendto(sockfd,block,SIZE,0,(const struct sockaddr*) &servaddr,len);
		printf("\nerror1 open for writing");
		return -1;
	}
	while(n>4)
	{
	cout=write(ofd,&block->buff,n-4);
		if(cout!=n-4)
		{
			block->fd=-1;
			sendto(sockfd,block,SIZE,0,(const struct sockaddr*) &servaddr,len);
			printf("\nerror2 open for writing");
			return -1;
		}
	block->fd=ifd;
	sendto(sockfd,block,SIZE,0,(const struct sockaddr*) &servaddr,len);
	n=recvfrom(sockfd,block,SIZE,0,(struct sockaddr*) &servaddr,&len);
	}
	fprintf(stdout,"\nFile received");
}		

void creatSock()
{
	sockfd=socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd<0) {
		perror("socket");
		exit(0);
	}
	servaddr.sin_family=AF_INET;
	if(inet_pton(AF_INET,"127.0.0.1",&servaddr.sin_addr)<0)
	{
		printf("Invaid Address");
		return;
	}
	servaddr.sin_port=htons(PORT);
} 
