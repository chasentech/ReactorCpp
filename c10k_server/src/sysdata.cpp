#include "sysdata.h"

int SysData::m_cpuRate = 0;  //0-100
int SysData::m_memoryUse = 0;    //
int SysData::m_memoryTotal = 0;  //


unsigned long SysData::GetCPUTotalTime(CPUInfo* pstCPUInfo)
{
    return pstCPUInfo->user + pstCPUInfo->nice + pstCPUInfo->system +
        pstCPUInfo->idle + pstCPUInfo->iowait + pstCPUInfo->irq + pstCPUInfo->softirq;
}

void SysData::DisplayCPUInfo(CPUInfo* pstCPUInfo)
{
    printf("%s %lu %lu %lu %lu %lu %lu %lu\n", pstCPUInfo->name, pstCPUInfo->user, pstCPUInfo->nice, pstCPUInfo->system,
        pstCPUInfo->idle, pstCPUInfo->iowait, pstCPUInfo->irq, pstCPUInfo->softirq);
}

int SysData::GetCPUInfo(CPUInfo *pstCPUInfo)
{
    FILE * fp = fopen("/proc/stat", "r");

    char buf[1024];
    fgets(buf, sizeof(buf), fp);

    sscanf(buf,"%s %lu %lu %lu %lu %lu %lu %lu",
        pstCPUInfo->name, &pstCPUInfo->user, &pstCPUInfo->nice,&pstCPUInfo->system,
        &pstCPUInfo->idle, &pstCPUInfo->iowait, &pstCPUInfo->irq,&pstCPUInfo->softirq);

    fclose(fp);

    return 0;
}

unsigned long SysData::GetCPUUsageRate()
{
    CPUInfo stCPUInfo1;
    CPUInfo stCPUInfo2;

    memset(&stCPUInfo1, 0, sizeof(stCPUInfo1));
    memset(&stCPUInfo2, 0, sizeof(stCPUInfo2));

    GetCPUInfo(&stCPUInfo1);
    //DisplayCPUInfo(&stCPUInfo1);
    sleep(1);
    GetCPUInfo(&stCPUInfo2);
    //DisplayCPUInfo(&stCPUInfo2);

    unsigned long nTotalTime = GetCPUTotalTime(&stCPUInfo2) - GetCPUTotalTime(&stCPUInfo1);
    unsigned long nIdleTime = stCPUInfo2.idle - stCPUInfo1.idle;
    //printf("total time: %lu\n", nTotalTime);
    //printf("idle time:  %lu\n", nIdleTime);	//等待时间
    if (nTotalTime == 0)
    {
        return 0;
    }

    unsigned long usage = (nTotalTime - nIdleTime) * 100 / nTotalTime;
    //printf("CPU use: %lu\n", usage);

    return usage;

}

void SysData::CaptureCpuRate()
{
    m_cpuRate =  GetCPUUsageRate();
}
void SysData::CaptureMemory()
{
    //获取内存信息
    struct sysinfo info;
    sysinfo(&info);
    m_memoryTotal = (int)(info.totalram/1024/1024);
    m_memoryUse = (int)((info.totalram-info.freeram)/1024/1024);
}

int SysData::getCpuRate()
{
    return m_cpuRate;
}

int SysData::getMemoryUse()
{
    return m_memoryUse;
}

int SysData::getMemoryTotal()
{
    return m_memoryTotal;
}
