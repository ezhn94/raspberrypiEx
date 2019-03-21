#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>

// File To Key
// key_t ftok(const char* pathname, int proj_id)
//
int main(int argc, char **argv)
{
	key_t msgKey;

	msgKey = ftok("/home/pi/raspberryPiEx", 'A');
	printf("ftok_key = %s\n", msgKey);

	return 0;
}
