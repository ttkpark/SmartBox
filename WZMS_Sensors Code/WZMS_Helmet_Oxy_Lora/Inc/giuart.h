//-----------------giyong-giho's uart --------------------
#include <stdint.h>

//#define SystemCoreClock 32000000UL
//#define USE_xprintf 

/*
struct typ_64{
	unsigned long upper;
	unsigned long lower;
};
*/

void delay_us(uint32_t i);
void delay_ms(uint32_t i);
char To36jin_out(char buf);
int ADCin(char adc_channel);

extern unsigned long millis();

char Debugarray[1024];
void print(char* fmt,...);
void println(char* fmt,...);