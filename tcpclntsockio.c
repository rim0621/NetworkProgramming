#include<stdio.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<string.h>

int main(int argc,char **argv)
{
	int connectSd;
	int clntAddrLen, readLen;
	struct sockaddr_in clntAddr;
	char wBuff[BUFSIZ];

	if(argc !=3)
	{
		printf("usage: %s < IP Address> <port number>\n",argv[0]);
		return -1;
	}

	connectSd=socket(PF_INET,SOCK_STREAM,0);
	printf("===========client protgram=============\n");
	memset(&clntAddr,0,sizeof(clntAddr));
	clntAddr.sin_family=AF_INET;
	clntAddr.sin_addr.s_addr=inet_addr(argv[1]);
	clntAddr.sin_port=htons(atoi(argv[2]));

	connect(connectSd,(struct sockaddr *) &clntAddr,sizeof(clntAddr));

	send(connectSd,"Normal_MSG1", strlen("Normal_MSG1"),0);
	send(connectSd,"Normal_MSG2", strlen("Normal_MSG2"),0);
	send(connectSd,"Urgent_MSG", strlen("Urgent_MSG"),MSG_OOB);
	send(connectSd,"Normal_MSG3", strlen("Normal_MSG3"),0);
	close(connectSd);

	return 0;
}
