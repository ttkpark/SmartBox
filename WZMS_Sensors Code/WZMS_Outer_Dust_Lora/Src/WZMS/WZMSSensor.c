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

void UART_INIT();
//#define UART_INIT() 

int UART_Baud;

extern DATA_SystemConst data_SystemConst;
extern DATA_SystemInfo data_SystemInfo;

// Setup for the WZMS Sensor Processing Unit
int  WZMSSensor_init()
{
  //UART_INIT();
	Sense_Info = data_SystemConst.senseinfo;//"_0tp_1ox_2ps_3bt_4tp";  //System constant(Sensor Value)
  UART_Baud = 0;


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

  // println("APPKEY = %s",printBin(NwkCode,16));

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
  getBusVoltage_V();
  WZMSSensor_processing(-1);
}

//float readTemperature();
float read_Vcc();


void duprocessing(char* dat,int len);
// void oxyprocessing(char* dat);
//void co2processing(char* dat);
//void cmprocessing(char* dat,int len);

extern uint16_t UART_avail;
uint16_t UART_focus = 0;
extern uint8_t UARTRcvBuf[128];
extern uint8_t UARTRcvData[2];
void UART_INIT()
{
    // HAL_UART_Receive_IT(SensorUart,UARTRcvBuf,1);
  // HAL_UART_Receive_IT(SensorUart,UARTRcvData,1);
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
    UART_focus++;
    return UARTRcvBuf[UART_focus + (UART_focus!=1?-1:+127)];
  }
  else return 0;
}
// void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
// {
//   Debug("@");
//   if(huart->Instance == SensorUart->Instance)
//   {
//     Debug("#");
//     HAL_UART_Receive_IT(SensorUart,UARTRcvBuf+UART_avail,1);
//     if(UART_avail<127)
//     {
//       UART_avail++;
//     }
//     else UART_avail=0;
//   }
// }

int cnt = 0, no = 0;
int Received=1;
uint32_t start;
char SeBuf[65];
int Recording;
int focus = 0;

#define CHSTART(x,l)   (x == 0x42) //(' '<x && x <= 127)//
#define CHSTART2(x,l)  (x == 0x4D) //(' '<x && x <= 127)//
#define CHEND(x,l)     (l>=x) //(x == '\n')//

