#include "connection.h"

Connection::Connection(const char *serIP, const int port)
{
    bzero(&m_servaddr, sizeof(m_servaddr));
    m_servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, serIP, &m_servaddr.sin_addr);
    m_servaddr.sin_port = htons(port);

    m_cliNum = 0;
    m_sockfdArray.clear();
}

Connection::~Connection()
{
    //close fd
    for (int i = 0; i < m_cliNum; i++)
    {
        if (m_sockfdArray[i] != 0)
            close(m_sockfdArray[i]);
    }
}

void Connection::connectServer(int cliNum)
{
    //init m_sockfdArray
    m_cliNum = cliNum;
    m_sockfdArray.reserve(m_cliNum);
    for (int i = 0; i < m_cliNum; i++)
    {
        m_sockfdArray[i] = 0;
    }


    //模拟m_clinum个客户端
    int sockfd = 0;
    for (int i = 0; i < m_cliNum; i++)
    {
        sockfd = Socket(AF_INET, SOCK_STREAM, 0);
        Connect(sockfd, (struct sockaddr *)&m_servaddr, sizeof(m_servaddr));

        m_sockfdArray[i] =  sockfd;
        printf("i = %d\n", i);

        //连接成功，服务器反馈信息，必要的
        char rece_ser[10] = {0};
        Read(sockfd, rece_ser, 10);
        printf("received = %s\n", rece_ser);

    }
    printf("-----------------------\n");
}

void Connection::connectLoop()
{
    while (1)
    {
        for (int i = 0; i < m_cliNum; i++)
        {
            memset(m_buff, 0, MAXLINE);
            data_to_buff(m_buff, MAXLINE, i);   //**sleep(1)
            Write(m_sockfdArray[i], m_buff, strlen(m_buff));
        }
        printf("sleep!\n");
        //sleep(1);               //*sleep(1)
    }
}
