#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<sys/wait.h>
#include<string.h>
#include<unistd.h>

#define SM_NAME "/Test"

int main(int argc, char **argv)
{
	int shmfd;
	int pid;
	int status;
	int *test_value;

	shmfd=shm_open(SM_NAME,O_RDWR | O_CREAT,0777);
	if(shmfd==-1)
	{
		fprintf(stderr,"open error\n");
		exit(1);
	}

	if(ftruncate(shmfd,sizeof(int))==-1)
	{
		fprintf(stderr,"Truncate Error]\n");
		exit(1);
	}

	test_value = mmap(0,sizeof(int),PROT_READ|PROT_WRITE,MAP_SHARED,shmfd,0);
	close(shmfd);

	*test_value=10;
	printf("Test value: %d \n", *test_value);

	pid=fork();
	if(pid==0)
	{
		*test_value= *test_value + 1;
		printf("child process test value: %d\n", *test_value);
		sleep(3);
		*test_value= *test_value + 1;
		printf("child process test value: %d\n", *test_value);
		exit(1);
	}
	else if(pid<0)
	{
		fprintf(stderr,"Fork Failure \n");
		exit(1);
	}
	else
	{
		sleep(1);
		*test_value= *test_value + 5;
		printf("Parent process test value: %d\n", *test_value);
		wait(&status);
	}

	shm_unlink(SM_NAME);
	return 0;
}

