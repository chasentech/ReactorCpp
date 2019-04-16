#include "mythread.h"
#include "myepollser.h"
#include "savedata.h"
#include "buff.h"

pthread_t MyThread::ntid_subReactor;
pthread_t MyThread::ntid_dealData;
pthread_t MyThread::ntid_minotor;

pthread_mutex_t MyThread::counter_mutex_map = PTHREAD_MUTEX_INITIALIZER;


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

void *MyThread::pthread_dealData(void *arg)
{
    cout << (char *)arg << " " << endl;

    while (1)
    {
        int deal_falg = false;
        string str;
        int str_fd = 0;

        //使用线程池时，加锁，竞争获取资源
        //pthread_mutex_lock(&counter_mutex);
        if (!QueueData::queStrIsEmpty() && !QueueData::queEvFdIsEmpty())
        {
            //cout << "que_data: " << que_data.front() << endl;


            str = QueueData::queStrPop();
            str_fd = QueueData::queEvFdPop();

            deal_falg = true;
        }
        else usleep(10);
        //pthread_mutex_unlock(&counter_mutex);



        if (deal_falg == true)
        {
            SaveData::s_SysData sysData_t;

            const char *p = str.data();
            deCode(p, &sysData_t);


//            error!
            //cout << (int)str[0] << " R: " << sysData_t.m_cpuRate << " U: " << sysData_t.m_memoryUse
                 //<< " T: " << sysData_t.m_memoryTotal << endl;

            //全局表写入
            pthread_mutex_lock(&counter_mutex_map);
            SaveData::serSaveData[str_fd].s_sysData = sysData_t;
            pthread_mutex_unlock(&counter_mutex_map);
        }
    }
}

void *MyThread::pthread_monitor(void *arg)
{
    cout << (char *)arg << " " << endl;

    SaveData::s_Savedata saveData_t;
    while (1)
    {
//        if (monitor_fd != 0)
//        {
//            getCpuRate(&sysDate);
//            getMemory(&sysDate);

//            memset(buf_to_qt, 0, 40);
//            int k = 0;

//            k += sysdate_to_buf(buf_to_qt, &sysDate);
//            k += sysdate_to_buf(buf_to_qt + k, &serSaveData[6].sysData);

//            pthread_mutex_lock(&counter_mutex_map);
//            buf_to_qt[k++] = w_cpuScale / 100 + '0';
//            buf_to_qt[k++] = w_cpuScale / 10 % 10 + '0';
//            buf_to_qt[k++] = w_cpuScale % 10 + '0';
//            buf_to_qt[k++] = 'E';

//            buf_to_qt[k++] = w_memScale / 100 % 10 + '0';
//            buf_to_qt[k++] = w_memScale / 10 % 10 + '0';
//            buf_to_qt[k++] = w_memScale % 10 + '0';
//            buf_to_qt[k++] = 'D';
//            pthread_mutex_unlock(&counter_mutex_map);



//            //cout << "......." << buf_to_qt << endl;


//            write(monitor_fd, buf_to_qt, strlen(buf_to_qt));

//            printf("%d, %d, %d\n", sysDate.m_cpuRate, sysDate.m_memoryUse, sysDate.m_memoryTotal);
//            //sleep(1); //获取CPU使用率需要sleep(1)
//        }
//        else sleep(1);

        //全局表读取
        pthread_mutex_lock(&counter_mutex_map);
        int i = 0;
        //w_cpuScale = 0;
        //w_memScale = 0;
        for(map<int, SaveData::s_Savedata>::iterator iter = SaveData::serSaveData.begin();
            iter != SaveData::serSaveData.end(); iter++, i++)
        {
           int key = iter->first;
           SaveData::s_Savedata value = iter->second;

           //if (value.sysData.m_cpuRate > 70) w_cpuScale++;
           //if (value.sysData.m_memoryUse > 6000) w_memScale++;

           cout.setf(ios::left);
           cout << setw(6) << key << " " << setw(15) << value.s_cliData.IP << " " << setw(5) << value.s_cliData.port << " "
                << setw(3) << value.s_sysData.m_cpuRate << " " << setw(4) << value.s_sysData.m_memoryUse << " "
                << setw(4) << value.s_sysData.m_memoryTotal << endl;
        }
//        if (i != 0)
//        {
//            w_cpuScale = w_cpuScale*100 / i;
//            w_memScale = w_memScale*100 / i;
//        }
        pthread_mutex_unlock(&counter_mutex_map);
        sleep(1);

    }
}
