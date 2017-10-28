#include<mqueue.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<signal.h>

#define MQ_NAME "/test_mq"
#define MQ_FAILED ((mqd_t) -1)
#define PORT 9001

void chld_handler(int sig)
{
	int status;
	pid_t cpid=wait(&status);
	if(WIFEXITED(status))
	{
		printf("child process %d is terminated \n",cpid);
	}
}

int main(int argc, char **argv)
{
	int listenSd, connectSd;
	struct sockaddr_in srvAddr,clntAddr;
	int clntAddrLen, readLen,strLen;
	char rBuff[BUFSIZ];
	char *tmp;
	int nSockOpt;
	pid_t pid, checker_pid;
	mqd_t mqd;
	int priority=10;
	struct mq_attr attr;
	attr.mq_flags=0;
	attr.mq_maxmsg=100;
	attr.mq_msgsize=50;

	struct sigaction act;
	act.sa_handler=chld_handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags=0;
	sigaction(SIGCHLD,&act,0);

	if(argc !=2)
	{
		printf("usage: %s [port number]\n",argv[0]);
		return -1;
	}

	mqd=mq_open(MQ_NAME,O_RDWR|O_CREAT, 0777,&attr);
	if(mqd==MQ_FAILED)
	{
		fprintf(stderr,"MQ_open error\n");
		exit(1);
	}

	checker_pid=fork();
	if(checker_pid==0)
	{
		if(mq_getattr(mqd,&attr)==-1)
		{
			fprintf(stderr,"MQ_ATTR error\n");
			exit(1);
		}
		tmp=malloc(attr.mq_msgsize);
		while(1)
		{
			readLen=mq_receive(mqd,tmp,attr.mq_msgsize,&priority);
			printf("checker: input length is %d\n",readLen);
		}
		exit(1);
	}
	printf("Server start...\n");
	listenSd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(listenSd==-1)
	{
		fprintf(stderr,"Socket creation error");
		return -1;
	}
	nSockOpt=1;
        setsockopt(listenSd,SOL_SOCKET,SO_REUSEADDR, &nSockOpt,sizeof(nSockOpt));

	
	memset(&srvAddr,0,sizeof(srvAddr));
	srvAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	srvAddr.sin_family=AF_INET;
	srvAddr.sin_port=htons(atoi(argv[1]));


	int a=bind(listenSd,(struct sockaddr *) &srvAddr,sizeof(srvAddr));
	if(a==-1)
	{
		fprintf(stderr,"Bind error");
		return -1;
	}
	if(listen(listenSd,5)<0)
	{
		fprintf(stderr,"Listen error");
		return -1;
	}
	
	clntAddrLen=sizeof(clntAddr);
	while(1)
	{
		connectSd=accept(listenSd,(struct sockaddr*) &clntAddr, &clntAddrLen);
		if(connectSd==-1)
		{
			continue;
		}
		else
		{	
			fprintf(stderr," a client is connected...\n");
		}
		pid=fork();
		if(pid==0)
		{
			close(listenSd);
			while(1)
			{
				readLen=read(connectSd,rBuff,sizeof(rBuff));
				if(readLen==0)
				{
					break;
				}
				rBuff[readLen-1]='\0';
				printf("client : %s \n", rBuff);
				if(mq_send(mqd,rBuff,strlen(rBuff),priority)==-1)
				{
					fprintf(stderr,"MQ_Send Error\n");
					exit(1);
				}
			}
			close(connectSd);
			fprintf(stderr,"the client is disconnected .\n");
			return 0;
		}
		else if(pid==-1)
		{
			fprintf(stderr,"fork error");
			continue;
		}
		else
		{
			close(connectSd);
		}
	}
	close(listenSd);
	return 0;
}
