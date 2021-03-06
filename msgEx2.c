#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>

// for IPC function

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
typedef struct myData SCORE;

void sigHandler(int sig)
{
}

struct myData
{
	long int msgType;
	char name[20];
	int kor;
	int eng;
	int mat;
};
int main()
{
	int prunning = 1;
	int running =1;
	SCORE person[10];
	struct msqid_ds msqstat;
	int msgid;
	int personNum =0;
	
	signal(SIGUSR1, sigHandler);

	pid_t pid;

	pid = fork();
	msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
	
	if(msgid == -1)
	{
		fprintf(stderr, "msgget failed\n");
		exit(EXIT_FAILURE);
	}
	if(pid == -1)
	{
		fprintf(stderr, "fork error\n");
		exit(EXIT_FAILURE);
	}
	else if(pid ==0)	// 자식 프로세서(proc2)
	{
		pause();
		while(running)
		{
			if(msgrcv(msgid, &person[personNum], sizeof(person)-sizeof(long), 0, 0) == -1)
			{
				fprintf(stderr, "msgrcv error: %d\n",errno);
				exit(EXIT_FAILURE);
			}
			if(person[personNum].msgType == 2)
				running = 0;
			else
			{
				int sum;
				sum =person[personNum].kor + person[personNum].eng +person[personNum].mat;
				getchar();
				printf("Name: %s", person[personNum].name);				
				printf("SUM: %d\n", sum);
				printf("AVG: %f\n", (float)sum/3);
			}
			if(msgctl(msgid, IPC_STAT, &msqstat) == -1)
			{
				perror("fail msgctl()");
				exit(1);
			}
			if(msqstat.msg_qnum > 0)
				running =1;
			personNum++;
		}	

		if(msgctl(msgid, IPC_RMID, 0) == -1)
		{
			fprintf(stderr, "msgctl error\n");
			exit(EXIT_FAILURE);
		}
		exit(EXIT_SUCCESS);
	}
	else			// 부모 프로세스
	{
		while(prunning)
		{	
			printf("INPUT NAME:");
			fgets(person[personNum].name, sizeof(person[personNum].name), stdin);
			if(!strncmp(person[personNum].name, "end", 3))
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
				person[personNum].msgType =1;
				getchar();				
			}
				
			if(msgsnd(msgid, &person[personNum], sizeof(person)-sizeof(long), 0) == -1)
			{
				fprintf(stderr, "msgsnd error\n");
				exit(EXIT_FAILURE);
			}
			personNum++;
		}
		wait(NULL);
	}
}

