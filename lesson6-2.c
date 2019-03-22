#include <wiringPi.h>
#include <unistd.h>
#include <stdio.h>

#include <time.h>
#include <sys/time.h>

#define SW1		1 //wPin 1
#define SW2		3 //wPin 3
#define LED		2 //wPin 2


void pinAssign(void)
{
	pinMode(SW1, INPUT);
	pinMode(LED, OUTPUT);
	pinMode(SW2, INPUT);
}
void displayTime(struct timeval tv)
{
	printf("Time: %ld.%ld\r", tv.tv_sec, tv.tv_usec);
}

int main()
{
	//time_t start_t, end_t;
	struct timeval start_tv, stop_tv, diff_tv;
	struct timeval cur_tv;
	wiringPiSetup();
	char val1, val2;
	int flag = 1;

	pinAssign();

	while (digitalRead(SW1))						// switch1이 켜지면 start
	{
		if ((gettimeofday(&start_tv, NULL)) == -1)
		{
			perror("gettimeofday() call error");
			return -1;
		}
	}
	while (1)
	{
		usleep(10000);

		if ((gettimeofday(&stop_tv, NULL)) == -1)
		{
			perror("gettimeofday() call error");
			return -1;
		}

		if (start_tv.tv_usec > stop_tv.tv_usec)
		{
			stop_tv.tv_sec--;
			diff_tv.tv_usec = 1000000 + stop_tv.tv_usec - start_tv.tv_usec;
			diff_tv.tv_sec = stop_tv.tv_sec - start_tv.tv_sec;
		}
		else
		{
			diff_tv.tv_usec = stop_tv.tv_usec - start_tv.tv_usec;
			diff_tv.tv_sec = stop_tv.tv_sec - start_tv.tv_sec;
		}

		if (digitalRead(SW2) == 0)
		{
			printf("LapT: %ld.%ld\n", diff_tv.tv_sec, diff_tv.tv_usec);
			while (digitalRead(SW2) == 0) {};
		}
		else
		{
			printf("Time: %ld.%ld\r", diff_tv.tv_sec, diff_tv.tv_usec);
			fflush(stdout);
		}

	}

}
}