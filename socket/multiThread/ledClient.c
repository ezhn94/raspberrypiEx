#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "raspi.h"

void error_handling(char *message);
void read_routine(int sock, char *buf);
void write_routine(int sock, char *buf);

struct Data data;

int main(int argc, char *argv[])
{
	int sock;
	pid_t pid;
	char buf[BUF_SIZE];
	struct sockaddr_in serv_adr;


	if (argc != 3) {			// IP와 포트번호를 하나라도 입력하지 않은 경우
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);			// 소켓 생성, PF_INET: IPv4, 프로토콜 체계를 표현할 때 사용, SOCK_STREAM: TCP사용
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;					// AF_INET: IPv4, 주소체계를 표현할 때 사용
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));		// htons: short intger 데이터를 네트워크 byte order로 변경, argv[1]로 포트번호를 받는다.

	if (connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("connect() error!");

	pid = fork();
	if (pid == 0)
		write_routine(sock, buf);
	else
		read_routine(sock, buf);

	close(sock);
	return 0;
}

void read_routine(int sock, char *buf)
{
	int read_cnt;
	char filebuf[BUF_SIZE];
	FILE *fp;

	while (1)
	{
		int str_len = read(sock, &data, sizeof(data));
		if (str_len == 0)
			return;
		switch (data.cmd)			// userThread에서 data.cmd를 받아서 switch문 실행
		{			
		case WR_DIST: printf("data.hc04_dist=%f\n", data.hc04_dist);	// HC04의 거리데이터를 수신하여 값을 출력한다.
			break;
		case WR_IMG:													// 이미지 파일을 읽어 저장한다.
			fp = fopen("result.jpg", "w+b");
			while ((read_cnt = read(sock, filebuf, BUF_SIZE)) != 0)		// EOF 수신 시 종료
			{
				fwrite((void*)filebuf, 1, read_cnt, fp);
				if (read_cnt < BUF_SIZE)
					break;
			}
			fclose(fp);
			printf("File Write is done\n");
			break;
		}
	}
}
void write_routine(int sock, char *buf)
{
	data.led_Value = 0;

	while (1)
	{
		// LED를 깜빡이기 위한 코드
		if (data.led_Value == 1)
			data.led_Value = 0;
		else
			data.led_Value = 1;

		// 서버에 LED값 변경을 위한 요청 데이터 보내기
		data.cmd = WR_LED;			// cmd를 WR_LED로 설정하여 LED값을 변경하도록 함
		printf("data.led_Value=%d\n", data.led_Value);
		write(sock, &data, sizeof(data));

		// 서버에 HC04 거리 요청 데이터 보내기
		data.cmd = RD_HC04;			// cmd를 RD_HC04로 설정
		write(sock, &data, sizeof(data));

		data.cmd = RD_IMG;			// cmd를 RD_IMG로 설정
		write(sock, &data, sizeof(data));


		sleep(10);
	}

}
void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}