#include "SBMasterKey.h"
#include "Addstr.h"
#include "print.h"
#include "cJSON.h"
#include "RFInterface.h"

volatile int SWReset;
uint32_t start;

char* format_What[13] = {"bt","tp","ox","du","co","hu","ni","pp","hd","uv","ps","cm","mt"};
int num_isEqualText;
int isEqualText(char* a, char* b) {
	if (strlen(a) != (num_isEqualText = strlen(b)))return false;
	if (memcmp(a, b, num_isEqualText)) return false;
		return true;
}
int iscontainedText(char* a, char* b) {
	return strstr(a,b) != 0;
}

int format_a(char* str)
{
	char buf[3];
	memcpy(buf,str,2);
	buf[2] = 0;

	for(int i=0;i<sizeof(format_What)/sizeof(char*);i++)
		if(isEqualText(format_What[i],buf))return i;

	return 0xFF;
}


void ResetBtn() {// Connect to the MasterKey
	Debug("Connect to the MasterKey");
	Debug("50");

	int res = MasterkeyInterface();
	if (!res)//failed to communicate with Masterkey.
	{
		Debug("571");
		SWReset = 0;
		return;//exit
	}

	Debug("58");
	SWReset = 0;
	Debug("59");
	start = millis();
	Debug3("1370:start = %d",start);/////////////////////////////////////
}


extern "C" int RFsend();

// RF interface With NRF24
int MasterkeyInterface() {
  int Masterkey_Interfaced = 0;
	int retrying = 0 ,limit = 10;
	do {
		retrying++;
		yield();
		println("connect to Masterkey");
		Masterkey_Interfaced = !RFsend();//if failed, it returns '1'.

		yield();
		if(Masterkey_Interfaced)
		{
			yield();
			extern uint8_t jsondata[128];
			Masterkey_Data((char*)jsondata);
			println("Recv data : %s",jsondata);
		}

	} while (Masterkey_Interfaced == 0 && retrying < limit);
	if (Masterkey_Interfaced == 0)return 0;
	Debug("MasterkeyInterface() succeed");
	return 1;
}

extern int RFLen;
//마스터키로부터 데이터를 받아 내용을 추출하여 EEPROM에 저장하는 함수.
void Masterkey_Data(char* data) {
	println("MasterKey_Data() start");
	PrintMemoryMap(0,data, RFLen, 16);
	println("RFLen = %d", RFLen);

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

