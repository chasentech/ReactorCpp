#ifndef QUEUEDATA_H
#define QUEUEDATA_H

#include <sys/socket.h>
#include <sys/epoll.h>

#include <iostream>
#include <queue>

using namespace std;

class QueueData
{
public:
    static queue<struct epoll_event> queEvs;
    static queue<string> queStr;
    static queue<int> queEvFd;

    static void queEvsPush(struct epoll_event events);
    static void queStrPush(string str);
    static void queEvFdPush(int fd);

    static bool queEvsIsEmpty();
    static bool queStrIsEmpty();
    static bool queEvFdIsEmpty();

    static struct epoll_event queEvsPop();
    static string queStrPop();
    static int queEvFdPop();

};


#endif // QUEUEDATA_H
