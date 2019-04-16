#ifndef _SYSDATA_H_
#define _SYSDATA_H_

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include <stdlib.h>


class SysData
{
public:
    typedef struct cpu_info
    {
        char name[8];
        unsigned long user;
        unsigned long nice;
        unsigned long system;
        unsigned long idle;	//等待时间
        unsigned long iowait;
        unsigned long irq;
        unsigned long softirq;
    } CPUInfo;


    static void CaptureCpuRate();
    static void CaptureMemory();

    static int getCpuRate();
    static int getMemoryUse();
    static int getMemoryTotal();

private:
    static int m_cpuRate;  //0-100
    static int m_memoryUse;    //
    static int m_memoryTotal;  //

    static unsigned long GetCPUTotalTime(CPUInfo* pstCPUInfo);
    static void DisplayCPUInfo(CPUInfo* pstCPUInfo);
    static int GetCPUInfo(CPUInfo *pstCPUInfo);
    static unsigned long GetCPUUsageRate();
};

#endif
