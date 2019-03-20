#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>

#define loopCount 10
#define delayTime 200	//500ms

// ./helloGpio [Pinnumver]
int main(int argc, char **argv)
{
	int gpioNo;
	int i;

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
		// STEP 3: Pin write
		digitalWrite(gpioNo, HIGH);
		delay(delayTime);
		digitalWrite(gpioNo, LOW);
		delay(delayTime);
	}

	return 0;
}



