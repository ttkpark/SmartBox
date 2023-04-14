#include "WZMSRF.h"
#include "WZMSCore.h"

#ifdef __cplusplus
extern "C"
{
#endif
#define Debug println
extern uint8_t rx_address[5];

void DevEUI2RF(uint8_t *DevEUI,uint8_t *RF_addr);

// Initalizes the RF Module.
int  WZMSRF_init()
{
	RFCode = rx_address;
    DevEUI2RF(DeviceCode,RFCode);
    RFInit();
	return 1;
}

// Sends the data to Masterkey
int WZMSRF_Masterkey()
{
    return MasterkeyInterface();
}

// RF interface With NRF24
int MasterkeyInterface() {
    int Masterkey_Interfaced = 0;
	int retrying = 0 ,limit = 10;//,  cport=1, port = 1720;
	//char *host = "192.168.0.170";
	do {
		retrying++;
		//Debug("retrying : %d", retrying);

		println("connect to Masterkey");
		Masterkey_Interfaced = !RFsend();//if failed, it returns '1'.

		if(Masterkey_Interfaced)
		{
			extern uint8_t jsondata[128];
			Masterkey_Data(jsondata);
			println("Recv data : %s",jsondata);
		}

	} while (Masterkey_Interfaced == 0 && retrying < limit);
	if (Masterkey_Interfaced == 0)return 0;
	Debug("MasterkeyInterface() succeed");
	return 1;
}

extern char format_What[13];
extern int RFLen;
//마스터키로부터 데이터를 받아 내용을 추출하여 EEPROM에 저장하는 함수.
void Masterkey_Data(char* data) {
	println("MasterKey_Data() start");
	PrintMemoryMap(0,data, RFLen, 16);
	println("RFLen = %d", RFLen);

	//Read_SystemInfo();
	//Read_SystemConst();

	WriteEEPROM(FLASH_ADDR_SYSINFO, &RFLen, 1);
	
	WriteEEPROM(FLASH_ADDR_SYSINFO + 1,data,RFLen);
	/*
	println("len = %d", len);
	for(int i=0;i<RFLen;i++){
		a = data[i];
		print("data[%d] = %02X, wrote..", i, a);
		WriteEEPROM(i + 1, &a, 1);

		ReadEEPROM(i+1,&b,1);
		println("read = %02X", b);
	}
	*/

	/*
	uint8_t *point = data;
	
	// data [0..15]  :  APPKEY
	memcpy(Data_SystemInfo->APPKEY,data,16);
	memcpy(NwkCode,Data_SystemInfo->APPKEY,16);
	if(*((int*)(data+16)) >= 999)Data_SystemInfo->Int_SenseTime = *((int*)(data+16));
	else Data_SystemInfo->Int_SenseTime = *((int*)(data+16))*1000;

	// data printing..
	println("File 2:%s\n raw data :", File_SystemInfo);
	PrintMemoryMap(0,point, 45, 16);


	// data [20..44]  :  보정값
	for(int i=0;i<5;i++)
	{
		int num = point[20+i];
		println("Adjust Message %d : %d",i,num);
		
		if(num <= 0xF0)
		{
			float f;
			println("point 25 %s",printBin(point+25,4));
			memcpy(&f,point+25,4);
			println("Adjust Message %d %d : %s",i,num,float_print(f));
			
			//센서값과 일치하는 제일 앞의 센서포트에 조정값을 저장합니다.
			int j=0;
			for(j=0;j<5;j++)
			{
				if(SensorData[j][63] == num)
				{
					println("Val_Adjust[%d](%s) : %s",j,format_What[num],float_print(f));
					Data_SystemInfo->Val_Adjust[j] = f;
					break;
				}
			}
			if(j==5)println("No Sensor : %d(%s)",num,format_What[num]);
		}
	}
	println("File 2:%s\n raw data :", File_SystemInfo);
	PrintMemoryMap(Data_SystemInfo,Data_SystemInfo, sizeof(DATA_SystemInfo), 16);
	*/
	//Store_SystemInfo();
	//Store_systemConst();
	
	Read_SystemInfo();
	Read_SystemConst();

    char Array[256];
	ReadEEPROM(0x00,Array,256);
	PrintMemoryMap(0x00,Array, 256, 16);

	SavedInfo(0);

	println("MasterKey_Json() end");
}
;

//DevEUI     01 00 00 00  00 00  00 00
//RF_rxaddr  01 00 00            00 00
void DevEUI2RF(uint8_t *DevEUI,uint8_t *RF_addr)
{
  memcpy(RF_addr,DevEUI,3);     // number Copying
  memcpy(RF_addr+3,DevEUI+6,2); // Projext and Country Copying
  println("DEVEUI     : %s",printBin(DevEUI,8));//for(int i=0;i<8;i++)print(" %02X", DevEUI[i]);
  //println("");
  println("RF_addr    : %s",printBin(RF_addr,5));//for(int i=0;i<8;i++)print(" %02X", rx_address[i]);
  //println("");
}

#ifdef __cplusplus
}
#endif