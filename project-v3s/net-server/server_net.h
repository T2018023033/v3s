#ifndef __SERVER_NET_H__
#define __SERVER_NET_H__

#include <stdio.h>
#include <string.h>
#include <sys/types.h>         
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netinet/in.h> 
#include <sys/time.h>
#include <time.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>


#define uint32_t  unsigned long long 
#define SERVER_LISTEN_NUM 10
//#define IP "10.21.46.60"
#define IP "192.168.123.129"

struct clientInfo_t {
    int client_fd;
    pthread_t client_pthread_id;
};

typedef struct netServer_t netServer;

struct netServer_t {
    struct clientInfo_t clientInfo[SERVER_LISTEN_NUM];
    pthread_mutex_t net_mutex;
    int client_alive_num;
    int server_fd;

    int (*add_client)(netServer* serverInfo, int clientFd,
                        void*(*client_func)(void* arg), void *args);

    int (*remove_client)(netServer* serverInfo, int clientFd);

    char* (*get_client_ip)(netServer *netserver, int client_fd);

    int (*send)(int fd, char *buf, int len);
    int (*recv)(int fd, char *buf, int len);

    int (*close)(netServer *serverInfo);
};

netServer* create_net_server(uint32_t port);

#endif
