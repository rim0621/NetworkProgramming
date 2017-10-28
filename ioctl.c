#include<stdio.h>
#include<netpacket/packet.h>
#include<net/if.h>
#include<errno.h>
#include<sys/socket.h>
#include<string.h>
#include<sys/ioctl.h>
#include<linux/if_ether.h>
#include<stdlib.h>

void errProc(const char *);
int main(int argc,char** argv)
{
	int pSocket;
	struct sockaddr_ll lAddr;
	struct ifreq ifr;
	size_t ifNameLen;
	int res,i,index;

	if(argc!=2){
		fprintf(stderr,"usage:%s <interface> \n",argv[0]);
		return -1;
	}

	printf("IFNAMSIZ: %d\n",IFNAMSIZ);
	ifNameLen=strlen(argv[1]);
	if(ifNameLen>sizeof(ifr.ifr_name)) errProc("interface name is long~\n");
	memcpy(ifr.ifr_name,argv[1],ifNameLen);
	ifr.ifr_name[ifNameLen]=0;
	printf("argv[1]:%s %d \n",ifr.ifr_name,ifNameLen);
	pSocket=socket(AF_INET,SOCK_DGRAM,0);
	res=ioctl(pSocket,SIOCGIFINDEX,&ifr);
	if(res<0) errProc("ioctl");
	printf("interface %s index: %d \n",argv[1],ifr.ifr_ifindex);

	index=ifr.ifr_ifindex;
	memset(&ifr,0,sizeof(ifr));
	ifr.ifr_ifindex=index;
	res=ioctl(pSocket,SIOCGIFNAME,&ifr);
	if(res<0) errProc("ioctl");
	printf("interface name : %s \n",ifr.ifr_name);

	res=ioctl(pSocket,SIOCGIFHWADDR,&ifr);
	if(res<0) errProc("ioctl");
	for(i=0;i<6;i++)
		printf("%02x",(unsigned char) ifr.ifr_addr.sa_data[1]);
	printf("\n");
	close(pSocket);
	return 0;
}

void errProc(const char *str)
{
	fprintf(stderr,"%s : %s \n",str,strerror(errno));
	exit(errno);
}

