#include "RFInterface.h"
#include "Addstr.h"
#include "cJSON.h"
#include "nrf24.h"
#include "print.h"

#ifdef __cplusplus
extern "C"
{
#endif*

char uartbuf[32];
uint8_t jsondata[512];
int uartlen0=0;
char uartint_ch;
uint8_t RFres;
int RFLen;

uint8_t temp;
uint8_t q = 0;
volatile int Start=0,Start2=0,count=0;
uint8_t data_array[32];
uint8_t tx_address[5] = {0xD7,0xD7,0xD7,0xD7,0xD7};
uint8_t rx_address[5] = {0x09,0x00,0x00,0x00,0x00};

uint8_t DeviceCode[8] = {0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
extern uint8_t payload_len;

#define DebugRF println
#define DebugRFn print
#define WZMS_millis millis
#define WZMS_LEDMode 
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
void RFInit(void)
{
  /* USER CODE BEGIN 1 */
  //xfunc_out = (void(*)(uint8_t))uart_set;
  memset(data_array,0,32);
  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  //HAL_Init();
  /* init hardware pins */

  println("Start!!");
  nrf24_init();
  
  /* Channel #2 , payload length: 4 */
  nrf24_config(2,32);

  /* Set the device addresses */
  nrf24_tx_address(tx_address);
  nrf24_rx_address(rx_address);

  //println("");

  //nrf24_powerUpTx();
  //print("> ");
  printDetails();
  println("TX device ready");
  nrf24_powerDown();
}

int RFsend()
{
  //println("[%8ld] RFsend START",micros());
  Start=1;
  RFInit();
  while(Start)
  {
    //println("[%8ld] Start : %d",micros(),Start);
    if(Start==2){
      RFres = 1;
      nrf24_powerDown();
      WZMS_LEDMode(3);
      delay(1000);
      Start=0;
    }
    if(Start==3){
      RFres = 0;
      nrf24_powerDown();
      printDetails();
      Start=0;
    }
    if(Start)
    {

      DebugRF("TX Start.");
      yield();
      // printDetails();
      count++;
      
      //Receive 1-1
      // 0x01) Requesting ResetBtn Interface
      //Inital        : Header 3Byte    + ComingAddr 5Byte          + DevEUI 8Byte                            +  Extra dummy's  (Total 0xLL Byte) 
      //1-1. Receive  : 0xAA 0x55 0x01  |  0xCD 0xCD 0xCD 0xCD 0xCD | 0x01 0x00 0x00 0x00 0x00 0x00 0x00 0x00 | 0x00 ... 0x00

      //1-2. Transmit : 0xB0 0xLN 0xef   (LN : Sending count , ef : eof line bytes(len = LN*32 + ef)) 
      //N-1. Transmit : Data~ 
      //LN-1A.Receive : 0xAC(ACK) 0x00 0x00 0x00 0x00.... 0x00 (It's ACK.)-------------------->End.
      //LN-1B.Receive : 0xA0(NAK) 0xNN(Request count for present Transmission) 0x00.... 0x00-->Repeat N-1.
      
      //HAL_Delay(2);
      DebugRF("");
      nrf24_powerUpTx();
      memset(data_array,0,32);
      data_array[0] = 0xAA;
      data_array[1] = 0x55;
      data_array[2] = 0x03;//ResetBtn3 Request
      data_array[3] = 0x00;
      memcpy(data_array+3,rx_address,5);
      memcpy(data_array+8,DeviceCode,8);
      
      //HAL_Delay(50);
      DebugRF("TX          : %s",printBin(data_array,4));
      if(WZRF_Tx()){DebugRF("> Message is lost ...%d",__LINE__);Start=2;continue;}
      if(WZRF_Rx()){DebugRF("Timeout : --ms.%d",__LINE__);Start=2;continue;}//Recv 1-1.1
      DebugRF("TX 1-1 : %s",printBin(rx_address,5));
      DebugRF(printBin(DeviceCode,8));

      
      DebugRF("RX          : %s",printBin(data_array,4));

      //Recv waiting..
      int succeed = 0;
      uint32_t tim;
      tim = millis();
      while(1){
          WZMS_LEDMode(1);
          if(!WZRF_Rx())
          {
              succeed = 1;
              WZMS_LEDMode(5);
              break;//데이터를 받으면
          }
          if(millis()-tim > 60000)
          {
              WZMS_LEDMode(3);
              break;
          }
      }
      uint32_t time = micros();
      
      if(!succeed){DebugRF("Timeout2 : --ms.%d",__LINE__);Start=2;continue;}
      DebugRF("RX          : %s",printBin(data_array,4));
      if(data_array[0]==0xAA)
      {
          if(WZRF_Rx()){DebugRF("Timeout : --ms.%d",__LINE__);Start=2;continue;}
          DebugRF("RX 1-2 : %s",printBin(data_array,5));
      }
      if(data_array[0]!=0xB0){print("error");Start=2;continue;}

      DebugRF("Start~");
      int count = data_array[1], remain = data_array[2], i=0;
      RFLen = count<<5 | remain;
      DebugRF("count:%d,remain:%d,RFLen:%d",count,remain,RFLen);
      char *temp = (char*)malloc(RFLen+1);
      DebugRF("temp : %#010x",temp);
      memset(temp,0,RFLen+1);
      DebugRF("%d",__LINE__);
      while(i!=count)
      {
          data_array[0] = 0xAD;
          data_array[1] = 0xAC;
          data_array[2] = 0x00;
          data_array[3] = i;
          if(WZRF_Tx()){DebugRF("> Message is lost ...%d",__LINE__);Start=2;break;}
          DebugRF("TX N-1 : %s",printBin(data_array,4));
          if(WZRF_Rx()){DebugRF("Timeout : --ms.%d",__LINE__);Start=2;break;}//Recv N-1

          
          memcpy(jsondata+i*32,data_array,32);
          DebugRF("RX N-1 : Rx%2d temp 0x%04X-0x%04X",i,i*32,i*32+31);

          i++;
      }
      if(Start==2)continue;
      if(remain)
      {
          data_array[0] = 0xAD;
          data_array[1] = 0xAD;
          if(WZRF_Tx()){DebugRF("> Message is lost ...%d",__LINE__);Start=2;continue;}
          DebugRF("TX 1-1 : %s",printBin(data_array,4));

          //Recv N-1 last
          if(WZRF_Rx()){DebugRF("Timeout : --ms.%d",__LINE__);Start=2;continue;}
          memcpy(jsondata+i*32,data_array,remain);
          DebugRF("RX N-1L: Rx%2d temp 0x%04X-0x%04X",i,i*32,i*32+remain);
      }
      //HAL_Delay(10);
      //nrf24_powerUpTx();
      data_array[0]=0xAC;//Send Ack.
      if(WZRF_Tx()){DebugRF("> Message is lost ...%d",__LINE__);Start=2;continue;}
      time = micros()-time;
      DebugRF("TX LN-1: ACK");
      DebugRF("RFLen : %d, count : %d, remain : %d",RFLen,count,remain);
      // DebugRF("Data : %s",jsondata);
      PrintMemoryMap(0,jsondata, RFLen, 16);
      free(temp);

      int mill = time/1000;
      int micr = time - mill*1000;
      DebugRF("Sensing time : %d.%03dms",mill,micr);
      Start=3;
    }
  }
  println("[%8ld] RFres : %d",micros(),RFres);
  return RFres;
}


volatile void BtnINT()
{
  //printDetails();
  Start=1;
  //delay_ms(10);
  //printDetails();
  //println("INt end.");
}
/*
unsigned long millis()
{
  return HAL_GetTick();
}
*/
volatile uint32_t Tm=0;

int WZRF_Rx()
{
  yield();
  //if(nrf24_rxFifoEmpty());
  nrf24_powerUpRx();
  volatile uint32_t tmpTime = WZMS_millis();
  while(!nrf24_dataReady() && (WZMS_millis()-tmpTime) < 1000){ delay_ms(1); }//delay_ms(1);//RX 1-2
  if(WZMS_millis()-tmpTime >= 1000)return 1;
  
  nrf24_getData(data_array); 
  return 0;
}
int WZRF_Tx()
{
  yield();
  int i=0;
	while(1)
  {
    nrf24_send(data_array);
    while(nrf24_isSending());//println("sending...%d",__LINE__);
    
    if(nrf24_lastMessageStatus() && nrf24_retransmissionCount()==15)
    {
      //print(" R%X",i++);
      if(i>15)return 1;
    }
    temp = nrf24_retransmissionCount();
    break;
  }
  //println("> Retranmission count: %d",temp);
  return 0;
}

#ifdef __cplusplus
}
#endif
