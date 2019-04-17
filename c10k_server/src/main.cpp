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


//****什么时候清理map****


pthread_mutex_t counter_mutex_main = PTHREAD_MUTEX_INITIALIZER;

int main()
{
    cout << "start main reactor!" << endl;
    MyEpollSer epollSer(8000, 10000);   //main reactor param: port cliNum
    MyThread::thread_subReactorRun();   //sub reactor
    //MyThread::thread_dealDataRun();     //deal data
    MyThread::thread_monitorRun();      //monitor

    ThreadPool threadpool(5, MyThread::pthread_dealData, (void *)"start dealDate");

    while (1)
    {
        int nfds = epollSer.epollNfds();

        for (int i = 0; i < nfds; i++)
        {
            pthread_mutex_lock(&counter_mutex_main);
            QueueData::queEvsPush(epollSer.getEventsIndex(i));
            pthread_mutex_unlock(&counter_mutex_main);

        }

        usleep(100);
    }



    return 0;
}
