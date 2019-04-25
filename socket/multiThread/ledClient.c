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


	if (argc != 3) {			// IP�� ��Ʈ��ȣ�� �ϳ��� �Է����� ���� ���
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);			// ���� ����, PF_INET: IPv4, �������� ü�踦 ǥ���� �� ���, SOCK_STREAM: TCP���
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;					// AF_INET: IPv4, �ּ�ü�踦 ǥ���� �� ���
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));		// htons: short intger �����͸� ��Ʈ��ũ byte order�� ����, argv[1]�� ��Ʈ��ȣ�� �޴´�.

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
		switch (data.cmd)			// userThread���� data.cmd�� �޾Ƽ� switch�� ����
		{			
		case WR_DIST: printf("data.hc04_dist=%f\n", data.hc04_dist);	// HC04�� �Ÿ������͸� �����Ͽ� ���� ����Ѵ�.
			break;
		case WR_IMG:													// �̹��� ������ �о� �����Ѵ�.
			fp = fopen("result.jpg", "w+b");
			while ((read_cnt = read(sock, filebuf, BUF_SIZE)) != 0)		// EOF ���� �� ����
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
		// LED�� �����̱� ���� �ڵ�
		if (data.led_Value == 1)
			data.led_Value = 0;
		else
			data.led_Value = 1;

		// ������ LED�� ������ ���� ��û ������ ������
		data.cmd = WR_LED;			// cmd�� WR_LED�� �����Ͽ� LED���� �����ϵ��� ��
		printf("data.led_Value=%d\n", data.led_Value);
		write(sock, &data, sizeof(data));

		// ������ HC04 �Ÿ� ��û ������ ������
		data.cmd = RD_HC04;			// cmd�� RD_HC04�� ����
		write(sock, &data, sizeof(data));

		data.cmd = RD_IMG;			// cmd�� RD_IMG�� ����
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