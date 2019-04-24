/*
服务器epoll类
*/
#ifndef MYEPOLLSER_H
#define MYEPOLLSER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

#include <iostream>
#include <vector>
#include <iomanip> //cout << setw(15)

#include "queuedata.h"
#include "mythread.h"

using namespace std;

class MyEpollSer
{
public:
    MyEpollSer(const int servPort, const int cliNum);
    ~MyEpollSer();

    int epollNfds();    //返回活动的events个数
    struct epoll_event getEventsIndex(int index);
    int getListenFd();
    int getEpfd();

    static void handleAccept(struct epoll_event *evs);  //处理连接
    static void handleRead(struct epoll_event *evs);    //读数据

private:
    static int m_epfd;
    static int m_listenfd;
    int m_cliNum;
    struct epoll_event m_ev;
    vector<struct epoll_event> m_events;
    static vector<char> m_buff;
    static pthread_mutex_t counter_mutex_map;

    static void setnonblocking(int sock);
    static void update_events(int epfd, int connfd, int events, int op);

};


#endif // MYEPOLLSER_H
