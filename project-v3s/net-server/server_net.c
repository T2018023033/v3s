#include "server_net.h"
#include <pthread.h>

static int keep_alive(int socket_fd)
{   
    int val;
    int optResult = 0;

    val = 1;
    optResult |= setsockopt(socket_fd,SOL_SOCKET,SO_KEEPALIVE,(char *)&val,sizeof(int));

    val = 3;
    optResult |= setsockopt(socket_fd,IPPROTO_TCP,TCP_KEEPCNT,(char *)&val,sizeof(int));

    val = 3;
    optResult |= setsockopt(socket_fd,IPPROTO_TCP,TCP_KEEPINTVL,(char *)&val,sizeof(int));

    val = 3;
    optResult |= setsockopt(socket_fd,IPPROTO_TCP,TCP_KEEPIDLE,(char *)&val,sizeof(int));

    return optResult;
}

static int close_server(netServer* serverInfo)
{
    close(serverInfo->server_fd);

    if(serverInfo)
        free(serverInfo);

    return 0;
}

static int add_client(netServer* serverInfo, int clientFd,
                        void*(*client_func)(void* arg), void *args)
{
    if(serverInfo->client_alive_num >= SERVER_LISTEN_NUM)   
        return -1;

    pthread_mutex_lock( &(serverInfo->net_mutex) );

    serverInfo->client_alive_num++;

    serverInfo->clientInfo[serverInfo->client_alive_num].client_fd = clientFd;
    pthread_create(&(serverInfo->clientInfo[serverInfo->client_alive_num].client_pthread_id),
                   NULL, client_func, args);

    pthread_mutex_unlock( &(serverInfo->net_mutex) );
    
    return (serverInfo->client_alive_num);
}

static int remove_client(netServer* serverInfo, int clientFd)
{
    int i=0, id=0;
    pthread_t exit_client_thread;

    pthread_mutex_lock( &(serverInfo->net_mutex) );

    do {
        if(serverInfo->clientInfo[id].client_fd == clientFd)
            break;
        ++id;
    }while(id<SERVER_LISTEN_NUM);

    serverInfo->clientInfo[id].client_fd = 0;
    exit_client_thread = serverInfo->clientInfo[id].client_pthread_id;
    serverInfo->clientInfo[id].client_pthread_id = 0;

    for(i=id; i<SERVER_LISTEN_NUM; i++){
        serverInfo->clientInfo[i].client_fd = serverInfo->clientInfo[i+1].client_fd;
        serverInfo->clientInfo[i].client_pthread_id = serverInfo->clientInfo[i+1].client_pthread_id;
    }

    serverInfo->client_alive_num--;
    pthread_mutex_unlock( &(serverInfo->net_mutex) );

    close(clientFd);
    pthread_exit((void*)-1);
    //pthread_join(exit_client_thread, NULL); //等待线程退出

    return 0; 
}

static int send_net(int fd, char *buf, int len)
{
    return send(fd, buf, len, 0);
}

static int recv_net(int fd, char *buf, int len)
{
    return recv(fd, buf, len, 0);
}

static char* get_client_ip(netServer *netserver, int client_fd)
{
    char buf[32]={0};
    char *buf_ip=buf;
    int len = 0, ret=0;
    struct sockaddr_in addr;

    len = sizeof(addr);
    ret = getpeername(client_fd,  (struct sockaddr *)&addr, &len);
    if(ret < 0){
        printf("get client ip err!\r\n");
        return NULL;
    }else {
        // printf("client login. ip: %s, port :%d \r\n",inet_ntoa(addr.sin_addr),ntohs(addr.sin_port));
        sprintf(buf_ip, "%s", inet_ntoa(addr.sin_addr));
    }

    return buf_ip;
}

netServer* create_net_server(uint32_t port)
{
    int fd=0, time=0;
    netServer *serverInfo = (netServer *)malloc(sizeof(netServer));

    struct sockaddr_in server_addr;
	server_addr.sin_family     = AF_INET;	
	server_addr.sin_port 	   = htons(port); 
	server_addr.sin_addr.s_addr= inet_addr(IP);// htonl(INADDR_ANY);
	//server_addr.sin_addr.s_addr= inet_addr(htonl(INADDR_ANY));

    fd = socket(AF_INET, SOCK_STREAM, 0);

    if (time) {
        struct timeval timeout;
        timeout.tv_sec  =  time;
        timeout.tv_usec =  0;
        if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0){
            goto error;
        }
    }
	int onFlag = 1;
	if (setsockopt(fd, SOL_SOCKET,SO_REUSEADDR,(char *)&onFlag, sizeof(onFlag)) < 0){
        goto error;
	}
    if(bind(fd, (struct sockaddr *)&server_addr, sizeof(server_addr))<0){
        goto error;
    }
    if(keep_alive(fd)<0){
        goto error;
    }
    if(listen(fd, SERVER_LISTEN_NUM) < 0){
        goto error;
    }

    // serverInfo->net_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_init(&(serverInfo->net_mutex), NULL);
    serverInfo->server_fd = fd;
    serverInfo->client_alive_num = 0;

    serverInfo->add_client      = add_client;
    serverInfo->remove_client   = remove_client;
    serverInfo->recv            = recv_net;
    serverInfo->send            = send_net;
    serverInfo->close           = close_server;
    serverInfo->get_client_ip   = get_client_ip;

    return serverInfo;

error:
    close(fd);
    return NULL;
}
