#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct my_msg_st{
	long int my_msg_type;
	char some_text[BUFSIZ];
};
int main()
{
	int running =1;
	int msgid;
	struct my_msg_st some_data;
	long int msg_to_receive = 0;
	struct msqid_ds data;

	msgid = msgget((key_t)1234, 0666 | IPC_CREAT);

	if(msgid == -1){
		fprintf(stderr, "msgget failed with error: %d\n" ,errno);
		exit(EXIT_FAILURE);
	}
	
	msgctl(msgid, IPC_STAT, &data);

	printf("남은 메시지 개수는 :%d\n", data.msg_qnum);

	while(data.msg_qnum-->0){
		if(msgrcv(msgid, (void*)&some_data, sizeof(some_data), msg_to_receive,0) == -1){
			fprintf(stderr, "msgrcv failed with error: %d\n", errno);
			exit(EXIT_FAILURE);
		}
		printf("You wrote: %s", some_data.some_text);
		if(strncmp(some_data.some_text, "end", 3) ==0 && data.msg_qnum == 0){
				printf("종료\n");
				running = 0;
		}
	}
	if(msgctl(msgid, IPC_RMID, 0) == -1){
		fprintf(stderr, "msgctl(IPC_RMID) failed\n");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}	
				
				
				

