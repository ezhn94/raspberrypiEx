#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>

// For IPC
//

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
typedef struct myData myData;

#define SHMSIZE 100 // int * 100

struct myData
{
	long int msgType;
	char name[20];
	int kor;
	int eng;
	int mat;
};

int main(void)
{
	void *shared_Mem = (void*)0;
	// void형 포인터: 주소만 중요함, 초기값으로 NULL값을 명시(아직 할당되지 않음을 나타냄)
	int shmid;
	int *shmaddr;
	int i;
	struct myData person[10];
	int rdData;
	pid_t pid;

	pid = fork();
	msgid = msgget((key_t)1234, 0666 | IPC_CREAT);

	if (msgid == -1)
	{
		fprintf(stderr, "msgget failed\n");
		exit(EXIT_FAILURE);
	}

	if (pid == -1)
	{
		fprintf(stderr, "fork error\n");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)	// 자식 프로세서(proc2)
	{
		// 5. shmctl
		if (shmctl(shmid, IPC_RMID, 0) == -1)
		{
			fprintf(stderr, "shmctl (IPC_RMID) failed\n");
			exit(EXIT_FAILURE);
		}
		exit(EXIT_SUCCESS);
	}
	else
	{
		// 1. shmget
		shmid = shmget((key_t)1234, sizeof(int)*SHMSIZE, 0666 | IPC_CREAT);
		if (shmid == -1)
		{
			fprintf(stderr, "shmget failed\n");
			exit(EXIT_FAILURE);
		}

		// 2. shmat
		shared_Mem = shmat(shmid, (void*)0, 0); // 다른 프로세서와 공유할 수 있는 영역
		if (shared_Mem == (void*)-1)
		{
			fprintf(stderr, "shmat failed\n");
			exit(EXIT_FAILURE);
		}

		printf("Memory attached at %X\n", (int)shared_Mem);
		shmaddr = (int*)shared_Mem;

		// 3. memory access		
		while ()
		{
			printf("이름 입력:");
			fgets(person[personNum].name, sizeof(person[personNum].name), stdin);

			if (!strncmp(person[personNum].name, "end", 3))
			{
				printf("END\n");
				person[personNum].msgType = 2;
				prunning = 0;
				kill(pid, SIGUSR1);
			}
			else
			{
				printf("KOR: ");
				scanf("%d", &person[personNum].kor);
				printf("ENG: ");
				scanf("%d", &person[personNum].eng);
				printf("MAT: ");
				scanf("%d", &person[personNum].mat);
				person[personNum].msgType = 1;
				getchar();
			}
			printf();
		}

		// 4. shmdt
		if (shmdt(shared_Mem) == -1)
		{
			fprintf(stderr, "shmdt failed\n");
			exit(EXIT_FAILURE);
		}



	}