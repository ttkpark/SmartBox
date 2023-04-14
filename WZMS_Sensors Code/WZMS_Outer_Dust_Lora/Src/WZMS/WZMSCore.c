#include "WZMSCore.h"

#ifdef __cplusplus
extern "C"
{
#endif


#define Debug println
#define Debug2 println
#define Debug3 println

extern char *Sense_Info;			//System constant(Sensor Value)
extern char *Format_Sense;			//System constant(for Making text)
extern char *Format_Sendstr;		//System constant

uint32_t blink;
uint32_t LoopT;
// Initalizes the WZMS Core.
void WZMS_Init()
{
	WZMSBase_Init();
	WZMSData_init();

	WZMSInt_preset();

	WZMSSensor_init();
	WZMSInt_init();
	WZMSRF_init();

	// main()

	
	// Setup()
	
	//EM0Count = 0;

	EnableBtns();

	println("0");
	PrintPortInit(Sense_Info);

	SavedInfo();

	println("7.1");

	
	LoopT = blink = WZMS_millis();
}

float process;
extern GPIO_Pin debugled;//PA8
// Manages the WZMS Core.
void WZMSLoop()
{
    /*
	if(WZMS_millis()-blink > 1000)
    {
      //read_Vcc();
      int stat = digitalRead(debugled);
      digitalWrite(debugled,!stat);
      delay_ms(10);
      digitalWrite(debugled,stat);
      
      blink = ((int)(WZMS_millis()/1000))*1000;

      println("[%8ld] debugled Toggled",micros());
    }
	*/
	WZMSInt_Loop();
	WZMSBase_Loop();
	

	//Debug2("13:%d,%d",Masterkeymode,isdebugmode);
	if (SWReset) { println("330:ResetBtn()"); ResetBtn(); return; }
	if (SWResetup) { println("355:Resetup()"); Resetup(); return; }
	//if (nowa > Data_SystemInfo->Int_SenseTime*200 && nowa < Data_SystemInfo->Int_SenseTime*300 && blinkstate==1)digitalWrite(14,blinkstate=0);
	//if (nowa > Data_SystemInfo->Int_SenseTime*700 && nowa < Data_SystemInfo->Int_SenseTime*800 && blinkstate==0)digitalWrite(14,blinkstate=1);

	//Non-Emergency process rotuine
	//
	//1:Reading the data from Sensors.
	//2:In printing time,
	//  It makes the send String
	//  and connects to sip(Server IP).
	//3:?? ?d,???,Error processing.
	if (!emergency) {
		if(millis() - LoopT > 500){
			LoopT = millis();
			if(WZMSInt_stat()==1)WZMS_LEDMode(5);
			else if(WZMSInt_stat()==2)WZMS_LEDMode(3);
			else WZMS_LEDMode(4);

			if(WZMSInterval == 0)WZMSInterval = 15000;

			process = ((WZMS_millis() - start) / (float)WZMSInterval)*100.f;

			//Debug2("process : %d,%g", WZMS_millis() - start, process);
			//1:Reading the data from Sensors.
			WZMSSensor_processing(WZMS_millis() - start);

			//yield();

			//Debug2("15");
			
			//Debug2("17");
			// delay_ms(100);
			//Debug2("process : %f", process);
			if (process > 100) {
				Debug2("181");
				WZMSSensor_processing(-1);
				Debug2("182");
				//time setting
				start = WZMS_millis();/////////////////////////////////////////////////////////
				//if WiFi is unconnected , Resetup() to reconnect to the AP.
				Debug2("183");
				//TimeReseted=0;
				

				//2:In printing time,
				//  It makes the send String
				//  and connects to sip(Server IP).

				WZMSInt_Data();

			}
		}
	}
	//Emergency mode
	//1:Reading the data from Sensors.
	//2:Trying to connect to the Server.
	//3:Sending datas to Servers to send a Emergency Message.
	else
	{
		if(WZMSInt_stat()==1)WZMS_LEDMode(7);
		else if(WZMSInt_stat()==2)WZMS_LEDMode(3);
		else WZMS_LEDMode(6);
		if(WZMS_millis() - start > WZMSInt_EmergencyTime)
		{
			WZMSSensor_processing(-1);
			
			println("Emergency Mode");
			//yield();

			//time setting
			start = WZMS_millis();/////////////////////////////////////////////////////////
			
			//TimeReseted=0;
			WZMSInt_Data();
		}
	}
	//yield();

	//Debug2("19");
}

void ResetBtn() {// Connect to the MasterKey
	DisableBtns();
	//disConnectNet();
	Debug("Connect to the MasterKey");
	Debug("50");

	int res = MasterkeyInterface();
	if (!res)//failed to communicate with Masterkey.
	{
		Debug("571");
		SWReset = 0;
		EnableBtns();
		return;//exit
	}
	EnableBtns();
	int trying_connect = 1,trying=0;
	while(trying < trying_connect)
	{
		Debug("Resetup in MasterkeyInterface() %d", trying);
		if(Resetup()==1)break;
		trying++;
	}
	if(trying >= trying_connect)//AP connection failed.
	{
		Debug("862");
		SWReset = 0;
		return;//exit
	}

	Debug("58");
	SWReset = 0;
	Debug("59");
	start = WZMS_millis();
	Debug3("1370:start = %d",start);/////////////////////////////////////
	WZMSInt_ReConnect();
}

int Resetup() {// Connect to the AP
	WZMSInt_ReConnect();
	return 1;
}


#ifdef __cplusplus
}
#endif