#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "queuedata.h"
#include "savedata.h"


using namespace std;

class MyThread
{
public:
    MyThread();
    ~MyThread();

    static void setMonitorFd(int value);
    static int getMonitorFd();

    static void setCliToMoniFd(int value);
    static int getCliToMoniFd();


    static void thread_subReactorRun();
    static void thread_dealDataRun();
    static void thread_monitorRun();

    static void *pthread_subReactor(void *arg);
    static void *pthread_dealData(void *arg);
    static void *pthread_monitor(void *arg);

private:

    static char m_toMonitorData[100];//发送给监视器的数据
    static int m_monitorFd;//监视器fd
    static int m_cliToMoniFd;//监视器监视的客户端fd
    static char m_commandData[20];

    static int w_cpuScale;//
    static int w_memScale;//

    static pthread_t ntid_subReactor;
    static pthread_t ntid_dealData;
    static pthread_t ntid_minotor;

    static pthread_mutex_t counter_mutex_map;
    static pthread_mutex_t counter_mutex_que;

    static void dataEvent_Command();
    static void dataEvent_Data(int str_fd, SaveData::s_SysData &sysData_t);
    static void dataEvent_Enter(int str_fd);
};


#endif // MYTHREAD_H
