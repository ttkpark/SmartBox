
#include "WZMSBase.h"

#ifndef _WZMS2Sensor_h_
#define _WZMS2Sensor_h_

#ifdef __cplusplus
extern "C"
{
#endif

char *Sense_Info;	//System constant(Sensor Value)
char *Format_Sense;					//System constant(for Making text)
char *Format_Sendstr;		//System constant

uint8_t PortData[5][64];		//for Sensing
uint8_t SensorData[5][64];		//for Printing

// Setup for the WZMS Sensor Processing Unit
int  WZMSSensor_init();

// Sensor Processing
void WZMSSensor_processing(int) __attribute__((weak));

// it's Equalvalent of Sensordata[i][63] to Port_SType_Table[i]
//uint8_t Port_SType_Table[5]
void PrintPortInit(char* point);
#ifdef __cplusplus
}
#endif

#endif //_WZMS2Sensor_h_