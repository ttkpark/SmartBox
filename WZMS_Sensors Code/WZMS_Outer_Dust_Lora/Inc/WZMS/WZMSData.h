
#include "WZMSBase.h"

#ifndef _WZMS2Data_h_
#define _WZMS2Data_h_

#ifdef __cplusplus
extern "C"
{
#endif

DATA_SystemConst *Data_SystemConst;
DATA_SystemInfo  *Data_SystemInfo;

char* File_SystemInfo;
char* File_APList;
char* File_SystemConst;

// Setup the Data Repository
void WZMSData_init(); 

int  Store_systemInfo();
int  Read_systemInfo();
int  Read_systemConst();

int MasterkeyInterface();
void Masterkey_Data(char* data);

void  __attribute__((weak)) Port_uartprocessing();
void  __attribute__((weak)) UserInitalize();

void SavedInfo();
int Store_SystemInfo();
int Read_SystemInfo();
int Read_SystemConst();

int Store_systemInfo();
int Read_systemInfo();
int Read_systemConst();

#ifdef __cplusplus
}
#endif

#endif //_WZMS2Data_h_