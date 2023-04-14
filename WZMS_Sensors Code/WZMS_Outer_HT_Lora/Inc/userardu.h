
#ifndef _userardu_h_
#define _userardu_h_

#include <stdio.h>
#include "main.h"

typedef struct{
	void* GPIOPort;
	uint16_t GPIOPin;
}GPIO_Pin;


typedef struct{
	IRQn_Type IRQType;
	GPIO_Pin Pin;
	void (*Func)();
	int state;
	int type;
}IT_Pin;
enum{
	UNUSED_PIN = 0
};
enum { 
   LOW     = 0, 
   HIGH    = 1, 
   CHANGE  = 2, 
   FALLING = 3, 
   RISING  = 4, 
 };
 enum { 
   INPUT           = 0x0, 
   OUTPUT          = 0x1, 
   INPUT_PULLUP    = 0x2, 
   INPUT_PULLDOWN  = 0x3, 
 };
 
enum
{
	RFO,
	PA_BOOST,
	PA_BOOST_20dBm
};

enum{
	LMIC_UNUSED_PIN = 0,
	NUM_DIO = 3
};
 
typedef struct{
	
	GPIO_Pin nss;		// byte 0: pin for select
	GPIO_Pin rxtx;		// byte 1: pin for rx/tx control
	GPIO_Pin rst;		// byte 2: pin for reset
	GPIO_Pin dio[NUM_DIO];	// bytes 3..5: pins for DIO0, DOI1, DIO2
	// true if we must set rxtx for rx_active, false for tx_active
	uint8_t rxtx_rx_active;	// byte 6: polarity of rxtx active
	int8_t rssi_cal;	// byte 7: cal in dB -- added to RSSI
				//   measured prior to decision.
				//   Must include noise guardband!
	uint32_t spi_freq;	// bytes 8..11: SPI freq in Hz.
}HalPinmap_t;

HalPinmap_t byte2Pinmap(uint8_t* pins);

void vprint(const char *fmt, va_list argp);
void xprintf(const char *fmt, ...); // custom printf() function
void xprintln(const char *fmt, ...); // custom println() function
void debug_init ();
void debug_led (int val);

uint8_t Pin2byte(char port,uint8_t pin);
GPIO_Pin byte2GPIO_Pin(uint8_t pins);

void Pinpullstate(GPIO_TypeDef *gpio,uint16_t Pin,int state);

void pinMode(GPIO_Pin pinNumber, int mode);
void digitalWrite(GPIO_Pin pinNumber, int status);
int digitalRead(GPIO_Pin pinNumber);

void interrupts();
void noInterrupts();

volatile uint32_t Millis;
volatile uint32_t Micros;

void Systick_Init(void);
uint32_t micros(void);
uint32_t millis(void);
void delay(uint32_t nTime);
void delayMicroseconds(uint32_t nTime);

void attachInterrupt(IT_Pin it, void (*func)(),int state);
void detachInterrupt(IT_Pin it);
IT_Pin digitalPinToInterrupt(GPIO_Pin pin);

#endif //_userardu_h_