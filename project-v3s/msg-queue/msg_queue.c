#include "msg_queue.h"

#define MSG_LEN 1024

static int msg_send(msgQueue* msg, const char *buf, int pri)
{
    return  mq_send(msg->handle, buf, MSG_LEN, pri);
}

static int msg_recv(msgQueue* msg, char *buf, int pri)
{
    return  mq_receive(msg->handle, buf, MSG_LEN, &pri);
}

static int msg_close(msgQueue* msg)
{
    if ((msg->handle = mq_open(msg->name,O_RDONLY,NULL,NULL))  > 0) {
        printf("msg queue exist, close msg queue !!! \r\n");
        mq_close(msg->handle);
        if(mq_unlink(msg->name)<0){
            printf("mq unlink err !!! \r\n");
            return -1;
        }
    }
    if(msg)
        free(msg);

    return 0;
}

msgQueue* create_msg_queue(char *name)
{
    struct mq_attr mq_param;
    msgQueue *msg = (msgQueue *)malloc(sizeof(msgQueue));

    sprintf(msg->name, "/%s", name);
    msg->handle = mq_open(msg->name, O_RDONLY, NULL, NULL);
    if (msg->handle > 0) {
        printf("msg queue exist !!! \r\n");
        mq_close(msg->handle);
        if(mq_unlink(msg->name)<0){
            printf("mq unlink err !!! \r\n");
            return NULL;
        }
    }

    mq_param.mq_maxmsg = 64;
    mq_param.mq_msgsize = MSG_LEN;
    mq_param.mq_flags = 0;
    mq_param.mq_curmsgs = 0;

    msg->handle = mq_open(msg->name, O_RDWR | O_CREAT, 0777, &mq_param);
    if(msg->handle  < 0){
        printf("open mq failed! \r\n");
        return NULL;
    }
    
    msg->send = msg_send;
    msg->recv = msg_recv;
    msg->close = msg_close;

    return msg;
}
