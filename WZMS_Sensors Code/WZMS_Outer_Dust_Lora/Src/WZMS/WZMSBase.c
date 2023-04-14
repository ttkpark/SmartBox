#include "WZMSBase.h"


#ifdef __cplusplus
extern "C"
{
#endif

#define Debug println
#define Debug2 println
#define Debug3 println

volatile void INT_serialEvent();
volatile void INT_EmergencyBtnEvent();
volatile void INT_RstBtnEvent();

const GPIO_Pin Unused_Pin = {0,};
#define SW_SERIAL_UNUSED_PIN Unused_Pin

GPIO_Pin Port_SEINT = Unused_Pin;
GPIO_Pin Port_Emerge = Unused_Pin, Port_Rst = Unused_Pin;
GPIO_Pin Port_SDA = Unused_Pin, Port_SCL = Unused_Pin;
GPIO_Pin Port_TX = Unused_Pin, Port_RX = Unused_Pin;
GPIO_Pin Port_LEDR = Unused_Pin, Port_LEDG = Unused_Pin;
GPIO_Pin Port_DSENSORSET = Unused_Pin;

volatile uint8_t Emstarted, Rststarted;
volatile uint8_t EmBtnstate, RstBtnstate;

extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef hlpuart1;
extern UART_HandleTypeDef huart1;
extern SPI_HandleTypeDef hspi1;

volatile uint32_t EmerBtnstart, RstBtnstart;

char* format_What[13] = {"bt","tp","ox","du","co","hu","ni","pp","hd","uv","ps","cm","mt"};

void WZMSBase_Init()
{
	WZMSInterval = 15000;
    NRFSPI = &hspi1;
    Serial = &hlpuart1;
    i2c = &hi2c1;
	SensorUart = &huart1;
    Port_SDA = SW_SERIAL_UNUSED_PIN, Port_SCL = SW_SERIAL_UNUSED_PIN;

	Port_TX = SW_SERIAL_UNUSED_PIN, Port_RX = SW_SERIAL_UNUSED_PIN;
	Port_SEINT = SW_SERIAL_UNUSED_PIN;
	Port_Emerge = byte2GPIO_Pin(0xAC), Port_Rst = byte2GPIO_Pin(0xAB);
	Port_LEDR = byte2GPIO_Pin(0xB4), Port_LEDG = byte2GPIO_Pin(0xB5);
	Port_DSENSORSET = byte2GPIO_Pin(0xAF);
	
	EM0Count = 0;emergency = 0;SWReset = 0;SWResetup = 0;
	
	emergencypresstime = 2000, emergencyBtnport = Port_Emerge;
	ResetBtnpresstime = 2000, ResetBtnport = Port_Rst;
    
	pinMode(emergencyBtnport, INPUT_PULLUP);
	pinMode(ResetBtnport, INPUT_PULLUP);

	pinMode(Port_LEDR, OUTPUT);
	pinMode(Port_LEDG, OUTPUT);

	pinMode(Port_DSENSORSET, OUTPUT);
	digitalWrite(Port_DSENSORSET,HIGH);

	Emstarted = 0; Rststarted = 0;
	EnableBtns();
	WZMS_LEDMode(0);
}

uint32_t WZMS_millis()
{
    return millis();
}

int ButtonPressed;
			
void WZMSBase_Loop()
{
	if(ButtonPressed){WZMS_LEDPulse();ButtonPressed=0;}
}

int num_isEqualText;
int isEqualText(char* a, char* b) {
	if (strlen(a) != (num_isEqualText = strlen(b)))return false;
	if (memcmp(a, b, num_isEqualText)) return false;
		return true;
}
int iscontainedText(char* a, char* b) {
	return strstr(a,b) != 0;
}
/*
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
*/
void PrintMemoryMap(int _showaddress, void* _address, int len, int bitRow) {
	Debug("PrintMemoryMap() start");
	char *_Buf1 = (char*)malloc(bitRow + 1);
	memset(_Buf1, 0, bitRow + 1);
	char* addr = (char*)_address;

	char* newline = "\r\n";
	print("Address : 0x%08X%s", _showaddress, newline);
	int remainbyte = len % bitRow;
	int focus = 0;

	//Debug((String)"focus      : " + focus);
	//Debug((String)"remainbyte : " + remainbyte);
	//Debug((String)"len        : " + len);
	//Debug((String)"bitRow     : " + bitRow);
	//Debug((String)"Consistence: " + len + "=" + (len-remainbyte) + "+" + remainbyte);

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

char* printBin(uint8_t *buf, int len)
{
	for(int i=0;i<len;i++)sprintf(sbufx+i*3," %02X",buf[i]);
	return sbufx;
}
// void delay_ms(uint32_t ms)
// {
// 	delay(ms);
// }


//Interrupt functions
void DisableUserInterrupt() 
{
	
}
void EnableUserInterrupt() 
{
	
}
void EnableBtns()
{
	ResetEnabled = 1;
	EmergencyEnabled = 1;
	EnableRstBtn();
	EnableEmergencyBtn();
}
void DisableBtns()
{
	ResetEnabled = 0;
	EmergencyEnabled = 0;
	DisableRstBtn();
	DisableEmergencyBtn();
}

void EnableRstBtn() {
	attachInterrupt(digitalPinToInterrupt(ResetBtnport), INT_RstBtnEvent, CHANGE);
	Pinpullstate(GPIOA,GPIO_PIN_11,GPIO_PULLUP);
}
void EnableEmergencyBtn() {
	attachInterrupt(digitalPinToInterrupt(emergencyBtnport), INT_EmergencyBtnEvent, CHANGE);
	Pinpullstate(GPIOA,GPIO_PIN_12,GPIO_PULLUP);
}
void DisableRstBtn() {
	detachInterrupt(digitalPinToInterrupt(ResetBtnport));
}
void DisableEmergencyBtn() {
	detachInterrupt(digitalPinToInterrupt(emergencyBtnport));
}


volatile void INT_EmergencyBtnEvent() {
	My_EmergencyBtnEvent();
}
volatile void INT_RstBtnEvent() {
	My_RstBtnEvent();
}



void My_EmergencyBtnEvent() {
	Debug("Emergency!!!");
	if(!digitalRead(emergencyBtnport)) // FALLING Edge, start
	{
		WZMS_LEDMode(1);
		EmerBtnstart = WZMS_millis();
	}
	else // RISING Edge, end. process.
	{
		Debug("now() - EmerBtnstart : %4d  > %d",WZMS_millis() - EmerBtnstart,emergencypresstime);
		if ((WZMS_millis() - EmerBtnstart) > emergencypresstime) {
			emergency = !emergency;
			Debug("emergency!!!");
			Debug("emergency : %d", emergency);
			if (!emergency)EM0Count = 2; // send after ending of the Em Mode.
			ButtonPressed = 1;
		}
	}
}
void My_RstBtnEvent() {
	if (SWReset)return;
	Debug("Interrupt!!!");
	if(!digitalRead(ResetBtnport)) // FALLING Edge, start
	{
		WZMS_LEDMode(1);
		RstBtnstart = WZMS_millis();
	}
	else // RISING Edge, end. process.
	{
		Debug("now() - RstBtnstart : %4d  > %d",WZMS_millis() - RstBtnstart,ResetBtnpresstime);
		if ((WZMS_millis() - RstBtnstart) > (int)(ResetBtnpresstime*1.1)) {
			Debug("SWReset!!!");
			SWReset = 1;
			ButtonPressed = 1;
		}
	}
}

void WZMS_LEDG(int on)
{
	digitalWrite(Port_LEDG,on);
}
void WZMS_LEDR(int on)
{
	digitalWrite(Port_LEDR,on);
}

int LEDGon,LEDRon;
int LEDLoopCount;
int LEDstate,StateOffset = 0;
void WZMS_LEDMode(int mode)
{
	LEDstate = mode;
	StateOffset = 0;
}
//LED 점멸을 제어한다.
void WZMS_LEDLoop()
{
	//println("WZMS_LEDLoop");
	if(LEDLoopCount++ >= 80)LEDLoopCount=0;//제어 주기 20: 1초 주기  40:2초 주기
	if(LEDstate==0) // Do Nothing
	{
		LEDGon = 0, LEDRon = 0;
	}
	else if(LEDstate==1) // button Toggle
	{
		StateOffset++;
		if(StateOffset>2)LEDstate=0;
		LEDGon = 1 ,LEDRon = 1;
	} 
	else if(LEDstate==2)return;
	else if(LEDstate==3) // Error
	{
		LEDGon = 1, LEDRon = 1;
	}
	else if(LEDstate==4) // Not connected to the Network
	{
		LEDGon = 0;
		if(LEDLoopCount>>2 == 0)LEDRon = 1;
		else LEDRon = 0;
	}
	else if(LEDstate==5) // Connected to the Network
	{
		LEDRon = 0;
		if(LEDLoopCount>>2 == 0)LEDGon = 1;
		else LEDGon = 0;
	}
	else if(LEDstate==6) // Emergency Mode (Not Connected)
	{
		LEDRon = 1;
		if(LEDLoopCount>>2 == 0)LEDGon = 1;
		else LEDGon = 0;
	}
	else if(LEDstate==7) // Emergency Mode (Connected)
	{
		LEDGon = 1;
		if(LEDLoopCount>>2 == 0)LEDRon = 1;
		else LEDRon = 0;
	}
	WZMS_LEDR(LEDGon);//회로가 바뀌었기 때문에 
	WZMS_LEDG(LEDRon);//R,G를 바꿔준다.
}
void WZMS_LEDPulse()
{
	WZMS_LEDMode(2);
	LEDRon = 0;
	WZMS_LEDG(0);
	WZMS_LEDR(0);
	delay_ms(80);
	for(int i=0;i<12;i++)
	{
		switch (i&3)
		{
		case 0:WZMS_LEDG(1);break;
		case 1:WZMS_LEDG(0);break;
		case 2:WZMS_LEDR(1);break;
		case 3:WZMS_LEDR(0);break;
		default:
			break;
		}
		
		println("WZMS_LEDPulse %d",i);
		delay_ms(80);
	}
	WZMS_LEDMode(0);
}



extern char* resstr[5];
void WriteEEPROM(uint16_t addr,void* _pdata,int len)
{
	println("WriteEEPROM start");
	char* pdata = _pdata;
	int res;

	//Sector Devide
	//start padding : addr%16 , startbyte = 16-startpad
	int lens = len;
	int startbyte = 16-(addr&15);
	println("start padding  addr%%16 : %d , startbyte : %d",addr&15,startbyte);

	if(addr&15)//startpad != 0
	{
		lens -= startbyte;
		if(lens <= 0){lens = 0; startbyte = len;}
		//println("padding HAL_I2C_Mem_Write(i2c,0xA0,%02X,1,%s,%02X,1000) res : %s",addr&0xFF ,printBin(pdata,startbyte) ,startbyte ,resstr[res<4?res:4]);
		res = HAL_I2C_Mem_Write(i2c,0xA0,addr&0xFF,1,pdata,startbyte,1000);
		println("padding Write Memory 0x%03X ~ 0x%03X res : %s",addr ,addr + startbyte-1 ,resstr[res<4?res:4]);
		pdata += startbyte; addr += startbyte;
	}

	//middle
	//count = len/16
	int count = lens/16;
	for(int i=0;i<count;i++)
	{
		lens -= 16;
		//println("middle  HAL_I2C_Mem_Write(i2c,0xA0,%02X,1,%s,%02X,1000) res : %s",addr&0xFF ,printBin(pdata,16) ,16 ,resstr[res<4?res:4]);
		res = HAL_I2C_Mem_Write(i2c,0xA0,addr&0xFF,1,pdata,16,1000);
		println("middle  Write Memory 0x%03X ~ 0x%03X res : %s",addr ,addr + 16-1 ,resstr[res<4?res:4]);
		pdata += 16;addr += 16;
	}

	//ending
	if(lens>0)
	{
		//println("ending  HAL_I2C_Mem_Write(i2c,0xA0,%02X,1,%s,%02X,1000) res : %s",addr&0xFF ,printBin(pdata,lens) ,lens ,resstr[res<4?res:4]);
		res = HAL_I2C_Mem_Write(i2c,0xA0,addr&0xFF,1,pdata,lens,1000);
		println("ending  Memory 0x%03X ~ 0x%03X res : %s",addr ,addr + lens-1 ,resstr[res<4?res:4]);
	}
	/*
	int Timeout = 0;
	uint8_t *pData = _pdata;
	if((addr&0xF) + len > 16){
		Timeout = 0;
		while(HAL_I2C_Mem_Write(i2c, (uint16_t)0xA0, (uint16_t)addr, I2C_MEMADD_SIZE_8BIT, pData, (uint16_t)16-(addr&0xF), 1000) != HAL_OK && Timeout++ < 10);
		*pData = *pData + (16-(addr&0xF));
		Timeout = 0;
		while(HAL_I2C_Mem_Write(i2c, (uint16_t)0xA0, (uint16_t)addr, I2C_MEMADD_SIZE_8BIT, pData, (uint16_t)(((addr&0xF)+len)-16), 1000) != HAL_OK && Timeout++ < 10);
	}
	else
	{
		while((len - 16) > 0){
			Timeout = 0;
			while(HAL_I2C_Mem_Write(i2c, (uint16_t)0xA0, (uint16_t)addr, I2C_MEMADD_SIZE_8BIT, pData, (uint16_t)16, 1000) != HAL_OK && Timeout++ < 10);
			len -= 16;
			pData += 16;
			addr += 16;
		}
		Timeout = 0;
		while(HAL_I2C_Mem_Write(i2c, (uint16_t)0xA0, (uint16_t)addr, I2C_MEMADD_SIZE_8BIT, pData, (uint16_t)len, 1000) != HAL_OK && Timeout++ < 10);
	}
    delay(3);
	*/
}
void ReadEEPROM(uint16_t addr,void* pdata,int len)
{
	// int res;
	// uint8_t Addr = addr&0xFF;
	// uint8_t i2cAddr = 0xA0;
	
	// HAL_I2C_Master_Transmit(i2c,i2cAddr,&Addr,1,1000);
	// res = HAL_I2C_Master_Receive(i2c,i2cAddr,pdata,len,1000);
	// println("%d Read %02X memory %d with return %s(%u)",addr,i2cAddr,Addr,resstr[res<4?res:4],res);

	uint8_t *pData = pdata;
	int Timeout = 0;
	while((len - 16) > 0){
		Timeout = 0;
		while(HAL_I2C_Mem_Read(i2c, (uint16_t)0xA0, (uint16_t)addr, I2C_MEMADD_SIZE_8BIT, pData, (uint16_t)16, 1000) != HAL_OK && Timeout++ < 10);
		len -= 16;
		pData += 16;
		addr += 16;
	}
	Timeout = 0;
	while(HAL_I2C_Mem_Read(i2c, (uint16_t)0xA0, (uint16_t)addr, I2C_MEMADD_SIZE_8BIT, pData, (uint16_t)len, 1000) != HAL_OK && Timeout++ < 10);

}

#ifdef __cplusplus
}
#endif