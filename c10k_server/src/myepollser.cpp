#include "myepollser.h"
#include "wrap.h"
#include "buff.h"


//生成用于处理epoll专用的文件描述符, 参数256可随意，现在该参数的作用被遗弃。
int MyEpollSer::m_epfd = epoll_create(10000 + 1);		//m_cliNum + 1
//创建套接字
int MyEpollSer::m_listenfd = Socket(AF_INET, SOCK_STREAM, 0);

vector<char> MyEpollSer::m_buff(100);
pthread_mutex_t MyEpollSer::counter_mutex_map = PTHREAD_MUTEX_INITIALIZER;


MyEpollSer::MyEpollSer(const int servPort, const int cliNum)
{
    m_cliNum = cliNum;
    m_events.reserve(m_cliNum);

    //buff init
    m_buff.reserve(100);
    for (int i = 0; i < 100; i++)
        m_buff[i] = 0;


    //准备服务器地址
    struct sockaddr_in serveraddr;
    serveraddr.sin_family       = AF_INET;
    serveraddr.sin_addr.s_addr  = htonl(INADDR_ANY);
    serveraddr.sin_port         = htons(servPort);

    //设置socket重用
    int opt = 1;
    setsockopt(m_listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    //绑定
    Bind(m_listenfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    //监听
    Listen(m_listenfd, m_cliNum);

    m_ev.data.fd = m_listenfd;        //设置epoll管理的socket，
    m_ev.events = EPOLLIN | EPOLLET;  //设置事件类型为可读事件，工作方式为边沿触发
    epoll_ctl(m_epfd, EPOLL_CTL_ADD, m_listenfd, &m_ev);    //注册epoll事件

}

MyEpollSer::~MyEpollSer()
{
    close(m_epfd);
    close(m_listenfd);
}


//把客户端的socket设置为非阻塞方式
void MyEpollSer::setnonblocking(int sock)
{
    int opts;
    opts = fcntl(sock, F_GETFL);
    if(opts < 0)
    {
        perror("fcntl(sock, GETFL)");
        exit(1);
    }

    opts = opts | O_NONBLOCK;
    if(fcntl(sock, F_SETFL, opts) < 0)
    {
        perror("fcntl(sock,SETFL,opts)");
        exit(1);
    }
}

//改变connfd读写属性
void MyEpollSer::update_events(int epfd, int connfd, int events, int op)
{
    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.events = events;
    ev.data.fd = connfd;

    int r = epoll_ctl(epfd, op, connfd, &ev);
    if (r != 0)
        perror("epoll_ctl failed");
}

int MyEpollSer::epollNfds()
{
    int nfds = epoll_wait(m_epfd, m_events.data(), m_cliNum, 10); //100ms等待epoll事件的发生
    return nfds;
}

struct epoll_event MyEpollSer::getEventsIndex(int index)
{
    return m_events[index];
}

int MyEpollSer::getListenFd()
{
    return m_listenfd;
}
int MyEpollSer::getEpfd()
{
    return m_epfd;
}

void MyEpollSer::handleAccept(struct epoll_event *evs)
{
    if(evs->data.fd == m_listenfd) //fd为监听的fd
    {
        char return_data[10] = {0};

        struct sockaddr_in clientaddr;
        socklen_t clientaddr_len = sizeof(clientaddr);

        int connfd = Accept(m_listenfd, (struct sockaddr*)&clientaddr, &clientaddr_len);
        if(connfd < 0)
        {
            perror("connfd < 0");
            exit(1);
        }
        setnonblocking(connfd);    //把客户端的socket设置为非阻塞方式

        const char *cli_ip = inet_ntoa(clientaddr.sin_addr);
        int cli_port = ntohs(clientaddr.sin_port);
        printf("fd = %d, connect from IP: %s, port: %d\n", connfd, cli_ip, cli_port);


        //全局表写入
        pthread_mutex_lock(&counter_mutex_map);
        SaveData::s_CliData cliData_t;
        cliData_t.fd = connfd;
        strcpy(cliData_t.IP, cli_ip);
        cliData_t.port = cli_port;

        SaveData::serSaveData[connfd].s_cliData = cliData_t;
        //cout << "CliData: " << cliData_t.fd << " " << cliData_t.IP
             //<< " " << cliData_t.port << endl;
        pthread_mutex_unlock(&counter_mutex_map);




        //设置事件类型为可读事件，边沿触发，并注册epoll事件
        update_events(m_epfd, connfd, EPOLLIN | EPOLLET, EPOLL_CTL_ADD);

        //retrun to client(necessary!!!)
        returnData(return_data, connfd);
        write(connfd, return_data, 10);
    }
}

void MyEpollSer::handleRead(struct epoll_event *evs)
{
    if (evs->data.fd != m_listenfd && evs->events & (EPOLLIN | EPOLLERR | EPOLLRDHUP))
    {
        int sockfd = evs->data.fd;
        if (sockfd < 0)
            return;

        if (evs->events & EPOLLRDHUP)
        {
            //if (sockfd == monitor_fd) monitor_fd = 0;
            close(sockfd);
            printf("client is EPOLLRDHUP!\n");
        }

        int n = 0;
        memset(m_buff.data(), 0, 100);
        if ( (n = read(sockfd, m_buff.data(), 100)) < 0)	//连接错误
        {
            //if (sockfd == monitor_fd) monitor_fd = 0;
            close(sockfd);
            printf("readline error\n");
        }
        else if (n == 0)	//断开连接
        {
            //if (sockfd == monitor_fd) monitor_fd = 0;
            close(sockfd);
            printf("client close the socket!\n");
        }
        else
        {
//            //Monitor cli
//            if (buff[0] == 'M')
//            {
//                buff[0] = 0;
//                printf("Monitor is connect!\n");
//                write(sockfd, "M", 1);
//                monitor_fd = sockfd;
//            }
//            else    //normal cli
//            {
                printf("recv data: %d %s, fd = %d\n", m_buff[0], &m_buff[1], sockfd);
                //接收到的信息入队
                QueueData::queStrPush(m_buff.data());
                QueueData::queEvFdPush(sockfd);

            //}





            //设置事件类型为可写事件，边沿触发，并注册epoll事件
            //update_events(epfd, sockfd, EPOLLOUT | EPOLLET, EPOLL_CTL_MOD);
        }

    }
}


