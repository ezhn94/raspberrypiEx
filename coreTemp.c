// CPU 온도 측정//
// /sys/class/thermal/thermal_zone0/temp	-> zone0의 온도, 23도일때 23000으로 나옴, 1000으로 나눌 것
// 파일이 아니지만 파일에 접근하는 것처럼 한다.
// 온도를 새로 읽기 위해서는 파일을 닫은 후 다시 연다.


#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define buf_size 10

float readCpuTemp(void)
{	
	int fd;
	float temp;		//온도	
	char buf[buf_size];

	// cpu 온도와 cpu 속도 정보를 읽기 전에 문자열 버퍼 초기화
	memset(buf, 0, buf_size);	//buf를 0으로 buf_size 만큼
	fd = open("/sys/class/thermal/thermal_zone0/temp", O_RDONLY);
	if(fd<3)
	{
		printf("Error: Can't open temp file\n");
	}
	read(fd, buf, 5);
	temp = atoi(buf);	
	temp /= 1000;	
	close(fd);		// close 하지 않으면 온도가 업데이트가 불가능함
	
	return temp;	
}
int main(int argc, char **argv)
{	
	float temp;
	
	while(1)
	{	
		temp =	readCpuTemp();
		printf("CPU 온도: %6.3fC\n", temp);
		sleep(1);
	}
}
