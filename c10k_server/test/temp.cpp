#include "buff.h"
#include "sysdata.h"

//解析数据相关函数
//反馈客户端的信息


void data2Buff(char *data, int len)
{
    char buf_temp[10] = {0};
    int n = 0;
    int j = 1;


    SysData::CaptureMemory();
    SysData::CaptureCpuRate();//获取CPU使用率需要sleep(1)

    //14 3 5 R 5 3 5 6 U 8 1 2 3 T
    n = sprintf(buf_temp, "%d", SysData::getCpuRate());
    for (int i = 0; i < 10 && i < n; i++)
    {
        data[j++] = buf_temp[i];
    }
    data[j++] = 'R';

    n = sprintf(buf_temp, "%d", SysData::getMemoryUse());
    for (int i = 0; i < 10 && i < n; i++)
    {
        data[j++] = buf_temp[i];
    }
    data[j++] = 'U';

    n = sprintf(buf_temp, "%d", SysData::getMemoryTotal());
    for (int i = 0; i < 10 && i < n; i++)
    {
        data[j++] = buf_temp[i];
    }
    data[j++] = 'T';

    data[0] = j;



    //长度检查
    if (j >= len)
        perror("data buff out of range!\n");

}

void deCode(const char *str, SaveData::s_sysData *sysData_t)
{
    int count = str[0];
    int temp = 0;

    int i = 1;
    for (i = 1; ; i++)  //退出条件为str[i] == 'T'
    {
        if (str[i] >= '0' && str[i] <= '9')
        {
            temp = temp * 10 + str[i] - '0';

        }
        else if (str[i] == 'R')
        {
            sysData_t->m_cpuRate = temp;
            temp = 0;
        }
        else if (str[i] == 'U')
        {
            sysData_t->m_memoryUse = temp;
            temp = 0;
        }
        else if (str[i] == 'T')
        {
            sysData_t->m_memoryTotal = temp;
            temp = 0;
            break;
        }
    }

    if (i+1 != count) perror("rece data error\n");
}



