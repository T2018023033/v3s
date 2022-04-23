#ifndef __UART_H__
#define __UART_H__

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <strings.h>
#include <stdint.h>

struct ble_mesh_send_t {
	uint8_t fh[8];
	uint16_t maddr;
	uint8_t data[10];
	uint8_t ft[2];
};

struct ble_mesh_recv_t {
	uint8_t fh[3];
	uint16_t send_maddr;
	uint16_t recv_maddr;
	uint8_t data[10];
};

int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop);
int init_uart(char *device);


#endif 