#include <iostream>
#include <queue>
#include <vector>
#include <map>
#include <iomanip>

#include "wrap.h"
#include "myepollser.h"
#include "queuedata.h"
#include "mythread.h"
#include "threadpool.h"

using namespace std;


pthread_mutex_t counter_mutex_main = PTHREAD_MUTEX_INITIALIZER;

int main()
{
    cout << "start main reactor!" << endl;
    MyEpollSer epollSer(8000, 10000);   //主线程，监听活动的fd
    MyThread::thread_subReactorRun();   //线程一，处理IO
    //MyThread::thread_dealDataRun();     //deal data
    MyThread::thread_monitorRun();      //线程二，与监视器通讯

    //线程池，处理数据
    ThreadPool threadpool(5, MyThread::pthread_dealData, (void *)"start dealDate");

    while (1)
    {
        int nfds = epollSer.epollNfds();

        for (int i = 0; i < nfds; i++)
        {
            //入队需要加锁
            pthread_mutex_lock(&counter_mutex_main);
            QueueData::queEvsPush(epollSer.getEventsIndex(i));
            pthread_mutex_unlock(&counter_mutex_main);

        }

        usleep(100);
    }

    pthread_mutex_destroy(&counter_mutex_main);
    return 0;
}
