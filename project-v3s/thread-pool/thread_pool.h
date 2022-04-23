#ifndef _THREAD_POOL_H
#define _THREAD_POOL_H

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// 任务结构体
typedef struct Task
{
    void (*function)(void* arg, int len);
    void* arg;
    int len;
}Task;

typedef struct ThreadPool ThreadPool;

// 线程池结构体
struct ThreadPool
{
    // 任务队列
    Task* taskQ;
    int queueCapacity;  // 容量
    int queueSize;      // 当前任务个数
    int queueFront;     // 队头 -> 取数据
    int queueRear;      // 队尾 -> 放数据

    pthread_t managerID;    // 管理者线程ID
    pthread_t *threadIDs;   // 工作的线程ID
    int minNum;             // 最小线程数量
    int maxNum;             // 最大线程数量
    int busyNum;            // 忙的线程的个数
    int liveNum;            // 存活的线程的个数
    int exitNum;            // 要销毁的线程个数
    pthread_mutex_t mutexPool;  // 锁整个的线程池
    pthread_mutex_t mutexBusy;  // 锁busyNum变量
    pthread_cond_t notFull;     // 任务队列是不是满了
    pthread_cond_t notEmpty;    // 任务队列是不是空了

    int shutdown;           // 是不是要销毁线程池, 销毁为1, 不销毁为0

    int  (*destroy)(ThreadPool* pool);
    void (*add)(ThreadPool* pool, void(*func)(void*, int), void* arg, int len);
    int  (*busyThread)(ThreadPool* pool);
    int  (*aliveThread)(ThreadPool* pool);
};

ThreadPool* PoolWorkQueueCreate(int min, int max, int queueSize);

#endif  // _THREADPOOL_H

