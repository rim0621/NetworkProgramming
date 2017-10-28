#include<stdio.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>

void errProc();
int main(int argc, char **argv)
{
	int clntSd;
	struct sockaddr_in clntAddr;
	int clntAddrLen,readLen,recvByte,maxBuff;
	char wBuff[BUFSIZ];
	char rBuff[BUFSIZ];
	if(argc!=3)
	{
		printf("usage:%s[ip address] [port] \n",argv[0]);
	}
	clntSd=socket(AF_INET,SOCK_STREAM,0);
	if(clntSd==-1) errProc();
	printf("==================client program===============\n");

	memset(&clntAddr,0,sizeof(clntAddr));
	clntAddr.sin_family=AF_INET;
	clntAddr.sin_addr.s_addr=inet_addr(argv[1]);
	clntAddr.sin_port=htons(atoi(argv[2]));

	if(connect(clntSd,(struct sockaddr *)&clntAddr,sizeof(clntAddr))==-1)
	{
		close(clntSd);
		errProc();
	}

	while(1)
	{
		fgets(wBuff,BUFSIZ-1,stdin);
		readLen=strlen(wBuff);
		if(readLen<2) continue;
		write(clntSd,wBuff,readLen-1);
		recvByte=0;
		maxBuff=BUFSIZ-1;
		do{
			recvByte+=read(clntSd,rBuff,maxBuff);
			maxBuff-=recvByte;
		}while(recvByte<(readLen-1));
		rBuff[recvByte]='\0';
		printf("server: %s \n",rBuff);
		wBuff[readLen-1]='\0';
		if(!strcmp(wBuff,"END")) break;
	}

	printf("END^^\n");
	close(clntSd);

	return 0;
}
void errProc()
{
	fprintf(stderr,"error : %s \n", strerror(errno));
	exit(errno);
}
