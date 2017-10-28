#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>

int main(int argc,char **argv)
{
	int srvSd,clntSd;
	struct sockaddr_in srvAddr,clntAddr;
	int clntAddrLen,readLen,strLen;
	char rBuff[BUFSIZ];
	socklen_t optLen;
	int optVal;

	if(argc!=3)
	{
		printf("usage:%s[port] [reuseaddr 0 or 1]\n",argv[0]);
		return -1;
	}
	
	printf("server start....\n");
	srvSd=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
	optVal=atoi(argv[2]);
	optLen=sizeof(optVal);
	setsockopt(srvSd,SOL_SOCKET,SO_REUSEADDR,(void *)&optVal,optLen);

	memset(&srvAddr,0,sizeof(srvAddr));
	srvAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	srvAddr.sin_family=AF_INET;
	srvAddr.sin_port=htons(atoi(argv[1]));

	if(bind(srvSd,(struct sockaddr *) &srvAddr, sizeof(srvAddr))==-1)
	{
		fprintf(stderr,"bind error");
		return -1;
	}
	listen(srvSd,5);
	clntAddrLen=sizeof(clntAddr);
	while(1)
	{
		clntSd=accept(srvSd,(struct sockaddr *)&clntAddr,&clntAddrLen);
		while(1)
		{
			readLen=read(clntSd,rBuff,sizeof(rBuff));
			if(!strcmp(rBuff,"END")||(readLen==0))
			{
				close(clntSd);
				break;
			}
			rBuff[readLen-1]='\0';
			printf("client: %s \n",rBuff);
		}
	}
	close(srvSd);
	return 0;
}
