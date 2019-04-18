#include<wiringPiI2C.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#define  DevAddr  0x53  //device address

struct acc_dat {
	int x;
	int y;
	int z;
};

void adxl345_init(int fd)
{
	// initialize command seq
	wiringPiI2CWriteReg8(fd, 0x31, 0x0b);
	wiringPiI2CWriteReg8(fd, 0x2d, 0x08);

	usleep(11100);		// 11.1ms wait
	//	wiringPiI2CWriteReg8(fd, 0x2e, 0x00);

	// TODO: take 100 data point sample -> avg cod
	// 샘플링 대신 0으로 초기값을 설정
	wiringPiI2CWriteReg8(fd, 0x1e, 0x00);
	wiringPiI2CWriteReg8(fd, 0x1f, 0x00);
	wiringPiI2CWriteReg8(fd, 0x20, 0x00);

	//	DUR register
	//	Disable the tap/double tap functions
	wiringPiI2CWriteReg8(fd, 0x21, 0x00);	

	//	Latent register
	//	Disable the double tap functions
	wiringPiI2CWriteReg8(fd, 0x22, 0x00);	

	//	Window register
	//	Disable the double tap functions
	wiringPiI2CWriteReg8(fd, 0x23, 0x00);	

	//	THRESH_ACT register:	
	wiringPiI2CWriteReg8(fd, 0x24, 0x01);	
	wiringPiI2CWriteReg8(fd, 0x25, 0x0f);
	wiringPiI2CWriteReg8(fd, 0x26, 0x2b);
	wiringPiI2CWriteReg8(fd, 0x27, 0x00);

	wiringPiI2CWriteReg8(fd, 0x28, 0x09);
	wiringPiI2CWriteReg8(fd, 0x29, 0xff);
	wiringPiI2CWriteReg8(fd, 0x2a, 0x80);
	wiringPiI2CWriteReg8(fd, 0x2c, 0x0a);
	wiringPiI2CWriteReg8(fd, 0x2f, 0x00);
	wiringPiI2CWriteReg8(fd, 0x38, 0x9f);
}

struct acc_dat adxl345_read_xyz(int fd)
{
	char x0, y0, z0, x1, y1, z1;
	struct acc_dat acc_xyz;

	x0 = 0xff - wiringPiI2CReadReg8(fd, 0x32);
	x1 = 0xff - wiringPiI2CReadReg8(fd, 0x33);
	y0 = 0xff - wiringPiI2CReadReg8(fd, 0x34);
	y1 = 0xff - wiringPiI2CReadReg8(fd, 0x35);
	z0 = 0xff - wiringPiI2CReadReg8(fd, 0x36);
	z1 = 0xff - wiringPiI2CReadReg8(fd, 0x37);

	acc_xyz.x = (int)(x1 << 8) + (int)x0;
	acc_xyz.y = (int)(y1 << 8) + (int)y0;
	acc_xyz.z = (int)(z1 << 8) + (int)z0;

	return acc_xyz;
}

int main(void)
{
	int fd;
	struct acc_dat acc_xyz;

	fd = wiringPiI2CSetup(DevAddr);

	if (-1 == fd) {
		perror("I2C device setup error");
	}

	adxl345_init(fd);

	while (1) {
		acc_xyz = adxl345_read_xyz(fd);
		printf("x: %05d  y: %05d  z: %05d\r", acc_xyz.x, acc_xyz.y, acc_xyz.z);
		fflush(stdout);
		delay(500);
	}

	return 0;
}