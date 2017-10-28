#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>

void *init_thread(void *parm)
{
	int i;
	for(i=1;i<10;i++)
	{
		printf("counter: %d\n",i);
		sleep(1);
	}
	printf("thread is now terminated \n");
}
int main(int argc,char **argv)
{
	pthread_t thread_id;
	int res;
	if(pthread_create(&thread_id,NULL,init_thread,NULL)!=0)
	{
		fprintf(stderr,"thread creation error\n");
		exit(0);
	}

	if(pthread_join(thread_id,(void **)&res)!=0)
	{
		fprintf(stderr,"pthread join error\n");
		exit(0);
	}

	printf("main founction is terminated. \n");
	return 0;
}
