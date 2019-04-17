#include "buff.h"
#include "sysdata.h"


void returnData(char *p, int len, int fd)
{
    char buf_temp[10] = {0};
    int j = 2;
    int n = 0;
    n = sprintf(buf_temp, "%d", fd);

    for (int i = 0; i < 10 && i < n; i++)
    {
        p[j++] = buf_temp[i];
    }
    p[j++] = '|';
    p[j++] = '$';

    if (j > len)
    {
        perror("return data error!");
        return;
    }

    p[0] = j;
    p[1] = 'E';

}

void data2Buff(char *data, int len)
{
    char buf_temp[10] = {0};
    int n = 0;
    int j = 2;

    SysData::CaptureMemory();
    SysData::CaptureCpuRate();//获取CPU使用率需要sleep(1)

    //14 3 5 R 5 3 5 6 U 8 1 2 3 T
    n = sprintf(buf_temp, "%d", SysData::getCpuRate());
    for (int i = 0; i < 10 && i < n; i++)
    {
        data[j++] = buf_temp[i];
    }
    data[j++] = '|';

    n = sprintf(buf_temp, "%d", SysData::getMemoryUse());
    for (int i = 0; i < 10 && i < n; i++)
    {
        data[j++] = buf_temp[i];
    }
    data[j++] = '|';

    n = sprintf(buf_temp, "%d", SysData::getMemoryTotal());
    for (int i = 0; i < 10 && i < n; i++)
    {
        data[j++] = buf_temp[i];
    }
    data[j++] = '|';
    data[j++] = '$';

    data[0] = j;
    data[1] = 'D';



    //长度检查
    if (j > len)
        perror("data buff out of range!\n");

}

void cliData2Buff(char *data, int len, SaveData::s_SysData &saveData)
{
    char buf_temp[10] = {0};

    sprintf(buf_temp, "%d", saveData.m_cpuRate);
    int j = data[0];
    for (int i = 0; i < 10 && buf_temp[i] != '\0'; i++)
    {
        data[j++] = buf_temp[i];
    }
    data[j++] = '|';

    sprintf(buf_temp, "%d", saveData.m_memoryUse);
    for (int i = 0; i < 10 && buf_temp[i] != '\0'; i++)
    {
        data[j++] = buf_temp[i];
    }
    data[j++] = '|';

    sprintf(buf_temp, "%d", saveData.m_memoryTotal);
    for (int i = 0; i < 10 && buf_temp[i] != '\0'; i++)
    {
        data[j++] = buf_temp[i];
    }
    data[j++] = '|';
    data[j++] = '$';

    data[0] = j;
    //28 D9|4434|7872|38|4003|8000|$
    //长度检查
    if (j > len)
        perror("data buff out of range!\n");
}

void wholeRate2Buff(char *data, int len, int rateCpu, int rateMen)
{
    char buf_temp[10] = {0};

    sprintf(buf_temp, "%d", rateCpu);
    int j = data[0];
    for (int i = 0; i < 10 && buf_temp[i] != '\0'; i++)
    {
        data[j++] = buf_temp[i];
    }
    data[j++] = '|';

    sprintf(buf_temp, "%d", rateMen);
    for (int i = 0; i < 10 && buf_temp[i] != '\0'; i++)
    {
        data[j++] = buf_temp[i];
    }
    data[j++] = '|';
    data[j++] = '$';

    data[0] = j;
    //28 D9|4434|7872|38|4003|8000|$
    //长度检查
    if (j > len)
        perror("data buff out of range!\n");

}

void deCode_Command(const char *str, char *dst, int len)
{
    //8C2235|$
    //12Cshutdown|$
    memset(dst, 0, len);

    int j = 0;
    for (int i = 0; i < 20 && str[i] != '$'; i++)
    {
        dst[j] = str[i];
    }
}

void deCode_Data(const char *str, SaveData::s_SysData &sysData_t)
{
    //decode
    int value[10] = {0};//save temp value
    int temp = 0;
    int j = 0;

    for (int i = 0; str[i] != '$'; i++)  //退出条件为str[i] == '$'
    {
        if (str[i] >= '0' && str[i] <= '9')
        {
            temp = temp * 10 + str[i] - '0';
        }
        else if (str[i] == '|')
        {
            value[j++] = temp;
            temp = 0;
        }
    }

    //extract data
    if (j == 3)
    {
        sysData_t.m_cpuRate = value[0];
        sysData_t.m_memoryUse = value[1];
        sysData_t.m_memoryTotal = value[2];
    }
    else printf("rece data error2\n");
}

void deCode_Enter()
{
//    //5 EM|$

//    int temp = 0;
//    int result = 0;
//    for (int i = 0; i != '$'; i++)
//    {
//        if (str[i] >= '0' && str[i] <= '9')
//        {
//            temp = temp * 10 + str[i] - '0';
//        }
//        else if (str[i] >= 'A' && str[i] <= 'Z')
//        {
//            //temp = str[i];
//            break;
//        }
//        else if (str[i] == '|')
//        {
//            result = temp;
//            temp = 0;
//        }
//    }
//    return 0;
//    //cout << str << endl;
}




char checkData(const char *str)
{
    //check data is right
    int count = str[0];
    if (str[count - 1] != '$')
    {
        perror("rece data error1\n");
        return -1;
    }

    //return type data
    return str[1];
}



