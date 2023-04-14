//-----------------giyong-giho's header in C---------------
#include "giuart.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include "main.h"
#include "userardu.h"

char To36jin_out(char buf)
{
	char output=0;
	if('0'<= buf && '9' >= buf){output = buf-'0';}
	else if('a' <= buf && buf <= 'z'){output = buf -'a'+10;}
	else if('A' <= buf && buf <= 'Z'){output = buf -'A'+10;}
	else if(buf == 0){output = 0 ;}
	else output=buf;
	return output;
}
void delay_ms(uint32_t delay)
{
	HAL_Delay(delay);
}
//millis() : Returns the Millisecond starting from begining.
/*
void delay_us(uint32_t us){
	us*= SystemCoreClock/9000000;us-=2;
	while(us--);
}*/
//char Debugarray[1024];
void print(char* fmt,...){
	va_list ap;
	va_start(ap, fmt);
	vprint(fmt, ap);
	va_end(ap);
}
void println(char* fmt,...){
 	va_list ap;
	va_start(ap, fmt);
 	vprint(fmt, ap);
	va_end(ap);
 	vprint("\r\n",ap);
}