#ifndef SAVEDATA_H
#define SAVEDATA_H

#include <iostream>
#include <map>

using namespace std;

class SaveData
{
public:
    typedef struct s_SysData
    {
        int m_cpuRate;  //0-100
        int m_memoryUse;    //
        int m_memoryTotal;  //
    }s_SysData;
    typedef struct s_CliData
    {
        int fd;  //文件描述符
        char IP[16];  //登录IP地址
        int port;  //登录的端口号
    }s_CliData;
    typedef struct s_Savedata
    {
        s_SysData s_sysData;
        s_CliData s_cliData;
    }s_Savedata;

    static map<int, s_Savedata> serSaveData;

private:

};


#endif // SAVEDATA_H
