#include<stdio.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<string.h>

#define PORT 9001

int main(int argc,char **argv)
{
	int connectSd;
	struct sockaddr_in6 clntAddr;
	int clntAddrLen,readLen;
	char wBuff[BUFSIZ];
	char tmpv6[INET6_ADDRSTRLEN];

	int i ,res;

	if(argc!=3)
	{
		printf("usage: %s <ip address> <prot number> \n",argv[0]);
		return -1;
	}

	connectSd=socket(PF_INET6,SOCK_STREAM,0);
	if(connectSd==-1)
	{
		printf("socket creation error");
		return -1;
	}
	printf("======client progream =====\n");

	memset(&clntAddr,0,sizeof(clntAddr));
	clntAddr.sin6_family=AF_INET6;
	res=inet_pton(AF_INET6,argv[1],(void*)&(clntAddr.sin6_addr));
	if(res<=0)
	{
		fprintf(stderr,"Address error\n");
		return -1;
	}
	clntAddr.sin6_port=htons(atoi(argv[2]));
	clntAddr.sin6_scope_id=if_nametoindex("eth0");

	if(inet_ntop(AF_INET6,&(clntAddr.sin6_addr),tmpv6,INET6_ADDRSTRLEN)==NULL)
	{
		fprintf(stderr, "inet_ntop error\n");
		return -1;
	}

	printf("========connecting to %s====\n",tmpv6);

	if(connect(connectSd,(struct sockaddr *) &clntAddr, sizeof(clntAddr))==-1)
	{
		printf("connect error");
		close(connectSd);
		return -1;
	}
	while(1)
	{
		fgets(wBuff,sizeof(wBuff),stdin);
		readLen=strlen(wBuff);
		if(!strcmp(wBuff,"end\n"))break;
		write(connectSd,wBuff,readLen);
	}
	close(connectSd);
	return 0;
}
