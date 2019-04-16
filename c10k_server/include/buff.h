#ifndef BUFF_H
#define BUFF_H

#include "savedata.h"

void returnData(char *p, int fd);
void data2Buff(char *data, int len);
void deCode(const char *str, SaveData::s_SysData *sysData_t);


#endif // BUFF_H
