#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>
#include<fcntl.h>

int listenSd,connectSd;
void urgent_handler(int sig);

int main(int argc,char **argv)
{
	struct sockaddr_in srvAddr,clntAddr;
	int clntAddrLen, readLen,strLen,state;
	char rBuff[BUFSIZ];
	pid_t pid;
	struct sigaction act;

	if(argc!=2)
	{
		printf("Usage: %s [port number]\n",argv[0]);
		return -1;
	}

	act.sa_handler=urgent_handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags=0;

	printf("server start.....\n");
	listenSd=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);

	memset(&srvAddr,0,sizeof(srvAddr));
	srvAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	srvAddr.sin_family=AF_INET;
	srvAddr.sin_port=htons(atoi(argv[1]));

	bind(listenSd,(struct sockaddr *) &srvAddr,sizeof(srvAddr));
	listen(listenSd,5);

	clntAddrLen=sizeof(clntAddr);

	connectSd=accept(listenSd,(struct sockaddr *)&clntAddr, &clntAddrLen);
	fprintf(stderr,"A client is connected ....\n");

	fcntl(connectSd,F_SETOWN,getpid());
	state=sigaction(SIGURG,&act,0);

	while(1)
	{
		readLen=recv(connectSd,rBuff,sizeof(rBuff),0);
		rBuff[readLen]='\0';
		printf("client : %s \n",rBuff);
		if(readLen==0)
		{
			close(connectSd);
			break;
		}
	}
	fprintf(stderr,"the  client is disconnected \n");
	close(listenSd);
	return 0;
}

void urgent_handler(int sig)
{
	int readLen;
	char rBuff[BUFSIZ];

	readLen=recv(connectSd, rBuff,sizeof(rBuff),MSG_OOB);
	rBuff[readLen]='\0';
	printf("client(urgent): %s \n",rBuff);
}
