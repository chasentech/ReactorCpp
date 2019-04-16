#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/errno.h>

#include "wrap.h"
#include "sysdata.h"
#include "buff.h"

using namespace std;

//m_buff使用数组实现
#define MAXLINE 100


class Connection
{
public:
    Connection(const char *serIP, const int port);
    ~Connection();

    void connectServer(int cliNum);  //模拟cliNum个客户端连接
    void connectLoop(); //循环发送消息

private:
    struct sockaddr_in m_servaddr;  //服务器地址结构体

    vector<int> m_sockfdArray;  //保存cli的fd
    char m_buff[MAXLINE];   //buff

    int m_cliNum;   //客户端最大个数连接
};




#endif
