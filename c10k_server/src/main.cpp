#include <iostream>
#include <queue>
#include <vector>
#include <map>
#include <iomanip>

#include "wrap.h"
#include "myepollser.h"
#include "queuedata.h"
#include "mythread.h"

using namespace std;


//****什么时候清理map****

int main()
{
    cout << "start main reactor!" << endl;
    MyEpollSer epollSer(8000, 10000);   //main reactor param: port cliNum
    MyThread::thread_subReactorRun();   //sub reactor
    MyThread::thread_dealDataRun();     //deal data
    MyThread::thread_monitorRun();      //monitor

    while (1)
    {
        int nfds = epollSer.epollNfds();

        for (int i = 0; i < nfds; i++)
        {
            QueueData::queEvsPush(epollSer.getEventsIndex(i));
        }

        usleep(100);
    }



    return 0;
}
