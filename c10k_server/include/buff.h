/*
对字符串进行的操作
*/

#ifndef BUFF_H
#define BUFF_H

#include "savedata.h"

//反馈客户端的信息
void returnData(char *p, int len, int fd);
//系统信息tobuff
void data2Buff(char *data, int len);
//监听的客户端信息tobuff
void cliData2Buff(char *data, int len, SaveData::s_SysData &saveData);
//集群信息tobuff
void wholeRate2Buff(char *data, int len, int rateCpu, int rateMen);
//集群最大连接数tobuff
void fdMinMax(char *data, int len, int fdMax);


//解析命令数据
void deCode_Command(const char *str, char *dst, int len);
//解析普通数据
void deCode_Data(const char *str, SaveData::s_SysData &sysData_t);
//解析验证数据
void deCode_Enter();
//检查信息是否有误
char checkData(const char *str);


#endif // BUFF_H
