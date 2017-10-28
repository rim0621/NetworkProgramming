#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<netinet/in.h>

int main(int argc, char**argv)
{
	int af,res;
	char tmpv6[INET6_ADDRSTRLEN];
	char buf[sizeof(struct in6_addr)];
	if(argc!=3)
	{
		fprintf(stderr,"usage: %s [ipv4 or ipv6] [ipaddr]\n",argv[0]);
		return -1;
	}
	if(!strcmp(argv[1],"IPv4")) af=AF_INET;
	else if(!strcmp(argv[1],"IPv6")) af=AF_INET6;
	else af=atoi(argv[1]);
	printf("size of struct in6_addr : %d \n",sizeof(struct in6_addr));
	printf("size of struct in_addr: %d\n",sizeof(struct in_addr));
	printf("size of macro INET6_addrstrlen : %d\n",INET6_ADDRSTRLEN);
	printf("size of macro INET_ADDRSTRLEN: %d \n" ,INET_ADDRSTRLEN);
	res=inet_pton(af,argv[2],buf);
	if(res==0) fprintf(stderr, "address format is not vaild.\n");
	if(res<0) fprintf(stderr,"address family is not ipv4 of ipv6 \n");
	if(res<=0) return -1;
	if(inet_ntop(af,buf,tmpv6,INET6_ADDRSTRLEN)==NULL)
	{
		fprintf(stderr,"inet_ntop error\n");
		return -1;
	}
	printf("%s\n",tmpv6);
	return 1;
}
