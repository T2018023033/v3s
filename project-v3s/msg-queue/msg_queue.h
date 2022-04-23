#ifndef __MSG_QUEUE_H__
#define __MSG_QUEUE_H__

#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>

typedef struct msgQueue_t msgQueue;

struct msgQueue_t {
    mqd_t handle;
    char name[32];
    int (*send)(msgQueue *msg, const char *buf, int pri);
    int (*recv)(msgQueue *msg, char *buf, int pri);
    int (*close)(msgQueue *msg);
};

msgQueue* create_msg_queue(char *name);

#endif