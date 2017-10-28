#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>

int main(int argc, char **argv)
{
	int srvSd,clntSd;
	struct sockaddr_in6 srvAddr, clntAddr;
	int clntAddrLen, readLen;
	char rBuff[BUFSIZ];

	if(argc!=2) printf("usage: %s [port] \n", argv[0]);

	printf("server start...\n");
	srvSd=socket(PF_INET6,SOCK_STREAM,IPPROTO_TCP);
	if(srvSd==-1)
	{
		fprintf(stderr,"socket creation erro");
		return -1;
	}

	memset(&srvAddr,0,sizeof(srvAddr));
	srvAddr.sin6_family=AF_INET6;
	srvAddr.sin6_port=htons(atoi(argv[1]));
	srvAddr.sin6_flowinfo=0;
	srvAddr.sin6_addr=in6addr_any;

	if(bind(srvSd,(struct sockaddr *) &srvAddr,sizeof(srvAddr))==-1)
	{
		fprintf(stderr,"bind error");
		return -1;
	}

	if(listen(srvSd,5)<0)
	{
		fprintf(stderr,"listen error");
		return -1;
	}

	clntAddrLen=sizeof(clntAddr);
	while(1)
	{
		clntSd=accept(srvSd,(struct sockaddr *)&clntAddr,&clntAddrLen);
		if(clntSd==-1)
		{
			fprintf(stderr,"Accept error");
		}
		while((readLen=read(clntSd,rBuff,sizeof(rBuff)-1)) !=0)
		{
			printf("client : %s ",rBuff);
			memset(rBuff,0, BUFSIZ);
		}
	}
	close(srvSd);
	return 0;
}
