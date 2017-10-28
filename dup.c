#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<fcntl.h>
#include<errno.h>

int main(int argc, char **argv)
{
	int fd1,fd2,fd3;
	char buff[BUFSIZ];
	int readLen,nWrite;
	if(argc!=2)
	{
		printf("usage: %s filename \n",argv[0]);
		return -1;
	}
	fd1=open(argv[1],O_RDWR|O_CREAT|O_TRUNC);
	fd2=open(argv[1],O_RDONLY);
	fd3=dup(fd1);

	fgets(buff,BUFSIZ-1,stdin);
	readLen=strlen(buff);
	nWrite=write(fd1,"abc",3);
	nWrite+=write(fd1,buff,readLen+1);
	if(nWrite==-1) 
	{
		fprintf(stderr,"nWrite error");
		exit(1);
	}

	printf("Offset (fd1) : %d \n",(int) lseek(fd1,0,SEEK_CUR));
	printf("Offset (fd2) : %d \n",(int) lseek(fd2,0,SEEK_CUR));
	printf("Offset (fd3) : %d \n",(int) lseek(fd3,0,SEEK_CUR));

	read(fd2,buff,4);
	write(1,buff,4);

	printf("---------------------\n");
	printf("Offset (fd1) : %d \n",(int) lseek(fd1,0,SEEK_CUR));
	printf("Offset (fd2) : %d \n",(int) lseek(fd2,0,SEEK_CUR));
	printf("Offset (fd3) : %d \n",(int) lseek(fd3,0,SEEK_CUR));

	close(fd1);
	fgets(buff,BUFSIZ-1,stdin);
	readLen=strlen(buff);
	nWrite+=write(fd3,buff,readLen);

	lseek(fd2,0,SEEK_SET);
	memset(buff,0,BUFSIZ);
	readLen=read(fd2,buff,nWrite);
	write(1,buff,nWrite);
	close(fd2);
	close(fd3);

	return 0;
}

