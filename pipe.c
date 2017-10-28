#include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<fcntl.h>
#include<stdlib.h>
void errProc(const char*);

int main(int argc,char **argv)
{
	int pipeFd[2];
	int res;
	char buff[BUFSIZ];
	pid_t pid;
	int readLen, nWrite;
	int openFd, status;

	if(argc !=2){
		fprintf(stderr,"Usage: %s [file] \n" , argv[0]);
	}
	res=pipe(pipeFd);
	if(res==-1) errProc("pipe");

	pid=fork();
	if(pid==-1) errProc("fork");

	if(pid==0)
	{
		close(pipeFd[1]);
		openFd=open(argv[1],O_WRONLY|O_CREAT|O_TRUNC); //make ONLY writing file
		while(1)
		{
			readLen=read(pipeFd[0],buff,BUFSIZ-1);
			if(readLen==-1) errProc("read");
			if(readLen==0) break;
			write(openFd,buff,readLen);
		}
		printf("parent process closed the pipe.\n");
		close(openFd);
		close(pipeFd[0]);		
		return 0;
	}
	else
	{
		close(pipeFd[0]);
		while(1)
		{
			fgets(buff,BUFSIZ-1,stdin);
			readLen=strlen(buff);
			if(readLen==4&&!strncmp(buff,"END",3))
				break;
			nWrite=write(pipeFd[1],buff,readLen);
			if(nWrite==-1) errProc("write");
			printf("%d bytes are written \n",nWrite);
		}
		close(pipeFd[1]);
		wait(&status);
		return 0;
	}
}

void errProc(const char* str)
{
        fprintf(stderr,"%s : %s \n", str,strerror(errno));
        exit(1);
}


