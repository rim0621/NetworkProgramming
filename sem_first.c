#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<sys/wait.h>
#include<string.h>
#include<unistd.h>
#include<semaphore.h>

#define SM_NAME "/Test"
#define SEM_NAME "/SemTest"
//#define SEM_FAILED ((sem_t *)(-1))
int main(int argc, char **argv)
{
	int shmfd, pid, status,i;
	int *test_value;
	sem_t *sem;

	sem=sem_open(SEM_NAME,O_RDWR|O_CREAT,0777,1);
	if(sem==SEM_FAILED)
	{
		fprintf(stderr,"Sem_open Error \n");
		exit(1);
	}

	shmfd=shm_open(SM_NAME,O_RDWR | O_CREAT, 0777);
	if(shmfd==-1)
	{
		fprintf(stderr,"shm open error \n");
		exit(1);
	}

	if(ftruncate(shmfd, sizeof(int))==-1)
	{
		fprintf(stderr,"truncate Error \n");
		exit(1);
	}

	test_value=mmap(0,sizeof(int), PROT_READ|PROT_WRITE,MAP_SHARED,shmfd,0);
	close(shmfd);

	*test_value=10;
	printf("Test value : %d\n",*test_value);

	pid=fork();
	if(pid==0)
	{
		for(i=0;i<10;i++)
		{
			sem_wait(sem);
			*test_value=*test_value +1;
			printf("child process test value : %d \n",*test_value);
			sem_post(sem);
		}
		exit(1);
	}
	else if(pid<0)
	{
		fprintf(stderr,"fork failure \n");
		exit(1);
	}
	else
	{
		for(i=0;i<10;i++)
		{
			sem_wait(sem);
			*test_value=*test_value +5;
			printf("parent process test value: %d\n",*test_value);
			sem_post(sem);
		}
		wait(&status);
	}

	shm_unlink(SM_NAME);
	sem_unlink(SEM_NAME);
	return 0;
}
