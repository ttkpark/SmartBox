#include <stdarg.h>
#include <stdint.h>
#include "print.h"

#ifdef __cplusplus
extern "C"
{
#endif

extern void sendstr(char*);
extern void sendstrln(char*);
char Debugarray[1024];
void print(char* fmt,...){
 va_list ap;
  va_start(ap, fmt);
  vsprintf(Debugarray, fmt, ap);
  sendstr(Debugarray);
}
void println(char* fmt,...){
  va_list ap;
  va_start(ap, fmt);
  vsprintf(Debugarray, fmt, ap);
  sendstrln(Debugarray);
}
char sbufx[64];
char* printBin(void *buf, int len)
{
	for(int i=0;i<len;i++)sprintf(sbufx+i*3," %02X",((uint8_t*)buf)[i]);
	return sbufx;
}
#ifdef __cplusplus
}
#endif