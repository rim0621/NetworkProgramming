#define SPORT 90
#define DPORT 9000
#define IP_ADDRESS "127.0.0.1"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<netinet/in.h>
#include<netinet/ip.h>
#include<netinet/tcp.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<errno.h>

struct ip_hdr
{
 #if __BYTE_ORDER__==__LITTLE_ENDIAN

        uint8_t ip_hdr_len:4;
        uint8_t ip_version:4;
 #else
        uint8_t ip_version:4;
        uint8_t ip_hdr_len:4;
 #endif
        uint8_t ip_tos;
        uint16_t ip_len;

        uint16_t ip_id;
        uint16_t ip_off;

        uint8_t ip_ttl;
        uint8_t ip_proto;
        uint16_t ip_check;

        uint32_t ip_src;
        uint32_t ip_dst;
};

struct tcp_hdr
{
        uint16_t tcp_src;
        uint16_t tcp_dst;

        uint32_t tcp_seq;
        uint32_t tcp_ackno;

 #if __BYTE_ORDER__==__LITTLE_ENDIAN
        uint8_t tcp_rsv1:4;
        uint8_t tcp_hdr_len:4;
        uint8_t tcp_fin:1;
        uint8_t tcp_syn:1;
        uint8_t tcp_rst:1;
        uint8_t tcp_psh:1;
        uint8_t tcp_ack:1;
        uint8_t tcp_urg:1;
        uint8_t tcp_rsv2:2;
#else
        uint8_t tcp_hdr_len:4;
        uint8_t tcp_rsv1:4;
        uint8_t tcp_rsv2:2;
        uint8_t tcp_urg:1;
        uint8_t tcp_ack:1;
        uint8_t tcp_push:1;
        uint8_t tcp_rst:1;
        uint8_t tcp_syn:1;
        uint8_t tcp_fin:1;
#endif

        uint16_t tcp_win_size;

        uint16_t tcp_check;
        uint16_t tcp_urg_ptr;
};

struct usr_data
{
        uint16_t usr_id;
        uint16_t usr_len;
        uint32_t usr_data;
};


struct pseudo_hdr
{
	uint32_t src;
	uint32_t dst;
	uint8_t zeros;
	uint8_t proto;
	uint16_t len;
};

void errProc(const char*);
uint16_t checksum(const void *ptr,int len);
void parseTcpHeader(struct tcp_hdr *myHdr);

int main(int argc,char **argv)
{
	int socketSd;
	int size_tx_packet=sizeof(struct tcp_hdr)+sizeof(struct usr_data);
	int fromAddrLen;
	char rBuff[BUFSIZ];

	struct tcp_hdr* myTcp;
	struct pseudo_hdr *myPseudo;
	struct usr_data *myData;
	struct ip_hdr *myIp;

	struct in_addr srcAddr, destAddr;
	struct sockaddr_in sockAddr, fromAddr;

	int size_for_tcp_checksum=size_tx_packet + sizeof(struct pseudo_hdr);

	char* packet=(char*)malloc(size_for_tcp_checksum);
	myPseudo=(struct pseudo_hdr*) (packet);
	myTcp=(struct tcp_hdr*) (packet+sizeof(struct pseudo_hdr));
	myData=(struct usr_data*)(packet+sizeof(struct pseudo_hdr)+sizeof(struct tcp_hdr));

	if((socketSd=socket(PF_INET,SOCK_RAW,IPPROTO_TCP))<0)	errProc("socket");

	memset(packet,0,size_for_tcp_checksum);
	srcAddr.s_addr=inet_addr(IP_ADDRESS);
	destAddr.s_addr=inet_addr(IP_ADDRESS);

	myData->usr_id=1;
	myData->usr_len=16;
	myData->usr_data=1981;

	myPseudo->src=srcAddr.s_addr;
	myPseudo->dst=destAddr.s_addr;
	myPseudo->proto=IPPROTO_TCP;
	myPseudo->zeros=0;
	myPseudo->len=htons(size_tx_packet);

	myTcp->tcp_src=htons(SPORT);
	myTcp->tcp_dst=htons(DPORT);
	myTcp->tcp_seq=htons(rand()%time(NULL));
	myTcp->tcp_ackno=0;
	myTcp->tcp_hdr_len=5;
	myTcp->tcp_rsv1=0;
	myTcp->tcp_rsv2=0;
	myTcp->tcp_fin=0;
	myTcp->tcp_syn=1;
	myTcp->tcp_rst=0;
	myTcp->tcp_psh=0;
	myTcp->tcp_ack=0;
	myTcp->tcp_urg=0;
	myTcp->tcp_win_size=htons(1024);
	myTcp->tcp_check=0;
	myTcp->tcp_urg_ptr=0;
	myTcp->tcp_check=checksum(packet,size_for_tcp_checksum);

	sockAddr.sin_family=PF_INET;
	sockAddr.sin_addr=destAddr;
	sockAddr.sin_port=htons(DPORT);

	if(sendto(socketSd, myTcp,size_tx_packet, 0x0,(struct sockaddr*)&sockAddr,sizeof(sockAddr))<0) errProc("send error");

	myIp=(struct ip_hdr*) rBuff;
	myTcp=(struct tcp_hdr*)(rBuff+sizeof(struct ip_hdr));
	parseTcpHeader(myTcp);

	close(socketSd);

	return 0;
}

void parseTcpHeader(struct tcp_hdr * myHdr)
{

	printf("====================RECV tcp segmet==================");
	printf("source port : %d \n", ntohs(myHdr->tcp_src));
	printf("Destination port: %d\n",ntohs(myHdr->tcp_dst));
	printf("sequence no.t : %d\n",ntohs(myHdr->tcp_seq));
	printf("ACK no .: %d \n", ntohs(myHdr->tcp_ackno));
	printf("Flags : %c%c%c%c%c%c\n",(myHdr->tcp_fin?'F':'X'),(myHdr->tcp_syn?'S':'X'),(myHdr->tcp_rst?'R':'X'),(myHdr->tcp_psh?'P':'X'),(myHdr->tcp_ack?'A':'X'),(myHdr->tcp_urg?'U':'X'));
	printf("checksum: %X\n",ntohs(myHdr->tcp_check));
}
void errProc(const char *str)
{
	fprintf(stderr,"%s : %s\n",str,strerror(errno));
	exit(1);
}

uint16_t checksum(const void *ptr,int len)
{
	int sum=0;
	uint16_t answer=0;
	uint16_t *w=(uint16_t)ptr;
	int nleft=len;

	while(nleft>1){
		sum+=*w++;
		nleft -=2;
	}
	sum=(sum>>16)+(sum&0xFFFF);
	sum+=(sum>>16);
	answer=~sum;
	return (answer);
}
 
