#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netinet/tcp.h>
#include<linux/ip.h>
#include<string.h>

int main(int argc,char **argv)
{
	int tcpSd,clntSd;
	socklen_t optLen;
	int optVal;
	struct sockaddr_in srvAddr,clntAddr;
	int clntAddrLen, readLen,strLen;
	char rBuff[BUFSIZ];

	tcpSd=socket(PF_INET,SOCK_STREAM,0);

	optLen=sizeof(optVal);
	getsockopt(tcpSd,IPPROTO_IP,IP_TOS,(void*)&optVal,&optLen);
	printf("IP TOS of tcpSd: %d\n",optVal);

	optVal=IPTOS_THROUGHPUT;
	setsockopt(tcpSd,IPPROTO_IP,IP_TOS,(void *)&optVal,optLen);

	getsockopt(tcpSd,IPPROTO_IP,IP_TOS,(void*)&optVal,&optLen);
        printf("IP TOS of tcpSd: %d\n",optVal);

	if(argc!=2)
	{
		printf("usage:%s [port number]\n",argv[0]);
		return -1;
	}

	printf("server start....\n");
	if(tcpSd==-1)
	{
		fprintf(stderr,"socket creation error");
		return -1;
	}

	memset(&srvAddr,0,sizeof(srvAddr));
	srvAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	srvAddr.sin_family=AF_INET;
	srvAddr.sin_port=htons(atoi(argv[1]));

	if(bind(tcpSd,(struct sockaddr *)&srvAddr,sizeof(srvAddr))==-1)
	{
		fprintf(stderr,"bind error");
		return -1;
	}

	if(listen(tcpSd,5)<0)
	{
		fprintf(stderr, "listen error");
		return -1;
	}

	while(1)
	{
		clntSd=accept(tcpSd,(struct sockaddr*)&clntAddr,&clntAddrLen);
		if(clntSd==-1)
		{
			fprintf(stderr,"accept error");
		}
		while(1)
		{
			readLen=read(clntSd,rBuff,sizeof(rBuff));
			rBuff[readLen]='\0';
			printf("client: %s \n",rBuff);
			write(clntSd,rBuff,readLen+1);
			if(!strcmp(rBuff,"END"))
			{
				close(clntSd);
				break;
			}
		}
	}
	close(tcpSd);
	return 0;
}


