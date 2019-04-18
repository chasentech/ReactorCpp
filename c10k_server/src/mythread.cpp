#include "mythread.h"
#include "myepollser.h"
#include "buff.h"

pthread_t MyThread::ntid_subReactor;
pthread_t MyThread::ntid_dealData;
pthread_t MyThread::ntid_minotor;

pthread_mutex_t MyThread::counter_mutex_map = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t MyThread::counter_mutex_que = PTHREAD_MUTEX_INITIALIZER;


char MyThread::m_toMonitorData[100];
int MyThread::m_monitorFd = -1;
int MyThread::m_cliToMoniFd = -1;
char MyThread::m_commandData[20];

int MyThread::w_cpuScale = 0;   //
int MyThread::w_memScale = 0;   //


MyThread::MyThread()
{
    memset(m_toMonitorData, 0, 100);
    memset(m_commandData, 0, 20);
}
MyThread::~MyThread()
{
    pthread_mutex_destroy(&counter_mutex_map);
    pthread_mutex_destroy(&counter_mutex_que);
}

void MyThread::setMonitorFd(int value)
{
    m_monitorFd = value;
}
int MyThread::getMonitorFd()
{
    return m_monitorFd;
}
void MyThread::setCliToMoniFd(int value)
{
    m_cliToMoniFd = value;
}
int MyThread::getCliToMoniFd()
{
    return m_cliToMoniFd;
}

void MyThread::thread_subReactorRun()
{
    pthread_create(&ntid_subReactor, NULL, pthread_subReactor, (void *)"start subreactor!");
}
void MyThread::thread_dealDataRun()
{
    pthread_create(&ntid_dealData, NULL, pthread_dealData, (void *)"start dealDate!");
}
void MyThread::thread_monitorRun()
{
    pthread_create(&ntid_minotor, NULL, pthread_monitor, (void *)"start monitor!");
}

void *MyThread::pthread_subReactor(void *arg)
{
    cout << (char *)arg << " " << endl;

    while (1)
    {
        if (!QueueData::queEvsIsEmpty())
        {
            struct epoll_event events = QueueData::queEvsPop();
            MyEpollSer::handleAccept(&events);
            MyEpollSer::handleRead(&events);

        }
        else usleep(10); //
    }
}


void MyThread::dataEvent_Command()
{
    //cout << "cliclicli:" << m_commandData << endl;
    int temp = 0;
    int type = 0;
    if (m_commandData[0] >= '0' && m_commandData[0] <= '9')
        type = 1;
    else type = 2;

    if (type == 1)//获取某个客户端数据
    {
        for (int i = 0; m_commandData[i] != '|'; i++) //'|' is end
        {
            temp = temp * 10 + m_commandData[i] - '0';
        }

        pthread_mutex_lock(&counter_mutex_map);

        map<int, SaveData::s_Savedata>::iterator iterMin = SaveData::serSaveData.begin();

        int minFd = iterMin->first;


        //设置需要获取数据的客户端fd
        map<int, SaveData::s_Savedata>::iterator iter =
                SaveData::serSaveData.find(temp + minFd);
        if (iter != SaveData::serSaveData.end())
        {
            m_cliToMoniFd = temp + minFd;
            //printf("m_cliToMoniFd = %d\n", m_cliToMoniFd);
        }
        pthread_mutex_unlock(&counter_mutex_map);
        temp = 0;

    }
    if (type == 2)//执行命令，将'|'改为'\0'即可
    {
        for (int i = 0; ; i++) //'|' is end
        {
            if (m_commandData[i] == '|')
            {
                m_commandData[i] = '\0';
                break;
            }
        }
        //执行相关命令
        //......
    }
}
void MyThread::dataEvent_Data(int str_fd, SaveData::s_SysData &sysData_t)
{
    //全局表写入
    pthread_mutex_lock(&counter_mutex_map);
    SaveData::serSaveData[str_fd].s_sysData = sysData_t;
    pthread_mutex_unlock(&counter_mutex_map);
}
void MyThread::dataEvent_Enter(int str_fd)
{
    char temp[10] = {0};
    temp[0] = 5;
    temp[1] = 'E';
    temp[2] = 'Y';
    temp[3] = '|';
    temp[4] = '$';
    write(str_fd, temp, strlen(temp));

    m_monitorFd = str_fd;
    SaveData::serSaveData.erase(str_fd);//monitorfdb不需要加入全局表
}

