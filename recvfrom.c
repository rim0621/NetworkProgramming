#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
void errProc(const char*);
int main(int argc, char ** argv)
{
	int mySock,readLen,nRecv,res;
	char buff[BUFSIZ];
	struct sockaddr_in src#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
void error_handling(char *message);
 
int main(int argc, char *argv[])
{
    int serv_sock;
    int clnt_sock;
     
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;
     
    char message[]="Hello world!";
     
 #include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
void error_handling(char *message);
 
int main(int argc, char *argv[])
{
    int serv_sock;
    int clnt_sock;
     
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;
     
    char message[]="Hello world!";
     
    if(argc!=2)
    {
        printf("Usage : %s <port>\n",argv[0]);
        exit(1);
    }
     
    serv_sock=socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
    {
        error_handling("socket() error!");
    }
     
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_addr.sin_port=htons(atoi(argv[1]));
     
    if(bind(serv_sock, (struct sockaddr*) & serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");
     
    if(listen(serv_sock, 5) == -1)
        error_handling("listen() error");
         
    clnt_addr_size = sizeof(clnt_addr);
    clnt_sock=accept(serv_sock, (struct sockaddr*) & clnt_addr, &clnt_addr_size);
    if(clnt_sock == -1)
        error_handling("accept() error");
     
    write(clnt_sock, message, sizeof(message));
    close(clnt_sock);
    close(serv_sock);
    return 0;
}
 
void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}   if(argc!=2)
    {
        printf("Usage : %s <port>\n",argv[0]);
        exit(1);
    }
     
    serv_sock=socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
    {
        error_handling("socket() error!");
    }
     
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_addr.sin_port=htons(atoi(argv[1]));
     
    if(bind(serv_sock, (struct sockaddr*) & serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");
     
    if(listen(serv_sock, 5) == -1)
        error_handling("listen() error");
         
    clnt_addr_size = sizeof(clnt_addr);
    clnt_sock=accept(serv_sock, (struct sockaddr*) & clnt_addr, &clnt_addr_size);
    if(clnt_sock == -1)
        error_handling("accept() error");
     
    write(clnt_sock, message, sizeof(message));
    close(clnt_sock);
    close(serv_sock);
    return 0;
}
 
void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}Addr,destAddr;
	socklen_t addrLen;
	if(argc!=2){
		fprintf(stderr,"usage: %s Port",argv[0]);
		return 0;
	}
	mySock=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	if(mySock==-1) errProc("socket");
	memset(&srcAddr,0,sizeof(srcAddr));
	srcAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	srcAddr.sin_family=AF_INET;
	srcAddr.sin_port=htons(atoi(argv[1]));

	res=bind(mySock,(struct sockaddr *) &srcAddr, sizeof(srcAddr));
	if(res==-1) errProc("bind");
	addrLen=sizeof(destAddr);
	while(1)
	{
		//recvfrom is byte of message
		nRecv=recvfrom(mySock,buff,BUFSIZ-1,0,(struct sockaddr* ) &destAddr,&addrLen);
		if(nRecv==-1) errProc("recvfrom");
		printf("%d bytes were recv \n",nRecv);
	}
	return 0;

}

void errProc(const char *str)
{
	fprintf(stderr, "%s : %s\n",str,strerror(errno));
	exit(1);
}
