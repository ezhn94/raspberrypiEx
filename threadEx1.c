// 쓰레드 : 1 ~ 200000의 합

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/time.h>
// POSIX thread
#include <pthread.h>
#define DATASIZE	sizeof(int)*4

long long sum2;

// Thread function
void* t_function(void* data)
{
	long long  i;
	long long sum = 0;
	long long argData;
	pthread_t t_id;
	long long cnt = 0;
	struct timeval bgn, end;
	double diff;

	argData = *((int*)data);
	t_id = pthread_self();		// 자신의 id 값을 읽어온다 (getpid)

	//printf("getpid:%d, tid=%lu, argData: %d\n", getpid(), t_id, argData);

	for (i = argData; i < argData + 200000; i++)
	{
		sum += i;
		cnt++;
	}
	printf("%lli ~ %lli: %lli\n", argData, argData + 199999, sum);
	sum2 += sum;
	return (void*)sum;
}

int main(void)
{
	pthread_t p_thread[2];		// POSIX 쓰레드 id 데이터 타입
	int ret;	// return value
	long long  a = 1;
	int status;					// t_function의 리턴값을 받음
	struct timeval bgn, end;
	double diff;

	printf("PID=%d\n", getpid());

	gettimeofday(&bgn, NULL);

	if ((ret = pthread_create(&p_thread[0], NULL, t_function, (void*)&a)) < 0)	// 쓰레드 1 생성
	{
		perror("ERROR: pthread_create()");
		return -1;
	}
	pthread_join(p_thread[0], (void**)&status);

	
	//printf("thread_join1: %d\n", status);
	printf("result: %lli\n", sum2);


	gettimeofday(&end, NULL);
	diff = end.tv_sec + end.tv_usec / 1000000.0 - bgn.tv_sec - bgn.tv_usec / 1000000.0;

	printf("time: %f\n", diff);

/*
	long long test = 0;
	long long test1 = 0;
	long long test2 = 0;
	long long test3 = 0;
	
	for (int i = 1; i <= 100000; i++)
	{
		test1 += i;
	}
	for (int i = 100001; i <= 200000; i++)
	{
		test2 += i;
	}
	for (int i = 1; i <= 200000; i++)
	{
		test3 += i;
	}
	test = test1 + test2;
	printf("1~100000: %llu\n", test1);
	printf("100001~200000: %llu\n", test2);
	printf("1~200000: %llu\n", test);
	printf("1~200000: %llu\n", test3);*/

}

