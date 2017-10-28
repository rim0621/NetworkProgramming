#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<stdlib.h>
#include<fcntl.h>

#define SEM_NAME "/Test"

void *threadA_main(void *arg);
void *threadB_main(void *arg);

static sem_t* sem;
static int counter=0;

int main(int argc,char **argv)
{
	pthread_t thread_id_1, thread_id_2;
	int res;
	sem=sem_open(SEM_NAME,O_RDWR|O_CREAT,0777,1);
	if(sem==SEM_FAILED)
	{
		fprintf(stderr,"sem_open error\n");
		exit(1);
	}
	if(pthread_create(&thread_id_1,NULL,threadA_main,NULL)!=0)
	{
		fprintf(stderr, "thread 1 creation error \n");
		exit(0);
	}
	 if(pthread_create(&thread_id_2,NULL,threadB_main,NULL)!=0)
        {
                fprintf(stderr, "thread 2 creation error \n");
                exit(0);
        }
	if(pthread_join(thread_id_1,(void **)&res)!=0)
	{
		fprintf(stderr,"pthread 1 join error\n");
		exit(0);
	}
       if(pthread_join(thread_id_2,(void **)&res)!=0)
        {
                fprintf(stderr,"pthread 2 join error\n");
                exit(0);
        }

	sem_unlink(SEM_NAME);
	return 0;
}

void *threadB_main(void *arg)
{
	int i;
	for(i=0;i<60000;i++)
	{
		sem_wait(sem);
		counter+=2;
		printf(" thread A increase the counter by 2: counter - %d \n",counter);
		sem_post(sem);
	}
	return NULL;
}
void *threadA_main(void *arg)
{
        int i;
        for(i=0;i<60000;i++)
        {
                sem_wait(sem);
                counter+=3;
                printf(" thread B increase the counter by 3: counter - %d \n",counter);
                sem_post(sem);
	}
	return NULL;
}