int uartgetlen = 0;
void Port_uartprocessing() {
  print("@");
	// println("Port_uartprocessing()");
  // print("avail : %d,",uart_available());
  while(uart_available()){
		uint8_t ch = UART_readBuf();
    
		// println("avail : %d 0x%02X",uart_available(),ch);
		//Serial.println((String)"avail : " + Port_UART->available());
		// println("Recording : %d, focus : %d",Recording,focus);
    if(Recording == 4){
			if(CHEND(uartgetlen + 4,focus)){
				Recording=0;
				SeBuf[focus]=0;
				// println("Buf : %s", SeBuf);
				// println("Buf : %s", printBin(SeBuf,focus));
		    // println("uartgetlen : %d",uartgetlen);
				duprocessing(SeBuf,focus);
				// oxyprocessing(SeBuf);
				//co2processing(SeBuf);
				//cmprocessing(SeBuf,focus);
			}
			else SeBuf[focus++]=ch;
		}else if(Recording == 3){
			SeBuf[3]=ch;
      uartgetlen = (uartgetlen&0xFF00) | ch;
      if(uartgetlen > 256)uartgetlen = 28;
      Recording = 4;
      focus = 4;
    }else if(Recording == 2){
			SeBuf[2]=ch;
      uartgetlen = ch<<8;
      if(uart_available()>0){
		    uint8_t ch1 = UART_readBuf();
			  SeBuf[3]=ch1;
        uartgetlen = (uartgetlen&0xFF00) | ch1;
        if(uartgetlen > 256)uartgetlen = 28;
        Recording = 4;
        focus = 4;
      }else{
        Recording = 3;
        focus = 3;
      }
    }else if(Recording == 1){
			SeBuf[1]=ch;
      if(CHSTART2(ch,focus)){//0x4D identify
        Recording = 2;
			  focus = 2;
      }else{
        Recording = 0;
			  focus = 0;
      }
    }else if(CHSTART(ch,focus)){//0x42 identify
			SeBuf[0]=ch;
      if(uart_available()>0){
		    uint8_t ch1 = UART_readBuf();
			  SeBuf[1]=ch1;
        if(CHSTART2(ch1,focus)){
          Recording = 2;
          focus = 2;
        }
      }else{
        Recording = 1;
        focus = 1;
      }
		}
	}
}
uint8_t dudata[16];
void duprocessing(char* dat, int len)
{
	// println("len : %d", len);
	// println("dat : %s", printBin(dat,len));
  uint16_t ErrorCheck = 0;
  for(int i=0;i<30;i++){
    ErrorCheck += dat[i];
  }
  if(ErrorCheck != (dat[30]<<8|dat[31])){
    println("len : %d", len);
    println("dat : %s", printBin(dat,len));
    println("uartgetlen : %d",uartgetlen);
    println("Check Error (0x%04X != 0x%04X)",ErrorCheck, dat[30]<<8|dat[31]);
    return;
  }
	float *du_Data3 = (float*)&(dudata[0]);
	float *du_Data4 = (float*)&(dudata[4]);
	float *du_Data5 = (float*)&(dudata[8]);
  *du_Data3 = dat[10]<<8|dat[11];//  PM1.0 μ g/m3
  *du_Data4 = dat[12]<<8|dat[13];//  PM2.5 μ g/m3
  *du_Data5 = dat[14]<<8|dat[15];//  PM10  μ g/m3
}
extern GPIO_Pin Port_DSENSORSET;
void Sensor_On()
{
	digitalWrite(Port_DSENSORSET,HIGH);
}
void Sensor_Off()
{
	digitalWrite(Port_DSENSORSET,LOW);
}
#define DustSensorTime 30000
uint32_t t1 = 0;
volatile uint8_t isOxySensorOn;
void WZMSSensor_processing(int process){
  char* senseDt;
  if(millis() - t1 > 500){
    t1 = millis();
    Port_uartprocessing();
  }
  // println("WZMSInterval : %d   process : %d,  hh = %d", WZMSInterval, process, WZMSInterval - 5000);
  if(process == -1){
    isOxySensorOn = 0;
    if(WZMSInterval > DustSensorTime)
    {
      println("Sensor_OFF");
      Sensor_Off();
      isOxySensorOn = false;
    }
    //Sensor's Work:
    //1:extract the Sensor's Value
    //2:Save in the PortData[n] (if necessary)
    //3:copy it to the (float*)(SensorData[n]+32) (Value)
    //4:sprintf() following the spf(SensorData[n]+40) and put it in str(SensorData[n]+0)

    ((float*)(PortData[0]))[0] = *((float*)&(dudata[4]));//du
    ((float*)(PortData[3]))[0] = 99;//bt

    //Sensor Processing code
    for(int i=0;i<5;i++){
      println("PortData(%d) : %s", i, float_print(*((float*)(PortData[i]))));
    }
  }
  else if(!isOxySensorOn && WZMSInterval - DustSensorTime < process && WZMSInterval > DustSensorTime || WZMSInterval <= DustSensorTime)
  {
    println("Sensor_ON");
    Sensor_On();
    isOxySensorOn = true;
  }
}




#define INA219_ADDRESS                         (0x40)    // 1000000 (A0+A1=GND)

// #define MAX30205_ADDRESS        0x48  // 8bit address converted to 7bit

// // Registers
// #define MAX30205_TEMPERATURE    0x00  //  get temperature ,Read only
// #define MAX30205_CONFIGURATION  0x01  //
// #define MAX30205_THYST          0x02  //
// #define MAX30205_TOS            0x03  //


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
void ina219begin() {
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
}
float getBusVoltage_V() {
  uint16_t value1;
  //I2CreadByte2(0x40,0x03);
  value1 = I2CreadByte2(0x40,0x02);//INA219_REG_BUSVOLTAGE
  //println("INA219A 0x02 : 0x%04X [BD : %03X(%d)]",value1,value1>>3,value1>>3);
  int16_t value = (int16_t)((value1 >> 3) * 4);
  return value * 0.001;
}
/*
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
*/
float Vcc_Value;
float read_Vcc() {
  Vcc_Value = getBusVoltage_V();
  println(" Voltage : %s",float_print(Vcc_Value));
  return Vcc_Value;
}
/*
float readTemperature() {
  float T = 0.f;
  MAX30205CheckError();//Debug2("143");
  T = MAX30205getTemperature();//Debug2("144");
  println("Temperature : %s",float_print(T));
  
  return T;
}
*/
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

#ifdef __cplusplus
}
#endif