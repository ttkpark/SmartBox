
#ifndef _GH_PRINT_H
#define _GH_PRINT_H

#ifdef __cplusplus
extern "C"
{
#endif

extern void sendstr(char*);
extern void sendstrln(char*);
void print(char* fmt,...);
void println(char* fmt,...);
char* printBin(void *buf, int len);
#ifdef __cplusplus
}
#endif

#endif