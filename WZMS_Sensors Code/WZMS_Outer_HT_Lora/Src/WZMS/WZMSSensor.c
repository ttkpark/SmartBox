#include "WZMSSensor.h"
#include "WZMSCore.h"


#ifdef __cplusplus
extern "C"
{
#endif

#define Debug println


void I2C_BusCheck();
void ina219begin();
void MAX30205begin(void);
float getBusVoltage_V();
void UserInitalize();

//void UART_INIT();
#define UART_INIT() 

int UART_Baud;

extern DATA_SystemConst data_SystemConst;
extern DATA_SystemInfo data_SystemInfo;

// Setup for the WZMS Sensor Processing Unit
int  WZMSSensor_init()
{
  UART_INIT();
	Sense_Info = data_SystemConst.senseinfo;//"_0tp_1ox_2ps_3bt_4tp";  //System constant(Sensor Value)
  UART_Baud = 0;//9600;


  Battery_Full = data_SystemConst.bt_high, Battery_Low = data_SystemConst.bt_low;

  UserInitalize();
  return 0;
}

extern char *format_What[13];
int format_a(char* str)
{
	char buf[3];
	memcpy(buf,str,2);
	buf[2] = 0;

	for(int i=0;i<sizeof(format_What)/sizeof(char*);i++)
		if(isEqualText(format_What[i],buf))return i;

	return 0xFF;
}
// it's Equalvalent of Sensordata[i][63] to Port_SType_Table[i]
//uint8_t Port_SType_Table[5]
void PrintPortInit(char* point) {
	int a = 0; char* Format = (char*)point;//_0tp_1ox_2ps_3bt_4tp
	for(int i=0;i<5;i++)SensorData[i][63] = 0xFF;
	while (1) {
		if (*Format == 0)break;
		else if (*Format == '_')
		{
			Format++;
			a = *Format - '0';//the number of the PrintPort
			int port = a;
			a = 0; while (1)if ('0' <= Format[a] && Format[a++] <= '9')break;// the 'a' is the length of the 'tp','ox',etc.
			a++;
			
      SensorData[port][63] = format_a(Format+1);
			Debug("Format+1 = %s",Format+1);
			Debug("SensorData[%d][63] = %d",port,SensorData[port][63]);
			
			Format += a;
		}
		Format++;
	}

}

void DHT12processing();

char* resstr[5]={"HAL_OK","HAL_ERROR","HAL_BUSY","HAL_TIMEOUT","OTHER"};
void UserInitalize(){
  //WriteMemory();
  if(UART_Baud)
  {
    SensorUart->Init.BaudRate = UART_Baud;
    if (HAL_HalfDuplex_Init(SensorUart) != HAL_OK)
    {
      Error_Handler();
    }
  }

  println("APPKEY = %s",printBin(NwkCode,16));

  UART_INIT();
  //if(Port_Usage[3])Port_UART->begin(38400);
  //Port_I2C->begin();
  println("I2C_BusCheck()");
 
  I2C_BusCheck();

	println("AT24C16 Test");

	/*
  //AT24C16 Testing Code 
	uint8_t i[256] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F},a[3] = {0x04,0x05,0x06},addr;
	
	uint8_t res;

  for(int d=0;d<16;d++)
  {
    for(int j=0;j<16;j++)i[j]=(d<<4) | j;
    WriteEEPROM(d<<4,i,16);
  }

	memset(i,0xFE,0x20);

	ReadEEPROM(0x00,i,256);
	println(PrintMemoryMap(0x00,i, 256, 16));


  for(int d=0;d<8;d++)
  {
    for(int j=0;j<32;j++)i[j]=(d<<5) | j;
    WriteEEPROM(d<<5,i,32);
  }

	memset(i,0xFE,0x20);

	ReadEEPROM(0x00,i,256);
	println(PrintMemoryMap(0x00,i, 256, 16));


  for(int d=0;d<4;d++)
  {
    for(int j=0;j<64;j++)i[j]=(d<<6) | j;
    WriteEEPROM(d<<6,i,64);
  }

	memset(i,0xFE,0x20);

	ReadEEPROM(0x00,i,256);
	println(PrintMemoryMap(0x00,i, 256, 16));
  */

  
  // Initialize the INA219.
  // By default the initialization will use the largest range (32V, 2A).  However
  // you can call a setCalibration function to change this range (see comments).
  //ina219.begin();
  // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
  //ina219.setCalibration_16V_400mA();
  //double Resistance = 1;
  //ina219.ina219_currentDivider_mA=200 * Resistance;
  //ina219.ina219_powerMultiplier_mW=0.1f / Resistance;
  ina219begin();
  //getBusVoltage_V();
  WZMSSensor_processing(-1);
}

//float readTemperature();
//float read_Vcc();

/*
void oxyprocessing(char* dat);
void co2processing(char* dat);
void cmprocessing(char* dat,int len);

uint16_t UART_avail = 0;
uint16_t UART_focus = 0;
char UARTRcvBuf[128];
void UART_INIT()
{
    HAL_UART_Receive_IT(SensorUart,UARTRcvBuf,1);
}
uint8_t uart_available()
{
  return UART_avail>=UART_focus ? UART_avail-UART_focus : UART_avail-UART_focus+128;
}
uint8_t UART_readBuf()
{
  if(UART_focus==127)
  {
    UART_focus=0;
    return UARTRcvBuf[127];
  }
  else if(UART_focus != UART_avail)
  {
    return UARTRcvBuf[UART_focus++ + (UART_focus!=0?-1:+127)];
  }
  else return 0;
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart->Instance == SensorUart->Instance)
  {
    HAL_UART_Receive_IT(SensorUart,UARTRcvBuf+UART_avail,1);
    if(UART_avail<127)
    {
      UART_avail++;
    }
    else UART_avail=0;
  }
}

int cnt = 0, no = 0;
int Received=1;
uint32_t start;
char SeBuf[65];
int Recording;
int focus;

#define CHSTART(x,l)   (' '<x && x <= 127)//(x == 0xFF) //
#define CHEND(x,l)     (x == '\n')//(l>=8) //

void Port_uartprocessing() {
  while(uart_available()){
		uint8_t ch = UART_readBuf();
		//println("avail : %d 0x%02X",uart_available(),ch);
		//Serial.println((String)"avail : " + Port_UART->available());
		if(Recording){
			if(CHEND(ch,focus)){
				Recording=0;
				SeBuf[focus]=0;
				println("Buf : %s", SeBuf);
				//println("Buf : %s", printBin(SeBuf,focus));
				oxyprocessing(SeBuf);
				//co2processing(SeBuf);
				//cmprocessing(SeBuf,focus);
			}
			else SeBuf[focus++]=ch;
		}else if(CHSTART(ch,focus)){
			SeBuf[0]=ch;
      Recording=1;
			focus=1;
		}
	}
}
;
uint8_t codata[16];
void cmprocessing(char* dat, int len)
{
	float *cm_Data = (float*)&(codata[0]);
	float *cm_pre = (float*)&(codata[4]);
	float *cm_no = (float*)&(codata[8]);
	float *count = (float*)&(codata[12]);
	if(*count >= 10)*count = 0;
  (*count)++;

  char* point = memchr(dat,0x04,len);
  if(!point) (*cm_no)++;
  else
  {
    //println("Data : %s", printBin(point,8));
    //println("len  : %d", len);

    //println("Gas Type : 0x%02X", point[0]);
    //println("Unit     : 0x%02X", point[1]);
    //println("No.dec   : 0x%02X", point[2]);
    //println("Concent  : %d", point[3]<<8 | point[4]);
    //println("Max      : %d", point[5]<<8 | point[6]);
    //println("Checksum : 0x%02X", point[7]);
    
    *cm_Data = (point[3]<<8 | point[4])*0.1;
    //println("Concent  : %s", float_print(*cm_Data));
    //println("Concent  : %s", float_print((point[3]<<8 | point[4])*0.1));
  }

  if(*count == 10 && *cm_no > 8)//기준이 잘못됨.
  {
    *cm_pre = *cm_Data;
    *cm_no = 0;
  }

  if(abs(*cm_Data - *cm_pre) < 10)// 기준 갱신 (변화량이 10 이상이면)
  {
    *cm_pre = *cm_Data;
  }
  else // 실패라면
  {
    (*cm_no)++;
    *cm_Data = *cm_pre;
  }

}
uint8_t co2data[16];
void co2processing(char* dat)
{
	float *co2_Data = (float*)&(co2data[0]);
	float *co2_pre = (float*)&(co2data[4]);
	float *co2_no = (float*)&(co2data[8]);
	float *count = (float*)&(co2data[12]);
	if(*count >= 10)*count = 0;
  (*count)++;

  char* point = dat;

  for(;(point-dat)<strlen(dat);point++)
    if('0' <= *point && *point <= '9')break;
  if(!((point-dat)<strlen(dat)))
  {
    (*co2_no)++;
  }
  else
  {
    //println("Co2Data : %s",point);
    *co2_Data = atof(point);
  }

  if(*count == 10 && *co2_no > 8)//기준이 잘못됨.
  {
    *co2_pre = *co2_Data;
    *co2_no = 1;
  }

  if(abs(((*co2_Data - *co2_pre)*100) / *co2_pre) < 50)// 기준 갱신 (변동률 50% 이하이면)
  {
    *co2_pre = *co2_Data;
  }
  else // 실패라면
  {
    (*co2_no)++;
    *co2_Data = *co2_pre;
  }
}
uint8_t oxydata[96];
void oxyprocessing(char* dat) {
	float *ox_Data = (float*)&(oxydata[0]);
	float *tp_Data = (float*)&(oxydata[16]);
	float *ps_Data = (float*)&(oxydata[32]);
	float *ppo2_Dt = (float*)&(oxydata[48]);

	float *ox_pre = (float*)&(oxydata[4]);
	float *tp_pre = (float*)&(oxydata[20]);
	float *ps_pre = (float*)&(oxydata[36]);
	float *ppo2_p = (float*)&(oxydata[52]);

	int *ox_no = (int*)&(oxydata[8]);
	int *tp_no = (int*)&(oxydata[24]);
	int *ps_no = (int*)&(oxydata[40]);
	int *ppo2_no = (int*)&(oxydata[56]);

	int *oxyfoc = (int*)&(oxydata[64]);
	int *count = (int*)&(oxydata[68]);
	char *oxybuf = (char*)&(oxydata[72]);
	float rate;
	//println("oxyprocessing start");
	//oxybuf = dat;
	memcpy(oxybuf, dat, strlen(dat)+1);
	oxyfoc = 0;
	char p_or_m;
	char* p_ch;
	p_ch = strchr(oxybuf, 'O');
	if (p_ch != 0)*ppo2_Dt = atof(p_ch + 2);

	//온도센서 보정

	//차이로 계산한다.

	p_ch = strchr(oxybuf, 'T');
	if (p_ch != 0) {
		p_or_m = *((char*)(p_ch + 2));
		*tp_Data = atof(p_ch + 3);
		if (p_or_m == '-')*tp_Data = -*tp_Data;
		if (*count >= 10 && *tp_no > 8) {// 기준이 잘못됨.
			*tp_pre = *tp_Data;// 현재 측정값으로 기준 갱신
			*tp_no = 0;
		}

		//Debug("p_or_m     : %d", p_or_m);
		//Debug("*tp_Data   : %g", *tp_Data);
		//Debug("*tp_pre    : %g", *tp_pre);

		//Debug("*count     : %d", *count);
		//Debug("*tp_no     : %d", *tp_no);
		//Debug("delta temp : %g", Abs(*tp_Data - *tp_pre));
		

		if (abs(*tp_Data - *tp_pre) < 2){//차이가 2 이하일 때
			//Debug("*tp_no  1  : %d", *tp_no);
			*tp_pre = *tp_Data; //과거 센서값 갱신
		}
		else {
			(*tp_no)++;     //반영x
			//Debug("*tp_no  2  : %d", *tp_no);
			*tp_Data = *tp_pre; //센서값 = 과거 센서값
		}
	}
	else;// Debug("Temperature failed.");//수신 실패

	//Debug("*tp_no  3  : %d", *tp_no);
	//측정값 보정과정
	//기준값 대비 30%에서 300% 사이의 센서값이 측정되면
	//기준값을 그 센서 값으로 갱신하고
	//그렇지 않은 데이터는 버린다.
	//만약 기준값이 잘못되었으면 그 기준값을 현재 측정값으로 보정한다.

	p_ch = strchr(oxybuf, 'P');
	if (p_ch != 0) {
		*ps_Data = (float)atoi(p_ch + 2);

		rate = *ps_Data * 100 / *ps_pre;


		if (*count >= 10 && *ps_no > 8) { *ps_pre = *ps_Data;rate = 1; *ps_no = 0; }

		//Debug("*ps_Data   : %g", *ps_Data);
		//Debug("*ps_pre    : %g", *ps_pre);
		//Debug("*count     : %d", *count);
		//Debug("*ps_no     : %d", *ps_no);
		//Debug("rate       : %g", rate);

		if (30 < rate && rate < 300)*ps_pre = *ps_Data;
		else { (*ps_no)++; *ps_Data = *ps_pre; }

	}
	else;// Debug("pressure failed.");//수신 실패

	//Debug("*tp_no  3  : %d", *tp_no);
	p_ch = strchr(oxybuf, '%');
	if (p_ch != 0) {
		*ox_Data = (float)atof(p_ch + 2);

		rate = *ox_Data * 100 / *ox_pre;//이전과 현재의 비율 계산.

		if (*count >= 10 && *ox_no > 8) {// 기준이 잘못됨.
			*ox_pre = *ox_Data;
			rate = 1;// 현재 측정값으로 기준 갱신
			*ox_no = 0;
		}

		//Debug("*ox_Data   : %g", *ox_Data);
		//Debug("*ox_pre    : %g", *ox_pre);
		//Debug("*count     : %d", *count);
		//Debug("*ox_no     : %d", *ox_no);
		//Debug("rate       : %g", rate);

		if (30 < rate && rate < 300)//현재값이 과거의 30%에서300% 사이일 때 
			*ox_pre = *ox_Data; //과거 센서값 갱신
		else {
			(*ox_no)++;     //반영x
			*ox_Data = *ox_pre; //센서값 = 과거 센서값
		}
	}
	else;// Debug("oxy failed.");//수신 실패

	//Debug("*tp_no  3  : %d", *tp_no);
	if ((*count)++ > 10)*count = 0;

	//println("atof(\"123.42\"): %g", atof("123.42"));
	// println("oxybuf        : %s", oxybuf);
	// println("oxy           : %sppO₂", float_print(*ppo2_Dt));
	// println("Oxygen        : %s％  (%d,%s)", float_print(*ox_Data), *ox_no, float_print(*ox_pre));
	// println("Temperature   : %s℃  (%d,%s)", float_print(*tp_Data), *tp_no, float_print(*tp_pre));
	// println("Pressure      : %sh㎩ (%d,%s)", float_print(*ps_Data), *ps_no, float_print(*ps_pre));

	// *tp_Data; // Temperature(℃)
	// *ox_Data; //Oxygen(％)
	// *ps_Data; // *ps_Data(h㎩)
}
*/
float AM2320Temp = 0,AM2320Humi = 0;
void WZMSSensor_processing(int process){
  char* senseDt;
	Port_uartprocessing();
  if(process == -1){
    // float bt = 0;
    // float volt = 0;
    // volt = read_Vcc();
    // bt = (100.f*(volt-Battery_Low))/(Battery_Full-Battery_Low); 
    // if(bt > 99.f)bt = 99;
    // else if(bt < 0.f) bt = 0;
    
    //float temp = readTemperature();
    DHT12processing();
    //Sensor's Work:
    //1:extract the Sensor's Value
    //2:Save in the PortData[n] (if necessary)
    //3:copy it to the (float*)(SensorData[n]+32) (Value)
    //4:sprintf() following the spf(SensorData[n]+40) and put it in str(SensorData[n]+0)
    //senseDt = (char*)SensorData[0]; sprintf(senseDt,senseDt+40,temp);((float*)(PortData[0]))[0] = temp;

    *((float*)(PortData[0])) = AM2320Temp;// Temp
    *((float*)(PortData[1])) = AM2320Humi;// Humi
    *((float*)(PortData[3])) = 68;//bt;

    //Sensor Processing code
    for(int i=0;i<5;i++){
      println("PortData(%d) : %s", i, float_print(*((float*)(PortData[i]))));
    }
  }
}


#define DHT12_ADDRESS           0x5C

#define INA219_ADDRESS          0x40    // 1000000 (A0+A1=GND)

#define MAX30205_ADDRESS        0x48  // 8bit address converted to 7bit

// Registers
#define MAX30205_TEMPERATURE    0x00  //  get temperature ,Read only
#define MAX30205_CONFIGURATION  0x01  //
#define MAX30205_THYST          0x02  //
#define MAX30205_TOS            0x03  //
/*
void I2CreadBytes(uint8_t address, uint8_t subAddress, uint8_t * dest, uint8_t count);
void I2CwriteBytes(uint8_t address, uint8_t subAddress, uint8_t * src, uint8_t count);

void I2CwriteBytes(uint8_t address, uint8_t subAddress, uint8_t * src, uint8_t count){
  HAL_I2C_Mem_Write(i2c,address<<1,subAddress,1,src,count,1000);
}

// Wire.h read and write protocols
void I2CwriteByte(uint8_t address, uint8_t subAddress, uint8_t data){
  I2CwriteBytes(address,subAddress,&data,1);
  //Port_I2C->beginTransmission(address);  // Initialize the Tx buffer
  //Port_I2C->write(subAddress);           // Put slave register address in Tx buffer
  //Port_I2C->write(data);                 // Put data in Tx buffer
  //Port_I2C->endTransmission();           // Send the Tx buffer
}

void I2CwriteByte2(uint8_t address, uint8_t subAddress, uint16_t data){
  uint8_t write[2] = { data<<8 ,data&0xFF };
  I2CwriteBytes(address,subAddress,write,2);
  //Port_I2C->beginTransmission(address);  // Initialize the Tx buffer
  //Port_I2C->write(subAddress);           // Put slave register address in Tx buffer
  //Port_I2C->write(data>>8);                 // Put data in Tx buffer
  //Port_I2C->write(data&0xff);                 // Put data in Tx buffer
  //Port_I2C->endTransmission();           // Send the Tx buffer
}

uint8_t I2CreadByte(uint8_t address, uint8_t subAddress){
  uint8_t read;
  I2CreadBytes(address,subAddress,&read,1);
  return read;
  //uint8_t data; // `data` will store the register data
  //Port_I2C->beginTransmission(address);
  //Port_I2C->write(subAddress);
  //Port_I2C->endTransmission(false);
  //Port_I2C->requestFrom(address, (uint8_t) 1);
  //data = Port_I2C->read();
  //return data;
}


uint16_t I2CreadByte2(uint8_t address, uint8_t subAddress){
  uint8_t read[2];
  I2CreadBytes(address,subAddress,read,2);
  return read[0]<<8 | read[1];
  //uint16_t data; // `data` will store the register data
  //Port_I2C->beginTransmission(address);
  //Port_I2C->write(subAddress);
  //Port_I2C->endTransmission(false);
  //Port_I2C->requestFrom(address, (uint8_t) 2);
  //data = Port_I2C->read() << 8 | Port_I2C->read();
  //return data;
}
void I2CreadBytes(uint8_t address, uint8_t subAddress, uint8_t * dest, uint8_t count){
  HAL_I2C_Mem_Read(i2c,address<<1,subAddress,1,dest,count,1000);
  // Port_I2C->beginTransmission(address);   // Initialize the Tx buffer
  // //Next send the register to be read. OR with 0x80 to indicate multi-read.
  // Port_I2C->write(subAddress);
  // Port_I2C->endTransmission(false);
  // uint8_t i = 0;
  // Port_I2C->requestFrom(address, count);  // Read bytes from slave register address
  // while (Port_I2C->available())
  // {
  //  dest[i++] = Port_I2C->read();
  // }
}
*/

void ina219begin() {
  /*
  // Set Calibration register to 'Cal' calculated above
  I2CwriteByte2(0x40, 0x05, 8192);//INA219_REG_CALIBRATION

  // Set Config register to take into account the settings above
  uint16_t config = 0x0000 |//INA219_CONFIG_BVOLTAGERANGE_16V 0x0000(0x2000)
                    0x1800 |//INA219_CONFIG_GAIN_8_320MV(0x1800)
                    0x0700 |//(INA219_CONFIG_BADCRES_12BIT_64S_34MS)(0x0780)
                    0x0018 |//INA219_CONFIG_SADCRES_12BIT_1S_532US(0x0078)
                    0x0006; //INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS(0x0007)
  I2CwriteByte2(0x40, 0x00, config);//INA219_REG_CONFIG
  
  // uint16_t value1 = I2CreadByte2(0x40,0x00);
  // println("INA219A 0x00 : 0x%04X(%d)",value1,value1);
  // value1 = I2CreadByte2(0x40,0x05);
  // println("INA219A 0x05 : 0x%04X(%d)",value1,value1);
  //double Resistance = 1;
  //ina219.ina219_currentDivider_mA=200 * Resistance;
  //ina219.ina219_powerMultiplier_mW=0.1f / Resistance;
  */
}
/*
float getBusVoltage_V() {
  uint16_t value1;
  //I2CreadByte2(0x40,0x03);
  value1 = I2CreadByte2(0x40,0x02);//INA219_REG_BUSVOLTAGE
  //println("INA219A 0x02 : 0x%04X [BD : %03X(%d)]",value1,value1>>3,value1>>3);
  int16_t value = (int16_t)((value1 >> 3) * 4);
  return value * 0.001;
}

float MAX30205getTemperature(void){
  uint8_t readRaw[2] = {0};
  I2CreadBytes(MAX30205_ADDRESS,MAX30205_TEMPERATURE, readRaw ,2); // read two bytes
  int16_t raw = readRaw[0] << 8 | readRaw[1];  //combine two bytes
  float temperature = raw  * 0.00390625;     // convert to temperature
  return  temperature;
}
void MAX30205shutdown(void){
  uint8_t reg = I2CreadByte(MAX30205_ADDRESS, MAX30205_CONFIGURATION);  // Get the current register
  I2CwriteByte(MAX30205_ADDRESS, MAX30205_CONFIGURATION, reg | 0x80);
}
void MAX30205begin(void){
  I2CwriteByte(MAX30205_ADDRESS, MAX30205_CONFIGURATION, 0x00); //mode config
  //I2CwriteByte(MAX30205_ADDRESS, MAX30205_THYST ,      0x00); // set threshold
  //I2CwriteByte(MAX30205_ADDRESS, MAX30205_TOS,       0x00); //
}
void MAX30205CheckError(void){
  uint8_t reg = I2CreadByte(MAX30205_ADDRESS, MAX30205_CONFIGURATION);
  if(reg){
    I2CwriteByte(MAX30205_ADDRESS, MAX30205_CONFIGURATION, 0x80);
    delay_ms(100);
    I2CwriteByte(MAX30205_ADDRESS, MAX30205_CONFIGURATION, 0x00);
    delay_ms(100);
    println("Config Register Error!!");
  }
}
float Vcc_Value;
float read_Vcc() {
  Vcc_Value = getBusVoltage_V();
  println(" Voltage : %s",float_print(Vcc_Value));
  return Vcc_Value;
}
float readTemperature() {
  float T = 0.f;
  MAX30205CheckError();//Debug2("143");
  T = MAX30205getTemperature();//Debug2("144");
  println("Temperature : %s",float_print(T));
  
  return T;
}
*/

		// SBSen_AM2320_Get();
unsigned short AM2320crc16(unsigned char *ptr, unsigned char len)
{
  unsigned short crc =0xFFFF;
  unsigned char i;
  while(len--)
  {
    crc ^=*ptr++;
    for(i=0;i<8;i++)
    {
      if(crc & 0x01)
      {
        crc>>=1;
        crc^=0xA001;
      }else
      {
        crc>>=1;
      }
    }
  }
  return crc;
}
void DHT12processing(){
	uint8_t valuearr[8] = {0x03,0x00,0x04,}, res=0;

	res = HAL_I2C_Master_Transmit(i2c,0xB8,(uint8_t*)"",1,1000);
	println("%d:res = %d(%s)",__LINE__,res,res<4?resstr[res]:"Unknown");
	delay(2);


	res = HAL_I2C_Master_Transmit(i2c,0xB8,valuearr,3,1000);
	println("%d:res = %d(%s)",__LINE__,res,res<4?resstr[res]:"Unknown");

	delay(3);
	res = HAL_I2C_Master_Receive(i2c,0xB8,valuearr,8,1000);
	println("%d:res = %d(%s)",__LINE__,res,res<4?resstr[res]:"Unknown");

	if(res != HAL_OK)return;
	println("Return : %s",printBin(valuearr,8));

	unsigned short crc = AM2320crc16(valuearr,6);
	unsigned short CRc = valuearr[7]<<8 | valuearr[6];
	println("crc(%d) = CRC(%d)", crc, CRc);
	if(crc != CRc)
	{
		println("Interface error!!",float_print(AM2320Temp));
		return;
	}

	AM2320Humi = (valuearr[2]<<8 | valuearr[3]) / 10.f;
	AM2320Temp = ((valuearr[4]&0x7F)<<8 | valuearr[5]) / 10.f;
	if(valuearr[4] & 0x80)AM2320Temp = -AM2320Temp;

	println("Temp : %s",float_print(AM2320Temp));
	println("Humi : %s",float_print(AM2320Humi));

}

void I2C_BusCheck(){
  /*
  uint8_t error, address;
  int nDevices;
  //*
  println("Scanning...");
  nDevices = 0;
  for(address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    //Port_I2C->beginTransmission(address);
    //print("I2C device scanning %02X..  ",address);  
      
    error = HAL_I2C_Master_Transmit(Port_I2C,address<<1,"",1,1000);//Port_I2C->endTransmission();

    if (error == HAL_OK)
    {
      println("I2C device found at address 0x%02X !",address);
      nDevices++;
    }
    else if (error>4)
      println("Unknown error at address 0x%02X !",address);
    //else println("Error. %s(%d)",resstr[error],error);
  }
  if (nDevices == 0)
    println("No I2C devices found");
  else
    println("done");
    */
}
;

#ifdef __cplusplus
}
#endif