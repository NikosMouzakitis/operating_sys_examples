/*	Dining philosopher's problem with 5 threads within' the same process.
 *									    *
 *	compile:		ggc main.c -lpthread			    *
*/	

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define N 5
		//still in this solution one philosopher eats in the table every momment.
int total = 17;	// initializing total food amount.
int ate[N];	// food consumed by each philosopher.

pthread_mutex_t turn;
pthread_t philosophers[N];

int ismax(int id)
{
	int i;
	int max;
	int cnt =0;

	max = ate[0];

	for(i = 0; i < N; i++)
	{
		if(ate[i] > max)
			max = ate[i];
	}
	
	for(i = 0; i < N; i++)
		if(ate[i] == ate[id])
			cnt++;
	if(cnt == N)
		return 2;	// optional signal,when every philosopher ate the same amount.

	if(max == ate[id])
		return 1;

	return 0;
}

void * eat(void *tid)
{
	long myid = (long) tid;
	
	while(1)
	{
		pthread_mutex_lock(&turn);

		if(total == 0)
		{
			pthread_mutex_unlock(&turn);
			break;
		}

		if ( (ismax(myid) == 1)   )
		{
			pthread_mutex_unlock(&turn);
			pthread_yield();
			continue;
		}

//		printf("philosopher[%d] just ate.\n",myid);
		total -=1;
		ate[myid] +=1;
		pthread_mutex_unlock(&turn);
		pthread_yield();
	}
}

int main(int argc,char *argv[],char *envp[])
{
	int i;
	
	pthread_attr_t attr;
	pthread_mutex_init(&turn,NULL);
	pthread_attr_init(&attr);


	for(i = 0; i < N; i++)
	{
		pthread_create(&philosophers[i],&attr,eat,(void *)i);
		printf("created a philosopher \n");
	}
	
	for(i = 0; i < N; i++)
		pthread_join(philosophers[i],NULL);
	
	for(i = 0; i < N;i++)
		printf("Philosopher %d ate total %d\n",i,ate[i]);

	pthread_mutex_destroy(&turn);

	return 0;
}
