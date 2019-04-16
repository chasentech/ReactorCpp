#include "buff.h"
#include "sysdata.h"

void data_to_buff(char *data, int len, int k)
{
    char buf_temp[10] = {0};
    int n = 0;
    int j = 1;

    if (k == 0)//k=0，使用真实数据
    {
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
    }
    else    //k!=0，使用模拟数据
    {
        n = sprintf(buf_temp, "%d", 35 + (k%50));
        for (int i = 0; i < 10 && i < n; i++)
        {
            data[j++] = buf_temp[i];
        }
        data[j++] = 'R';

        n = sprintf(buf_temp, "%d", (4000 + (k%3000)));
        for (int i = 0; i < 10 && i < n; i++)
        {
            data[j++] = buf_temp[i];
        }
        data[j++] = 'U';

        n = sprintf(buf_temp, "%d", 8000);
        for (int i = 0; i < 10 && i < n; i++)
        {
            data[j++] = buf_temp[i];
        }
        data[j++] = 'T';

        data[0] = j;
    }

    //长度检查
    if (j >= len)
        perror("data buff out of range!\n");

}


