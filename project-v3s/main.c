#include "thread-pool/thread_pool.h"
#include "net-server/server_net.h"
#include "msg-queue/msg_queue.h"
#include "cJSON/cJSON.h"
#include "cmd-deal/cmd_deal.h"
#include "uart-mesh/uart.h"
#include "sqlitedbms-api/sqlitedbms_client.h"
#include "mem-share/memsha.h"

ThreadPool* pool = NULL;
msgQueue* msg = NULL;
netServer* server = NULL;
SQL_HANDLE sqlhandle;
struct server_info_t netInfo;


void* client_task_func(void *arg)
{
    int ret=0;
    int *fd = (int *)arg;
    int client_fd = fd[0];
    char buf[256]={0}, cmd[128]={0};
    char *send_buf=NULL;
    int len=0;
    char *name=NULL;
 
    while(1)
    {
        /* socket客户端正常关闭的时候服务端会收到一个长度是0的数据包 */
        /* 阻塞接收, 判断接收是否为0，是：说明client断开 */
        ret = server->recv(client_fd, buf, 256);
        if( strcmp(buf, "quit")==0 || ret <= 0 ){
            break;
        }else{
            printf("ret = %d , recv buf:%s\n", ret, buf);
            switch(parse_json_data(buf)){
            case ADD:
                printf("added! \r\n");
                break;
            case SHOWLINE: 
                send_buf = packaging_showline_info_json(&len);
                server->send(client_fd, send_buf, len);
                memset(send_buf,0,sizeof(send_buf));
                break;
            case REFRESH:
                send_buf = packaging_addpatient_info_json(&len);
                server->send(client_fd, send_buf, len);
                memset(send_buf,0,sizeof(send_buf));
                break;
            case SHOWROOM:
                send_buf = packaging_room_info_json(&len);
                server->send(client_fd, send_buf, len);
                memset(send_buf,0,sizeof(send_buf));
                break;
            case SHOWPATIENT:
                name = parse_json_data_name(buf);
                send_buf = packaging_patient_info_json(&len,name);
                server->send(client_fd, send_buf, len);
                free(name);
                memset(send_buf,0,sizeof(send_buf));
                break;
            case DELPATIENT:
                name = parse_json_data_name(buf);
                send_buf = packaging_patient_info_json(&len,name);
                sprintf(cmd, "delete from manageHumanTb where name='%s';",name);
                sqlTbDel(sqlhandle, cmd);
                free(name);
                memset(cmd,0,sizeof(cmd));
                break;
            case UPDATEINFO:
                break;
            default:
                break;
            }

            memset(buf,0,sizeof(buf));
            
            // server->send(client_fd, buf, 32);
            //    pool->add(pool, task_func1, buf, 1024);
            //    pool->add(pool, task_func2, buf, 1024);

        }       
    }

    netInfo.exitnum++;
    netInfo.currnum--;
    server->remove_client(server, client_fd);
    return NULL;
}

void* mesh_uart_task(void *args) /* 开启一个线程处理 串口数据 */
{
    int fd = 0;
    int i=0;
    struct ble_mesh_recv_t mesh_recv;
    struct ble_mesh_send_t mesh_send;

    fd = init_uart("/dev/ttyS1"); /* 串口2 */
    if(fd < 0){
        printf("Failed to open ttyS1");
        return NULL;
    }

    uint8_t send_fh[] ={0x41,0x54,0x2b,0x4d,0x45,0x53,0x48,0x00};
    memcpy(mesh_send.fh, send_fh, sizeof(send_fh));
    mesh_send.maddr = 0x0500;  //目标地址 TODO:大小端问题
    mesh_send.ft[0] = 0x0d;
    mesh_send.ft[1] = 0x0a;

    while(1){
		if(read(fd, &mesh_recv, sizeof(mesh_recv))){
            if(mesh_recv.recv_maddr !=0 || mesh_recv.send_maddr !=0){
                printf("\r\nuart2 mesh recv recv_maddr: 0x%04x \r\n", mesh_recv.recv_maddr);
                printf("uart2 mesh recv send_maddr: 0x%04x \r\n", mesh_recv.send_maddr);
                printf("uart2 mesh recv data: %s \r\n\r\n", mesh_recv.data);

                memset(mesh_recv.data, 0, sizeof(mesh_recv.data));
                mesh_recv.recv_maddr = 0; /* bug： 一次接收完成 需要清除标志等待下一次发送 */
                mesh_recv.send_maddr = 0;

                //memcpy(mesh_send.data, send_data, 10);
                // write(fd, send_buf, 22);
                sprintf(mesh_send.data, "hi:%d",i++);
                write(fd, &mesh_send, 22);
            }
            else{ }
            
		}
        else{
            sleep(1);
        }
    }
}

void* net_info_task(void *args)
{
    char *mapaddr = memory_share_init();
    char buf[4096] = {0}, i;
    char *buffer = (char *)malloc(4096);

    while(1)
    {
        sprintf(buffer, "\n服务器状态:\r\n  当前连接:%d个\r\n  已经退出:%d个\r\n  一共连接:%d个 *%d", 
                    netInfo.currnum, netInfo.exitnum, netInfo.totalnum,netInfo.currnum);
        memcpy(mapaddr, buffer, 4096);
       
        sleep(1);
    }

    memory_share_close(mapaddr);
    free(buffer);
    return NULL;
}

int main(void)
{
    int net_ready = 0, client_fd=0;
    fd_set rset, allset;
    pthread_t mesh_uart, net_info;
    char *verMsg = (char *)malloc(128);
    
#if 1
    memset(verMsg, 0, 128);
    sqlhandle = sqlConn("192.168.123.129", "v3sDb");
    sqlVerGet(sqlhandle, verMsg);
    printf("sqlite3 version: %s\r\n", verMsg);

    //if( pthread_create(&mesh_uart, NULL, mesh_uart_task, NULL) < 0){
       // perror("create thread err!\r\n");
       // return -1;
    //}
    if( pthread_create(&net_info, NULL, net_info_task, NULL) < 0){
        perror("create thread err!\r\n");
        return -1;
    }
#endif 

    pool = PoolWorkQueueCreate(2, 4, 10);
    //msg = create_msg_queue("hi_msg");
    server = create_net_server(1234);
    // if(msg == NULL){
    //     printf("mq init error! \r\n");
    // }else{
    //     printf("mq init success! \r\n");
    // }

    FD_ZERO(&allset);	
    FD_SET(server->server_fd, &allset);
    while(1)
    {
        rset   = allset;
        net_ready = select((server->server_fd)+1, &rset, NULL, NULL, NULL);
        if (net_ready < 0) {
            printf("select failed ! \r\n");
            break;
        }
        if (FD_ISSET(server->server_fd, &rset)) //有设备连接
        {
            client_fd = accept(server->server_fd, NULL, NULL);
            if (client_fd < 0) {
                printf("accept failed !\r\n");
                continue;
            }
            else{
                server->add_client(server, client_fd, client_task_func, &client_fd);
                printf("new connect client ip = %s, client number = %d\r\n",
                    server->get_client_ip(server, client_fd), server->client_alive_num);
                netInfo.currnum++; 
                netInfo.totalnum++;
            }
        }
        
        sleep(1);
    }

    msg->close(msg);
    pool->destroy(pool);
    sqlClose(sqlhandle);

    return 0;
}

