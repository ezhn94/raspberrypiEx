#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

// For IPC
//

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHMSIZE 100 // int * 100

int main(void)
{
	void *shared_Mem=(void*)0;
	// void형 포인터: 주소만 중요함, 초기값으로 NULL값을 명시(아직 할당되지 않음을 나타냄)
	int shmid;
	int *shmaddr;
	int i;
	int rdData;

	// 1. shmget
	shmid =	shmget((key_t)1234, sizeof(int)*SHMSIZE, 0666 | IPC_CREAT);	
	if(shmid == -1)
	{
		fprintf(stderr, "shmget failed\n");
		exit(EXIT_FAILURE);
	}
	
	// 2. shmat
	shared_Mem = shmat(shmid, (void*)0, 0); // 다른 프로세서와 공유할 수 있는 영역
	if(shared_Mem == (void*)-1)
	{
		fprintf(stderr, "shmat failed\n");
		exit(EXIT_FAILURE);
	}
	
	printf("Memory attached at %X\n", (int)shared_Mem);
	shmaddr=(int*)shared_Mem;

	// 3. memory access
	for(i=0;i<SHMSIZE;i++)
	{
		rdData = *(shmaddr + i);
		printf("shmaddr: %X, data:%d\n", shmaddr+i, rdData);
	}
	
	// 4. shmdt
	if(shmdt(shared_Mem) == -1)
	{
		fprintf(stderr, "shmdt failed\n");
		exit(EXIT_FAILURE);
	}
	
	
	// 5. shmctl
	if(shmctl(shmid, IPC_RMID, 0) == -1)
	{
		fprintf(stderr, "shmctl (IPC_RMID) failed\n");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}

