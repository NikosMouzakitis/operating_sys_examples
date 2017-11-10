#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
			// User assigns some jobs placed in a queue and tags every job with a threads id
			// Every thread executes the job if the head of queue is tagged for it.
#define MAX_JOBS 5
#define THREADS_NO 3

pthread_t workers[THREADS_NO];
pthread_t oura;
pthread_mutex_t lock;
pthread_mutex_t initial_hash;

struct jobs{
	int worker_tid;	// worker's thread id.
	int jid;	// job id.
}queue[MAX_JOBS];

int hash_locked = 0;	// we use that to run only one time, and work correct.
int head = 0;		// queue's head.
int last = 0;		// queue's end.
int pin[THREADS_NO][THREADS_NO];	// matrix to connect thread ID's to argument passed on threads.

void * queue_manager(void * unused)
{
	int run = 1;	// ensure we run this function once.

	int worker_no,temp_last,jobs_no;

	while(run > 0)
	{
		pthread_mutex_lock(&lock);
		pthread_mutex_lock(&initial_hash);
	
		if(!hash_locked )	// make sure we got sorted the connection between thread ID's and pin matrix.
		{
			pthread_mutex_unlock(&lock);
			pthread_mutex_unlock(&initial_hash);	
			continue;
		}		

		printf("How many new jobs(1- %d)\n",MAX_JOBS);
		scanf("%d",&jobs_no);		// number of inserted jobs.
		last = jobs_no;		
		while(jobs_no > MAX_JOBS)
		{
			printf("Max jobs are %d\nEnter number of jobs:\n",MAX_JOBS);
			scanf("%d",&jobs_no);
			last = jobs_no;	
		}
		
		for(int i = 0; i < jobs_no; i++)
		{
			queue[head+i].jid = head+i;
			printf("Choose worker of %d_job(0-%d) ?\n",i+1,THREADS_NO-1);
			scanf("%d",&worker_no);
			queue[head+i].worker_tid = pin[worker_no][1];
		}
		
		printf("---------------------\nQueue Manager:\nNew jobs: %d\n",jobs_no);
		printf("---------------------\n");

		for(int o = 0; o < jobs_no; o++)
			printf(" job[%d] for thread_id: %d\n",o,queue[o].worker_tid);

		pthread_mutex_unlock(&initial_hash);
		pthread_mutex_unlock(&lock);
		return(-1);
		run -=1;
	}
}

void * onwork(void *arg)
{
		
	int to_write = pthread_self();
	int i = (int)arg;
	int cnt = 0;
	
	printf("  %d\t%d\n",i,pthread_self());

	while(1)
	{
		pthread_mutex_lock(&lock);
		pthread_mutex_lock(&initial_hash);
		
		if(pin[i][1] != to_write)
		{
			pin[i][1] = to_write;
			pin[i][0] = -1;		
			last++;
		}
		cnt = 0;
		
		for(int j = 0; j < 3; j++)
			if(pin[j][0] == -1)
				cnt++;
		if(cnt == 3)
			hash_locked =1;
		
		pthread_mutex_unlock(&initial_hash);
		
		if(head >= last)
		{
			printf("Worker[%d] exits.\n",i);
			pthread_mutex_unlock(&lock);
			break;
		}	
		if( (queue[head].worker_tid == to_write) )
		{
			printf("Job assigned to worker[%d] \n",i);
			head++;			
			sleep(1);
		}
		pthread_mutex_unlock(&lock);
	}
}

int main(int argc,char *argv[],char *envp[])
{
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_mutex_init(&lock,NULL);
	pthread_mutex_init(&initial_hash,NULL);

	pthread_create(&oura,&attr,queue_manager,(void*)NULL);	
	
	printf("WORKER\ttid\n");

	for(int i = 0; i < 3; i++)
		pthread_create(&workers[i],&attr,onwork,(void *) i);

	for(int i = 0; i < 3; i++)
		pthread_join(workers[i],NULL);

	pthread_join(oura,NULL);	
	printf("workers completed\n");	
	pthread_mutex_destroy(&initial_hash);
	pthread_mutex_destroy(&lock);
	
	return 0;
}
