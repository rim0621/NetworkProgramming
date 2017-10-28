#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>

int main(int argc,char **argv)
{
	int testSd;
	int sizeRBuff,sizeSBuff;
	socklen_t optSLen,optRLen;
	optSLen=sizeof(sizeSBuff);
	optRLen=sizeof(sizeRBuff);

	testSd=socket(PF_INET,SOCK_DGRAM,0);

	getsockopt(testSd,SOL_SOCKET,SO_SNDBUF,(void*)&sizeSBuff, &optSLen);
	printf("socket send buffer size: %d\n",sizeSBuff);
	getsockopt(testSd,SOL_SOCKET,SO_RCVBUF,(void*)&sizeRBuff,&optRLen);
	printf("socket recevie buffer size: %d\n",sizeRBuff);

	sizeRBuff=1024;
	sizeSBuff=2048;

	setsockopt(testSd,SOL_SOCKET,SO_SNDBUF,(void*)&sizeSBuff, &optSLen);
	setsockopt(testSd,SOL_SOCKET,SO_RCVBUF,(void*)&sizeRBuff, &optRLen);

        getsockopt(testSd,SOL_SOCKET,SO_SNDBUF,(void*)&sizeSBuff, &optSLen);
        printf("socket send buffer size: %d\n",sizeSBuff);
        getsockopt(testSd,SOL_SOCKET,SO_RCVBUF,(void*)&sizeRBuff, &optRLen);
        printf("socket recevie buffer size: %d\n",sizeRBuff);

	return 0;
}