void *MyThread::pthread_dealData(void *arg)
{
    cout << (char *)arg << " " << endl;

    while (1)
    {
        int deal_falg = false;
        string str;
        int str_fd = 0;

        //使用线程池时，加锁，竞争获取资源
        pthread_mutex_lock(&counter_mutex_que);
        if (!QueueData::queStrIsEmpty() && !QueueData::queEvFdIsEmpty())
        {
            //cout << "que_data: " << que_data.front() << endl;


            str = QueueData::queStrPop();
            str_fd = QueueData::queEvFdPop();

            deal_falg = true;
        }
        else usleep(10);
        pthread_mutex_unlock(&counter_mutex_que);

        if (deal_falg == true)
        {
            //SaveData::s_SysData sysData_t;

            switch (checkData(str.data()))
            {
                case 'C':deCode_Command(str.data()+2, (char *)&m_commandData, 20);
                        dataEvent_Command();
                        break;
                case 'D':SaveData::s_SysData sysData_t;
                        deCode_Data(str.data()+2, sysData_t);//解析数据
                        dataEvent_Data(str_fd, sysData_t);  //执行操作
                        break;
                case 'E': //int value = 0;
                        deCode_Enter();
                        dataEvent_Enter(str_fd);
                        break;
                default: perror("checkData error!"); break;
            }
        }
    }
}

void *MyThread::pthread_monitor(void *arg)
{
    cout << (char *)arg << " " << endl;

    //SaveData::s_Savedata saveData_t;
    while (1)
    {
        if (m_monitorFd != -1)
        {
            memset(m_toMonitorData, 0, 100);

            data2Buff(m_toMonitorData, 100);

            pthread_mutex_lock(&counter_mutex_map);

            m_toMonitorData[0]--;   //notice to delete '$'
            if (m_cliToMoniFd != -1)
            {
                cliData2Buff(m_toMonitorData,
                    100, SaveData::serSaveData[m_cliToMoniFd].s_sysData);   //modify 8
            }
            else
            {
                SaveData::s_SysData s_sysData;
                s_sysData.m_cpuRate = 0;
                s_sysData.m_memoryUse = 0;
                s_sysData.m_memoryTotal = 0;
                cliData2Buff(m_toMonitorData, 100, s_sysData);
            }

            m_toMonitorData[0]--;
            wholeRate2Buff(m_toMonitorData, 100, w_cpuScale, w_memScale);


            int fdMax = SaveData::serSaveData.size();
            m_toMonitorData[0]--;
            fdMinMax(m_toMonitorData, 100, fdMax);


            pthread_mutex_unlock(&counter_mutex_map);

            //32 D8|4476|7872|38|4003|8000|0|0|$
            write(m_monitorFd, m_toMonitorData, strlen(m_toMonitorData));
        }
        else sleep(1);

        //全局表读取
        pthread_mutex_lock(&counter_mutex_map);
        int i = 0;
        w_cpuScale = 0;
        w_memScale = 0;
        for(map<int, SaveData::s_Savedata>::iterator iter = SaveData::serSaveData.begin();
            iter != SaveData::serSaveData.end(); iter++, i++)
        {
           //int key = iter->first;
           SaveData::s_Savedata value = iter->second;

           if (value.s_sysData.m_cpuRate > 70) w_cpuScale++;
           if (value.s_sysData.m_memoryUse > 6000) w_memScale++;

           /*
           cout.setf(ios::left);
           cout << setw(6) << key << " " << setw(15) << value.s_cliData.IP << " " << setw(5) << value.s_cliData.port << " "
                << setw(3) << value.s_sysData.m_cpuRate << " " << setw(4) << value.s_sysData.m_memoryUse << " "
                << setw(4) << value.s_sysData.m_memoryTotal << endl;
           */
        }
        if (i != 0)
        {
            w_cpuScale = w_cpuScale*100 / i;
            w_memScale = w_memScale*100 / i;
        }

        cout.setf(ios::left);
        cout << setw(8) << "cliNum: " << setw(6) << SaveData::serSaveData.size()
             << setw(8) << ", moniFd: " << setw(4) << m_monitorFd
             << ", moniListenFd: " << setw(4) << m_cliToMoniFd << endl;

        pthread_mutex_unlock(&counter_mutex_map);

    }
}
