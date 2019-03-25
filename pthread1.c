#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// POSIX thread
#include <pthread.h>
#define DATASIZE	sizeof(int)*4

int glob_var = 11;
int *addr;

// Thread function
void* t_function(void* data)
{
	int i;
	int argData;
	pthread_t t_id;

	argData = *((int*)data);
	t_id = pthread_self();		// 자신의 id 값을 읽어온다 (getpid)
	sleep(argData);
	printf("getpid:%d, tid=%lu, argData: %d, glob_var:%d\n", getpid(), t_id, argData,glob_var);
	printf("heap memory access ");
	for (i = 0; i < 4; i++)
	{
		printf("%d ", *(addr+i));
	}
	printf("\n");
	return (void*)(argData*argData);
}

int main(void)
{	
	pthread_t p_thread[2];		// POSIX 쓰레드 id 데이터 타입
	int ret;	// return value
	int a = 1;
	int b = 2;
	int status;					// t_function의 리턴값을 받음
	int i;

	addr = malloc(DATASIZE);

	for (i = 0; i < 4; i++)
	{
		*(addr+i) = i;
	}

	printf("PID=%d\n", getpid());

	if ((ret = pthread_create(&p_thread[0],NULL, t_function, (void*)&a)) < 0)	// 쓰레드 생성
	{
		perror("ERROR: pthread_create() 1");
		return -1;
	}
	if ((ret = pthread_create(&p_thread[1], NULL, t_function, (void*)&b)) < 0)	// 쓰레드 생성
	{
		perror("ERROR: pthread_create() 2");
		return -2;
	}
	pthread_join(p_thread[0], (void**)&status);
	printf("thread_join1: %d\n", status);

	pthread_join(p_thread[1], (void**)&status);
	printf("thread_join2: %d\n", status);
}