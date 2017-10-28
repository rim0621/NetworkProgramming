#include<stdio.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<string.h>

int main(int argc,char **argv)
{
	int connectSd;
	struct sockaddr_in clntAddr;
	int clntAddrLen, readLen;
	char wBuff[BUFSIZ];
	FILE *wfp;

	if(argc!=3)
	{
		printf("usage:%s <IP address> <port number> \n",argv[0]);
		return -1;
	}
	connectSd=socket(PF_INET,SOCK_STREAM,0);
	printf("==========client program==============\n");
	memset(&clntAddr,0,sizeof(clntAddr));
	clntAddr.sin_family=AF_INET;
	clntAddr.sin_addr.s_addr=inet_addr(argv[1]);
	clntAddr.sin_port=htons(atoi(argv[2]));

	connect(connectSd,(struct sockaddr *)&clntAddr, sizeof(clntAddr));
	wfp=fdopen(connectSd,"w");
	while(1)
	{
		printf("send msg: ");
		fgets(wBuff,BUFSIZ,stdin);
		fputs(wBuff,wfp);
		fflush(wfp);
		if(!strcmp(wBuff,"END\n"))break;
	}
	fclose(wfp);

	return 0;
}

