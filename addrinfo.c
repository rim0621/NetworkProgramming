#include <sys/types.h>
#include <sys/socket.h>
#include<netdb.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/in.h>
#include<errno.h>

void errProc(const char *);
int main(int argc,char **argv)
{
	int res;
	int sock;
	struct addrinfo *results, *pInfo;
	struct addrinfo hint;
	struct sockaddr_in *addr;
	struct sockaddr_in6 *addr6;
	char strAddr[INET6_ADDRSTRLEN];
	if(argc!=3)
	{
		fprintf(stderr,"usage: %s<name> <port> \n",argv[0]);
		return -1;
	}

	memset(&hint,0,sizeof(hint));
	hint.ai_family=AF_UNSPEC;
	hint.ai_socktype=SOCK_STREAM;
	res=getaddrinfo(argv[1],argv[2],&hint,&results);
	if(res!=0) errProc("getaddrinfo");

	for(pInfo=results; pInfo != NULL; pInfo=pInfo->ai_next)
	{
		if(pInfo->ai_family==AF_INET)
		{
			addr=(struct sockaddr_in *)pInfo->ai_addr;
			inet_ntop(AF_INET, &(addr->sin_addr),strAddr,INET_ADDRSTRLEN);
			printf("IPv4: %s\n",strAddr);
		}
		else if(pInfo->ai_family==AF_INET6)
		{
			addr6=(struct sockaddr_in6 *) pInfo->ai_addr;
			inet_ntop(AF_INET6,&(addr6->sin6_addr),strAddr,INET6_ADDRSTRLEN);
			printf("IPv6: %s\n",strAddr);
		}
	}
	freeaddrinfo(results);
	return 0;
}

void errProc(const char* str)
{
	fprintf(stderr,"%s: %s\n",str,strerror(errno));
	exit(errno);
}

