#include "WZMSInterface.h"
#include "WZMSCore.h"


#ifdef __cplusplus
extern "C"
{
#endif
#define Debug println

/*
// Setup for WZMS Interface. (LoRa, WiFi, etc.)
int  WZMSInt_init()
{
}

// Get from Interfacing Module.
int  WZMSInt_stat()
{
    ;
}

// Sends the packet to the Module.
void WZMSInt_Data()
{
    ;
}
*/
void WZMSInt_Data()
{
    WZMSInt_DataProcess();
    // Code_SendBuf
    WZMSIntBase_Data();
}
// Processing the packet.
//  -> Code_SendBuf
extern char *format_What[13];
void WZMSInt_DataProcess()
{
	Debug("WZMSInt_DataProcess() start");
	if(emergency)
	{
		Code_SendBuf[0] = 0x09;//Em1
		Code_SendBuf[1] = 0xE0;
		*((float*)&Code_SendBuf[2]) = 1.0;
		SendBuflen = 6;
	}
	else if(EM0Count > 0)
	{
		if(WZMSInt_stat() == 1)EM0Count--;
		Code_SendBuf[0] = 0x09;//Em0
		Code_SendBuf[1] = 0xE0;
		*((float*)&Code_SendBuf[2]) = 0.0;
		SendBuflen = 6;
	}
	else
	{
		int len=1;

		Code_SendBuf[0] = 0x02;//dat
		//char *ptr = DataEncoding(Code_SendBuf);
		//Print the packet

		for(int i=0;i<5;i++){
			if(SensorData[i][63] != 0xFF)
			{
				len+=5;
				Code_SendBuf[len-5] = SensorData[i][63];
				float f = ((float*)(PortData[i]))[0] + Data_SystemInfo->Val_Adjust[i];
				print("%s ",printBin(&f,4));
				print("%9s ",float_print(f));
				memcpy(Code_SendBuf+(len - 4),&f,4);
				println("sensor %d : %d,[%d,%s] %s",i,len/5,SensorData[i][63],format_What[SensorData[i][63]],float_print(*((float*)Code_SendBuf+(len - 4))));
			}
		}
		SendBuflen = len;
	}
	
	Debug("WZMSInt_DataProcess() end");
}


#ifdef __cplusplus
}
#endif