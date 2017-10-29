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
	int pin[] = {1,2,3,4,5,6};
	int sum=0;
	int mul=1;
	int i = 0;

	pid_t pid;	// variable to store process identification numbers.

	pid = fork();

	if(pid < 0)
		exit(-2); // exit code for not working fork.


	if(pid == 0)
	{
		printf("child proccess, adding.\n");
		for(i = 0; i < 6; i++)
			sum+=pin[i];
		printf("sum is %d\n",sum);	
	}
	else
	{	

		printf("parent, multiplying.\n");
		for(i = 0; i < 6; i++)
			mul*=pin[i];
		printf("mul is %d\n",mul);
		wait(0);
	}
	return 0;
}
