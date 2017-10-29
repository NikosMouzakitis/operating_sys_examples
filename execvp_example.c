#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/wait.h>
int main(int argc,char *argv[],char *envp[])
{
	char *args[4];
	char text[3];
	strcpy(text,"ls");
	
	args[0] = malloc(sizeof(strlen(text) + 1));
	pid_t pid;

	strcpy(args[0],text);

	pid = fork();

	if(pid == 0)
	{
		sleep(1);
		printf("%d i am child with pid\n",getpid());
		execvp("/usr/bin/ls",args);
		//	anything writen here will never execute.
		printf("this will never show up\n");
		
	}
	else if(pid > 0 )
	{
		int o;
		o = wait(0);	// wait returns the id of the proccess that terminated.
		printf("i am parent finishing after %d finihed now.\n",o);
	}

	return 0;
}
