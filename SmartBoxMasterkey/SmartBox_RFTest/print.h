
#ifndef _GH_PRINT_H
#define _GH_PRINT_H

#ifdef __cplusplus
extern "C"
{
#endif
#define Debug println

extern void sendstr(char*);
extern void sendstrln(char*);
void print(char* fmt,...);
void println(char* fmt,...);
char* printBin(void *buf, int len);

void PrintMemoryMap(int _showaddress, void* _address, int len, int bitRow);
char* float_print(float s);
#ifdef __cplusplus
}
#endif

#endif