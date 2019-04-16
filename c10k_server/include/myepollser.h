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

using namespace std;

class MyEpollSer
{
public:
    MyEpollSer(const int servPort, const int cliNum);
    ~MyEpollSer();

    int epollNfds();
    struct epoll_event getEventsIndex(int index);
    int getListenFd();
    int getEpfd();
    static void handleAccept(struct epoll_event *evs);
    static void handleRead(struct epoll_event *evs);

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
