#include "queuedata.h"

queue<struct epoll_event> QueueData::queEvs;
queue<string> QueueData::queStr;
queue<int> QueueData::queEvFd;

void QueueData::queEvsPush(struct epoll_event events)
{
    queEvs.push(events);
}
void QueueData::queStrPush(string str)
{
    queStr.push(str);
}
void QueueData::queEvFdPush(int fd)
{
    queEvFd.push(fd);
}

bool QueueData::queEvsIsEmpty()
{
    return queEvs.empty();
}
bool QueueData::queStrIsEmpty()
{
    return queStr.empty();
}
bool QueueData::queEvFdIsEmpty()
{
    return queEvFd.empty();
}

struct epoll_event QueueData::queEvsPop()
{
    struct epoll_event x = queEvs.front();
    queEvs.pop();
    return x;
}
string QueueData::queStrPop()
{
    string x = queStr.front();
    queStr.pop();
    return x;
}
int QueueData::queEvFdPop()
{
    int x = queEvFd.front();
    queEvFd.pop();
    return x;
}

