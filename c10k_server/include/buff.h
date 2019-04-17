#ifndef BUFF_H
#define BUFF_H

#include "savedata.h"

void returnData(char *p, int len, int fd);
void data2Buff(char *data, int len);
void cliData2Buff(char *data, int len, SaveData::s_SysData &saveData);
void wholeRate2Buff(char *data, int len, int rateCpu, int rateMen);


void deCode_Command(const char *str, char *dst, int len);
void deCode_Data(const char *str, SaveData::s_SysData &sysData_t);
void deCode_Enter();
char checkData(const char *str);


#endif // BUFF_H
