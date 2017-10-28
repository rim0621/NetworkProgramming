#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
#include<sys/time.h>
#include<errno.h>
void errProc (const char *);

int main(int argc,char **argv)
{
	int listenSd, connectSd;
	struct sockaddr_in srvAddr, clntAddr;
	int clntAddrLen, readLen, strLen;
	char rBuff[BUFSIZ];
	int maxFd=0;
	fd_set defaultFds,rFds;
	int res,i;

	if(argc!=2)
	{
		printf("usage: %s [port] \n");
		return -1;
	}
	printf("server start....\n");
	listenSd=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(listenSd==-1) errProc("socket");

	memset(&srvAddr,0,sizeof(srvAddr));
	srvAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	srvAddr.sin_family=AF_INET;
	srvAddr.sin_port=htons(atoi(argv[1]));

	if(bind(listenSd,(struct sockaddr*)&srvAddr,sizeof(srvAddr))==-1) errProc("bind");
	if(listen(listenSd,5)<0) errProc("listen");

	FD_ZERO(&defaultFds);
	FD_SET(listenSd,&defaultFds);
	maxFd=listenSd;

	clntAddrLen=sizeof(clntAddr);
	while(1)
	{
		rFds=defaultFds;
		printf("monitoring..\n");
		if((res=select(maxFd+1,&rFds,0,0,NULL)==-1)) break;
		for(i=0;i<maxFd+1;i++)
		{
			if(FD_ISSET(i,&rFds))
			{
				connectSd=accept(listenSd,(struct sockaddr*)&clntAddr,&clntAddrLen);
				if(connectSd==-1)
				{
					fprintf(stderr,"accept error");
					continue;
				
					fprintf(stderr,"a client is connected...\n");
					FD_SET(connectSd,&defaultFds);
					if(maxFd<connectSd)
					{
						maxFd=connectSd;
					}
				}
				else
				{
					readLen=read(i,rBuff,sizeof(rBuff)-1);
					if(readLen==0)
					{
						fprintf(stderr,"a client is disconnected....\n");
						FD_CLR(i,&defaultFds);
						close(i);
						continue;
					}
					rBuff[readLen]='\0';
					printf("client(%d):%s \n",i-3,rBuff);
					write(i,rBuff,strlen(rBuff));
				}
			}
		}
	}
	close(listenSd);
	return 0;
}


void errProc(const char * str)
{
	fprintf(stderr,"%s : %s",str,strerror(errno));
	exit(1);
}
