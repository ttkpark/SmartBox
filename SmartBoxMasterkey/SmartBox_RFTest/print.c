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


void PrintMemoryMap(int _showaddress, void* _address, int len, int bitRow) {
	Debug("PrintMemoryMap() start");
	char *_Buf1 = (char*)malloc(bitRow + 1);
	memset(_Buf1, 0, bitRow + 1);
	char* addr = (char*)_address;

	char* newline = "\r\n";
	print("Address : 0x%08X%s", _showaddress, newline);
	int remainbyte = len % bitRow;
	int focus = 0;

	for (; focus < (len - remainbyte); focus++) {
		if ((focus % bitRow) == 0) {
			print("0x%08X: ", _showaddress + focus);
		}
		print("%02X ", (int)addr[focus]);
		if (32 <= (uint8_t)addr[focus] && (uint8_t)addr[focus] <= 127)
			_Buf1[focus%bitRow] = addr[focus];
		else
			_Buf1[focus%bitRow] = '.';

		if ((focus % bitRow) == (bitRow - 1)) {
			println(_Buf1);
			memset(_Buf1, 0, bitRow);
		}
	}

	if(remainbyte)
		for (int i = 0; i < bitRow; i++) {
			if ((focus % bitRow) == 0) {
				print("0x%08X: ", _showaddress + focus);
			}
			if (i > (remainbyte - 1)) {
				print("   ");
			}
			else {
				print("%02X ", addr[focus]);
				if ('0' <= (uint8_t)addr[focus] && (uint8_t)addr[focus] <= 127)
					_Buf1[focus%bitRow] = addr[focus];
				else
					_Buf1[focus%bitRow] = '.';
			}
			if (i == bitRow - 1) {
				println(_Buf1);
				memset(_Buf1, 0, bitRow);
			}
			focus++;
		}
	free(_Buf1);
	println("");
	Debug("PrintMemoryMap() end");
}

char sbufx[64];
char* float_print(float s)
{
	int dec = (int)s;
	int ddd = (s-dec)*1000;
	sprintf(sbufx,"%d.%03d",dec,ddd);
	return sbufx;
}
 
#ifdef __cplusplus
}
#endif