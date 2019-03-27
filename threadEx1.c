// ������ : 1 ~ 200000�� ��

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
	t_id = pthread_self();		// �ڽ��� id ���� �о�´� (getpid)

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
	pthread_t p_thread[2];		// POSIX ������ id ������ Ÿ��
	int ret;	// return value
	long long  a = 1;
	int status;					// t_function�� ���ϰ��� ����
	struct timeval bgn, end;
	double diff;

	printf("PID=%d\n", getpid());

	gettimeofday(&bgn, NULL);

	if ((ret = pthread_create(&p_thread[0], NULL, t_function, (void*)&a)) < 0)	// ������ 1 ����
	{
		perror("ERROR: pthread_create()");
		return -1;
	}
	pthread_join(p_thread[0], (void**)&status);

	
	printf("result: %lli\n", sum2);


	gettimeofday(&end, NULL);
	diff = end.tv_sec + end.tv_usec / 1000000.0 - bgn.tv_sec - bgn.tv_usec / 1000000.0;

	printf("time: %f\n", diff);
}

