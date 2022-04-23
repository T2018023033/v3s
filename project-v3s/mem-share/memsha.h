#ifndef __MEMSHA_H__
#define __MEMSHA_H__

#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

struct server_info_t {
    int totalnum;
    int currnum;
    int exitnum;
    unsigned char flags;
};

char* memory_share_init(void);
int memory_share_close(char *mapAddr);

#endif // __MEMSHA_H__
