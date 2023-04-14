#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>

#include "userardu.h"

#include "main.h"

#ifndef _WZMS2Base_h_
#define _WZMS2Base_h_

#ifdef __cplusplus
extern "C"
{
#endif

#define false 0
#define true 1

#define FLASH_ADDR_SYSINFO  0x00

typedef struct{
	float bt_low;
	float bt_high;
	unsigned int Code_SystemCode; // Constant value.
	uint8_t  DEVEUI[8];		  //Lora DevKEY
	uint8_t  senseinfo[12];		  //...
} DATA_SystemConst;

// typedef struct{
// 	int  Int_SenseTime;     //Time by second
// 	uint8_t APPKEY[16];		//Lora AppKEY
// 	float  Val_Adjust[5];   //Adjusting value for each 5 sensors.
// } DATA_SystemInfo;

#pragma pack(push, 1)//prevent struct packing

typedef struct{
	uint8_t Type[2];//Adjusting Sensors
	float Val[2];   //Adjusting value for each 5 sensors.
} Revision;
typedef struct{
	uint8_t Type;  //Adjusting Sensors
	float d01fr;   //Adjusting value.
	float d01to;   //Adjusting value.
	float d02fr;   //Adjusting value.
	float d02to;   //Adjusting value.
	float w01fr;   //Adjusting value.
	float w01to;   //Adjusting value.
	float w02fr;   //Adjusting value.
	float w02to;   //Adjusting value.
} StatusRange;

typedef struct{
	uint64_t startdate;
	uint64_t initdate;
	int warningmin;
	int	dangermin;
	int Savetotmin;
	uint8_t StrProductbytesize;
	uint16_t* StrProduct;
	uint8_t RACllbytesize;
	uint16_t* RACll;
	uint8_t RACmmbytesize;
	uint16_t* RACmm;
	uint16_t* RACss;
	uint8_t RACssbytesize;
	int sendtime;     //Time by second
	int isreplace;
	uint8_t APPKEY[16];		//Lora AppKEY
	uint8_t Type_Adjust[5];//Adjusting Sensors
	float Val_Adjust[5];   //Adjusting value for each 2 sensors.
	StatusRange staRange; //Status range structure
	int EEPROM_Len;
	int isAvailable;
} DATA_SystemInfo;
#pragma pack(pop)

//Sending data to Sending Module
uint8_t Code_SendBuf[64];
int SendBuflen;

float Battery_Full, Battery_Low;
int emergencypresstime, ResetBtnpresstime;
int EM0Count;

SPI_HandleTypeDef *NRFSPI;
UART_HandleTypeDef *Serial;
UART_HandleTypeDef *SensorUart;
I2C_HandleTypeDef *i2c;

void WZMSBase_Init();
void WZMSBase_Loop();

uint32_t WZMS_millis();
void println(char* fmt,...);
void print(char* fmt,...);
char* float_print(float s);
char* printBin(uint8_t *buf, int len);
void PrintMemoryMap(int _showaddress, void* _address, int len, int bitRow);
void delay_ms(uint32_t ms);
int isEqualText(char* a, char* b);
int iscontainedText(char* a, char* b);

void EnableBtns();
void EnableMserial();
void EnableRstBtn();
void EnableEmergencyBtn();

void DisableBtns();
void DisableRstBtn();
void DisableEmergencyBtn();
void DisableMserial();


void My_serialEvent();
void My_EmergencyBtnEvent();
void My_RstBtnEvent();

void WZMS_LEDLoop();
void WZMS_LEDMode(int mode);
void WZMS_LEDPulse();

// WZMSCore_Base
volatile int ResetBtnpressed; int ResetEnabled, EmergencyEnabled;
GPIO_Pin emergencyBtnport , ResetBtnport;
uint8_t *DeviceCode, *NwkCode;
// Period of Milisecond of the Loop
int WZMSInterval;
volatile int emergency;	//Emergency mode (1:Active)
volatile int SWReset, SWResetup;
uint32_t start;

// WZMSData_Base
void WriteEEPROM(uint16_t addr,void* pdata,int len);
void ReadEEPROM(uint16_t addr,void* pdata,int len);


#ifdef __cplusplus
}
#endif

#endif //_WZMS2Base_h_