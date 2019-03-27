// 입력 받아 계산


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

// POSIX thread
#include <pthread.h>

long long sum2;

struct add
{
	long long start;
	long long end;
};

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
	printf("%d %d\n", (*argData).start);
	t_id = pthread_self();		// 자신의 id 값을 읽어온다 (getpid)

	//printf("getpid:%d, tid=%lu, argData: %d\n", getpid(), t_id, argData);
	
	for (i = argData; i < argData + 200000; i++)
	{
		sum += i;
		cnt++;
	}
	printf("%lli ~ %lli: %lli\n", argData, argData + 99999, sum);
	sum2 += sum;
	return (void*)sum;
}

int main(void)
{
	pthread_t p_thread[2];		// POSIX 쓰레드 id 데이터 타입
	int ret;	// return value
	struct add a;	
	int status;					// t_function의 리턴값을 받음
	struct timeval bgn, end;
	double diff;

	printf("PID=%d\n", getpid());

	gettimeofday(&bgn, NULL);
	scanf("input start number:%lli", &a.start);
	scanf("input end   number:%lli", &a.end);

	if ((ret = pthread_create(&p_thread[0], NULL, t_function, (void*)&a)) < 0)	// 쓰레드 1 생성
	{
		perror("ERROR: pthread_create()");
		return -1;
	}
	pthread_join(p_thread[0], (void**)&status);
		
	printf("result: %lli\n", sum2);
	
	gettimeofday(&end, NULL);
	diff = end.tv_sec + end.tv_usec / 1000000.0 - bgn.tv_sec - bgn.tv_usec / 1000000.0;

	printf("%f\n", diff);

}

