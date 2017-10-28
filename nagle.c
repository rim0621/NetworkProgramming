#include<stdio.h>
#include<sys/socket.h>
#include<netinet/tcp.h>
#include<netinet/in.h>

int main(int argc,char **argv)
{
	int tcpSd;
	socklen_t optLen;
	int optVal;

	tcpSd=socket(PF_INET,SOCK_STREAM,0);

	optLen=sizeof(optVal);
	getsockopt(tcpSd,IPPROTO_TCP, TCP_NODELAY,(void*)&optVal,&optLen);
	printf("TCP nodelay option of tcpSd: %d\n",optVal);
	optVal=1;
	setsockopt(tcpSd,IPPROTO_TCP,TCP_NODELAY,(void*)&optVal,optLen);

	getsockopt(tcpSd,IPPROTO_TCP,TCP_NODELAY,(void*)&optVal,&optLen);
	printf("tcp nodelay option of tcpSd: %d\n");

	close(tcpSd);
	return 0;
}
