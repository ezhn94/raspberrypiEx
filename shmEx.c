#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>

// For IPC

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

typedef struct myData SCORE;

#define SHMSIZE 100 // int * 100


void sigHandler(int sig)
{
}

struct myData
{
	char name[20];
	int kor;
	int eng;
	int mat;
};

int main(void)
{
	void *shared_Mem = (void*)0;
	int shmid;
	//int *shmaddr;
	SCORE* score;

	//SCORE* score = (SCORE*)shared_mem

	int i;
	SCORE person;

	int rdData;
	pid_t pid;
	int sum = 0;
	int flag = 2;

	signal(SIGUSR1, sigHandler);

	shmid = shmget((key_t)1234, sizeof(int)*SHMSIZE, 0666 | IPC_CREAT);
	if (shmid == -1)
	{
		fprintf(stderr, "shmget failed\n");
		exit(EXIT_FAILURE);
	}
	score = (SCORE*)shmat(shmid, NULL, 0);
	SCORE* score = (SCORE*)shared_Mem;

	pid = fork();	

	if (pid == -1)
	{
		fprintf(stderr, "fork error\n");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)	// child proc
	{
		pause();
		printf("CHILD PROC\n");
		// 5. shmctl
		printf("flag %d\n", flag);
		while (flag!=0)
		{
			sum = score->kor + score->mat + score->eng;
			printf("Name: %s\n", score->name);
			printf("SUM: %d\n", sum);
			printf("AVG: %f\n", (float)sum / 3);
			flag--;
		}
		if (shmctl(shmid, IPC_RMID, 0) == -1)
		{
			fprintf(stderr, "shmctl (IPC_RMID) failed\n");
			exit(EXIT_FAILURE);
		}
		exit(EXIT_SUCCESS);
	}
	else				// parents proc
	{
		// 1. shmget
		shmid = shmget((key_t)1234, sizeof(int)*SHMSIZE, 0666 | IPC_CREAT);
		if (shmid == -1)
		{
			fprintf(stderr, "shmget failed\n");
			exit(EXIT_FAILURE);
		}

		// 2. shmat
		shared_Mem = shmat(shmid, (void*)0, 0);
		if (shared_Mem == (void*)-1)
		{
			fprintf(stderr, "shmat failed\n");
			exit(EXIT_FAILURE);
		}
				
		//shmaddr = (int*)shared_Mem;

		// 3. memory access		
		while (1)
		{
			printf("INPUT NAME:");
			fgets(person.name, sizeof(person.name), stdin);

			if (!strncmp(person.name, "end", 3))
			{
				printf("END\n");
				kill(pid, SIGUSR1);
				break;
			}
			else
			{
				printf("KOR: ");
				scanf("%d", &person.kor);
				printf("ENG: ");
				scanf("%d", &person.eng);
				printf("MAT: ");
				scanf("%d", &person.mat);
				getchar();
			}
			memcpy(&score, &shared_Mem, sizeof(SCORE));			
		}
		// 4. shmdt
		if (shmdt(shared_Mem) == -1)
		{
			fprintf(stderr, "shmdt failed\n");
			exit(EXIT_FAILURE);
		}
		wait(NULL);
	}
}