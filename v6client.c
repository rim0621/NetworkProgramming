#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<errno.h>

void errProc(char *);

int main(int argc,char **argv)
{
	int socketSd, serverPort;
	struct sockaddr_in6 ipv6Addr;
	struct addrinfo hint;
	struct addrinfo *results,*pInfo;
	int res,i,j;
	char rBuff[BUFSIZ];
	int readLen;

	if(argc!=3)
	{
		fprintf(stderr,"usage:%s <IPADDRESS> <PORT> \n",argv[0]);
		exit(1);
	}
	serverPort=atoi(argv[2]);
	memset(&ipv6Addr,0,sizeof(ipv6Addr));
	j=i=0;
	memset(&hint,0,sizeof(hint));
	hint.ai_socktype=SOCK_STREAM;
	hint.ai_protocol=IPPROTO_TCP;
	res=getaddrinfo(argv[1],argv[2],&hint,&results);
	if(res!=0) errProc("getaddrinfo");
	for(pInfo=results; pInfo !=NULL; pInfo= pInfo->ai_next)
	{
		j++;
		socketSd=socket(pInfo->ai_family,pInfo->ai_socktype, pInfo->ai_protocol);
		res=connect(socketSd,pInfo->ai_addr,pInfo->ai_addrlen);
		if(res<0)
		{
			fprintf(stderr,"connection failure(%s)",argv[1]);
			close(socketSd);
		}
		else{
			i++;
			break;
		}
	}
	freeaddrinfo(results);
	if(i<0)
	{
		fprintf(stderr,"%d results \n",j);
		close(socketSd);
		exit(1);
	}
	while(1)
	{
		fgets(rBuff,BUFSIZ-1,stdin);
		readLen = strlen(rBuff);
		if(readLen<=0) errProc("fgets");
		write(socketSd, rBuff,readLen-1);
		res=read(socketSd, rBuff,BUFSIZ);
		if(res<0) errProc("read");
		if(res==0) break;
		write(1,rBuff,readLen-1);
	}

	close(socketSd);
	return 0;
}
void errProc(char *str)
{
	fprintf(stderr,"%s : %s \n",str,strerror(errno));
	exit(1);
}

