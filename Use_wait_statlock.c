#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc,char *argv[])
{
	pid_t pid;

	pid = fork();
	
	if(pid < 0)
		exit(-2);
	if(pid == 0)
	{
		exit(9);
	}
	if(pid > 0)
	{
		int *p;

	        int l = wait(p);
		printf(" %d %d\n",pid,getpid());
		printf("l = %d\n",l);
		printf("%d statlock value\n",*p);
	}

	return 0;
}