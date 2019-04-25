#include <wiringPi.h>

#include <unistd.h>

#include <stdio.h>



//시간관련 헤더파일

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



int main()

{

	//time_t start_t, end_t;

	struct timeval start_tv, stop_tv, diff_tv;

	wiringPiSetup();

	char val1, val2;



	pinAssign();



	// SW1이 눌리면 START

	while (digitalRead(SW1));



	if ((gettimeofday(&start_tv, NULL)) == -1)

	{

		perror("gettimeofday() call error");

		return -1;

	}



	while (digitalRead(SW2) == 1)

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

		if (digitalRead(SW1) == 0) //버튼이 도중에 눌렸다면

		{

			printf("Time=%ld.%ld\n", diff_tv.tv_sec, diff_tv.tv_usec);

			while (digitalRead(SW1) == 0); //버튼을 뗄 떼까지 기다림

		}

		else

		{

			printf("Time=%ld.%ld\r", diff_tv.tv_sec, diff_tv.tv_usec);

			fflush(stdout);

		}

		while (digitalRead(SW1) == 0); //버튼을 뗄 떼까지 기다림

	}



	printf("Time=%ld.%ld\n", diff_tv.tv_sec, diff_tv.tv_usec);

}