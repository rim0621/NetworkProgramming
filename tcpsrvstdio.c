#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>

int main(int argc,char **argv)
{
	int listenSd,connectSd;
	struct sockaddr_in srvAddr,clntAddr;
	int clntAddrLen,readLen,strLen,state;
	char rBuff[BUFSIZ];
	FILE *rfp;

	if(argc!=2)
	{
		printf("usage:%s[prot number]\n",argv[0]);
		return -1;
	}

	printf("server start ....\n");
	listenSd=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);

	memset(&srvAddr,0,sizeof(srvAddr));
	srvAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	srvAddr.sin_family=AF_INET;
	srvAddr.sin_port=htons(atoi(argv[1]));

	bind(listenSd,(struct sockaddr*)&srvAddr, sizeof(srvAddr));
	listen(listenSd,5);

	clntAddrLen=sizeof(clntAddr);

	connectSd=accept(listenSd,(struct sockaddr*)&clntAddr, &clntAddrLen);
	fprintf(stderr,"A client is connected....\n");
	rfp=fdopen(connectSd,"r");

	while(!feof(rfp))
	{
		fgets(rBuff,BUFSIZ,rfp);
		printf("%s",rBuff);
	}
	fprintf(stderr,"the client is disconnected.\n");
	fclose(rfp);
	return 0;
}

