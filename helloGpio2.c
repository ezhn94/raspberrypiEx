#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define loopCount 10
#define delayMax 1024

// ./helloGpio [Pinnumver]
int main(int argc, char **argv)
{
	int gpioNo;
	int i;
	int delayTime;

	// STEP 1 : wiringPi init
	wiringPiSetup();
	// Pin number error
	if(argc < 2)
	{
		printf("Usage : %s gpioNo\n", argv[0]);				
		return -1;
	}
	
	gpioNo = atoi(argv[1]);

	// STEP 2: Pin direction setup
	pinMode(gpioNo, OUTPUT);
	
	for(i = 0; i<loopCount; i++)
	{
		for(delayTime = 0; delayTime<delayMax;delayTime++)
		{
		// STEP 3: Pin write
			digitalWrite(gpioNo, HIGH);
			usleep(delayTime);
			digitalWrite(gpioNo, LOW);
			usleep(delayMax-delayTime);
		}
		for(delayTime = 0; delayTime<delayMax; delayTime++)
		{
			digitalWrite(gpioNo, HIGH);
			usleep(delayMax-delayTime);
			digitalWrite(gpioNo, LOW);
			usleep(delayTime);
		}
	}

	return 0;
}



