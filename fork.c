#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

int main(int argc,char **argv)
{
	int count=0;
	int status;
	pid_t pid=fork();
	
	if(pid==0)
	{
		printf("A child\n");
		count=count+1;
		printf("childe : %d \n",count);
		sleep(15);
		return 3;
	}
	else if(pid<0)
	{
		printf("fork erro\n");
		exit(5);
	}
	else
	{
		wait(&status);
		if(WIFEXITED(status))
		{
			printf("child returns %d\n" ,WEXITSTATUS(status));
		}
		printf("parent \n");
		printf("parent : %d\n",count);
	}


	return 0;
}
