#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "queuedata.h"


using namespace std;

class MyThread
{
public:
    static void thread_subReactorRun();
    static void thread_dealDataRun();
    static void thread_monitorRun();

private:
    static pthread_t ntid_subReactor;
    static pthread_t ntid_dealData;
    static pthread_t ntid_minotor;

    static void *pthread_subReactor(void *arg);
    static void *pthread_dealData(void *arg);
    static void *pthread_monitor(void *arg);

    static pthread_mutex_t counter_mutex_map;
};


#endif // MYTHREAD_H
