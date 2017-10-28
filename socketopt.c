#include<stdio.h>
#include<sys/socket.h>

int main(int argc,char **argv)
{
	int tcpSd,udpSd;
	socklen_t optLen;
	int optVal;
	struct linger myLinger,resLinger;

	tcpSd=socket(PF_INET,SOCK_STREAM,0);
	udpSd=socket(PF_INET,SOCK_DGRAM,0);

	optLen=sizeof(optVal);
	getsockopt(tcpSd,SOL_SOCKET,SO_TYPE,(void*)&optVal,&optLen);
	printf("SO_STYLE of tcpSd: %d \n",optVal);
	getsockopt(udpSd,SOL_SOCKET,SO_TYPE,(void*)&optVal,&optLen);
	printf("SO_STYLE of udpSd: %d \n",optVal);
	

	getsockopt(tcpSd,SOL_SOCKET,SO_DEBUG,(void*)&optVal,&optLen);
	printf("SO_Debug of tcpSd: %d\n",optVal);
	optVal=1;
	setsockopt(tcpSd, SOL_SOCKET, SO_DEBUG, (void *)&optVal,optLen);
	getsockopt(tcpSd,SOL_SOCKET,SO_DEBUG,(void *)&optVal,&optLen);
	printf("so_debug of tcpSd : %d\n",optVal);
	
	myLinger.l_onoff=1;
	myLinger.l_linger=7;
	optLen=sizeof(myLinger);
	getsockopt(tcpSd,SOL_SOCKET,SO_LINGER,(void*)&resLinger,&optLen);
	printf("SO_LINGER - l_onoff: %d \n",resLinger.l_onoff);
	printf("SO_LINGER - l_linger: %d \n",resLinger.l_linger);
	setsockopt(tcpSd,SOL_SOCKET,SO_LINGER,(void *)&myLinger,optLen);
	getsockopt(tcpSd,SOL_SOCKET,SO_LINGER,(void *)&resLinger,&optLen);
	printf("SO_LINGER -l_onoff : %d \n",resLinger.l_onoff);
	printf("SO_LINGER -l_linger: %d\n",resLinger.l_linger);

	close(tcpSd);
	close(udpSd);
	return 0;
}

