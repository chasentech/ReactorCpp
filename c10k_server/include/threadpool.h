#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <time.h>


//线程池状态
typedef struct condition
{
    pthread_mutex_t pmutex;	//互斥锁
    pthread_cond_t pcond;	//条件变量
}condition_t;


//封装线程池中的对象需要执行的任务对象
typedef struct task
{
    void *(*run)(void *args);  //函数指针，需要执行的任务
    void *arg;              //参数
    struct task *next;      //任务队列中下一个任务
}task_t;


//线程池结构体
typedef struct threadpool
{
    condition_t ready;   //状态量
    task_t *first;       //任务队列中第一个任务
    task_t *last;        //任务队列中最后一个任务
    int counter;         //线程池中已有线程数
    int idle;            //线程池中空闲线程数
    int max_threads;     //线程池最大线程数
    int quit;            //是否退出标志
}threadpool_t;


class ThreadPool
{
public:
    ThreadPool(int threadNum, void *(*run)(void *arg), void *arg);
    ~ThreadPool();

    //线程池初始化
    static void threadpool_init(threadpool_t *pool, int threads);
    //增加一个任务到线程池
    static void threadpool_add_task(threadpool_t *pool, void *(*run)(void *arg), void *arg);
    //线程池销毁
    static void threadpool_destroy();

private:
    //int pool_num = 5;
    static threadpool_t m_pool;

    static int condition_init(condition_t *cond);//初始化
    static int condition_lock(condition_t *cond);//加锁
    static int condition_unlock(condition_t *cond);//解锁
    //pthread_cond_wait和pthread_cond_timedwait用来等待条件变量被设置，值得注意的是这两个等待调用需要一个已经上锁的互斥体mutex
    static int condition_wait(condition_t *cond);//等待
    //固定时间等待
    static int condition_timedwait(condition_t *cond, const struct timespec *abstime);
    //pthread_cond_signal则用于解除某一个等待线程的阻塞状态
    static int condition_signal(condition_t* cond);//唤醒一个睡眠线程
    //pthread_cond_broadcast用于设置条件变量，即 使得事件发生，这样等待该事件的线程将不再阻塞
    static int condition_broadcast(condition_t *cond);//唤醒所有睡眠线程
    static int condition_destroy(condition_t *cond);//释放


    static void *thread_routine(void *arg);
};

#endif // THREADPOOL_H
