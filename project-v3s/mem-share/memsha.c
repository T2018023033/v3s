#include "memsha.h"

#define MEM_MAX_SIZE 4096

char* memory_share_init(void)
{
    int shmId=0;	
	key_t shmKey;
	char *name = "/home/tzy/linux/v3s/code/qt/c-connect/c-shareMem/shm";
	char *mapAddr = NULL;

	shmKey = ftok(name, 0); /* get standant key */	
	shmId = shmget(shmKey, MEM_MAX_SIZE, 0644 | IPC_CREAT);
	if(shmId == -1){
		printf(" shmget error \r\n");
		return NULL;
	}

	mapAddr = (char *)shmat(shmId, NULL, 0);/* map memory */

    return mapAddr;
}

int memory_share_close(char *mapAddr)
{
   return shmdt(mapAddr); 
}

