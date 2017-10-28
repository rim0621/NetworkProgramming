#include<mqueue.h>
#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>

#define MQ_NAME "/test_mq"
#define MQ_FAILED ((mqd_t) -1)

int main(int argc, char ** argv)
{
	mqd_t mqd;
	int pid;
	unsigned int priority=10;
	char sbuff[50];
	void *rbuff;
	int readLen;
	struct mq_attr attr;
	attr.mq_flags=0;
	attr.mq_maxmsg=100;
	attr.mq_msgsize=50;

	mqd=mq_open(MQ_NAME,O_RDWR | O_CREAT, 0777,&attr);
	if(mqd==MQ_FAILED)
	{
		fprintf(stderr,"MQ_open error \n");
		exit(1);
	}

	pid=fork();
	if(pid==0)
	{
		printf("child: ");
		scanf("%s",sbuff);
		if(mq_send(mqd,sbuff,strlen(sbuff),priority)==-1)
		{
			fprintf(stderr,"MQ_send error\n");
			exit(1);
		}
	}
	else if(pid<0)
	{
		fprintf(stderr,"fork failure \n");
		exit(1);
	}
	else
	{
		if(mq_getattr(mqd,&attr)==-1)
		{
			fprintf(stderr,"MQ_GET_ATTR error\n");
			exit(1);
		}
		rbuff=malloc(attr.mq_msgsize);
		readLen=mq_receive(mqd,rbuff,attr.mq_msgsize, & priority);
		printf("parent recevies: %s\n",(char *)rbuff);
		exit(1);
	}
	return 0;
}
