#include "RFInterface.h"
#include "Addstr.h"
#include "cJSON.h"
#include "nrf24.h"
#include "print.h"

#ifdef __cplusplus
extern "C"
{
#endif

uint8_t temp;
uint8_t q = 0;
extern int32_t start;
uint8_t data_array[32];
uint8_t WZRF_RxB[32];
uint8_t WZRF_TxB[32];
uint8_t tx_address[5] = {0x01, 0x00, 0x00, 0x00, 0x00};
uint8_t rx_address[5] = {0xD7, 0xD7, 0xD7, 0xD7, 0xD7};
extern char Array[2048];
extern int Infobuflen;
//uint8_t arraya[32];
volatile int RecvedisCmd = 0, RecvINT = 0;
char DeviceSNM[15];//14B + ""
extern int Infobuflen;
extern char InfoBuf[1024];


void RFInit()
{
    /* init hardware pins */
    nrf24_init();

    /* Channel #2 , payload length: 32 */
    nrf24_config(2, 32 );

    /* Set the device addresses */
    nrf24_rx_address(rx_address);
    nrf24_tx_address(tx_address);

    nrf24_powerUpRx();
    printDetails();
    print("\r\n> RX device ready\r\n");
}

extern void RtnToSerial(char* format,char* rtnnum);

void RFLoop()
{
if (RecvedisCmd == 2) {
    nrf24_powerUpRx();
    RecvedisCmd = 0;
    RtnToSerial("NDT","00");
  }
  if (RecvINT)
  {
    println("interrupt!!");
    if (nrf24_dataReady())RecvedisCmd = 1;
    RecvINT = 0;
  }

  if (nrf24_dataReady())RecvedisCmd = 1; //ReceivingData();

  if (RecvedisCmd)
  {
    // int aasi = 0;
    // for(aasi=0;aasi < Infobuflen>>4;aasi++)
    //   println("0x%04X : %s",aasi<<4,printBin(InfoBuf+(aasi<<4),16));
    // if(aasi<Infobuflen)
    //   println("0x%04X : %s",aasi<<4,printBin(InfoBuf+(aasi<<4),Infobuflen-(aasi<<4)));
    
    if (WZRF_Recvdata()) {
      RecvedisCmd = 2;
      return;
    }
    /*
    if (data_array[2] == 0x01) //if ResetBtnReq  ResetBtn Masterkey start!!
    {
      //Receive 1-1
      // 0x01) Requesting ResetBtn Interface
      //Inital        : Header 3Byte    + ComingAddr 5Byte        + SerialNum 14Byte + Extra dummy's  (Total 0xLL Byte)
      //1-1. Receive  : 0xAA 0x55 0x01  |  0xCD 0xCD 0xCD 0xCD 0xCD | "wzkkh000000001"   0x00 ... 0x00

      //1-2. Transmit : 0xB0 0xLN 0xef   (LN : Sending count , ef : eof line bytes(len = LN*32 + ef))
      //N-1. Transmit : Data~
      //LN-1A.Receive : 0xAC(ACK) 0x00 0x00 0x00 0x00.... 0x00 (It's ACK.)-------------------->End.
      //LN-1B.Receive : 0xA0(NAK) 0xNN(Request count for present Transmission) 0x00.... 0x00-->Repeat N-1.

      println("");

      //delay(10);
      //if(WZRF_Senddata()){println("Sending failed..%d",__LINE__);RecvedisCmd=2;return;}
      memcpy(tx_address, data_array + 3, 5);
      memcpy(DeviceSNM, data_array + 8, 15);
      nrf24_tx_address(tx_address);
      println("RX 1-1 : %02X %02X %02X %02X %02X %02s", tx_address[4], tx_address[3], tx_address[2], tx_address[1], tx_address[0], DeviceSNM);


      Process_getkey("", DeviceSNM);

      int len = strlen(InfoBuf), count = len >> 5, remain = len & 31, i = 0;

      //Transmit 1-2
      data_array[0] = 0xB0;//Transmit 1-2
      data_array[1] = count;//Sendingcount.LN.
      data_array[2] = remain;//remainebyte.ef.
      data_array[3] = 0x00;
      if (WZRF_Tx()) {
        println("Sending failed..%d", __LINE__);
        RecvedisCmd = 2;
        return;
      }
      println("TX 1-2 : %02X %02X %02X %02X", data_array[0], data_array[1], data_array[2], data_array[3]);


      while (i != count)
      {

        if (WZRF_Rx()) {
          println("Receiving timeout.%d", __LINE__);  //RX 1-2
          RecvedisCmd = 2;
          return;
        }
        println("TX Req : %#02X %#02X %#02X %#02X", data_array[0], data_array[1], data_array[2], data_array[3]);

        //Transmit N-1
        //delay(5);
        ESP.wdtDisable();
        memcpy(data_array, InfoBuf + i * 32, 32);
        if (WZRF_Tx()) {
          println("Sending failed..%d", __LINE__);
          RecvedisCmd = 2;
          return;
        }
        println("TX N-1 : Tx%2d InfoBuf 0x%04X-0x%04X", i, i * 32, i * 32 + remain);
        i++;
      }
      if (remain)
      {
        if (WZRF_Rx()) {
          println("Receiving timeout.%d", __LINE__);  //RX 1-2
          RecvedisCmd = 2;
          return;
        }
        println("TX Req : %#02X %#02X %#02X %#02X", data_array[0], data_array[1], data_array[2], data_array[3]);

        //Transmit N-1 last
        //delay(5);
        memcpy(data_array, InfoBuf + i * 32, remain);
        if (WZRF_Tx()) {
          println("Sending failed..%d", __LINE__);
          RecvedisCmd = 2;
          return;
        }
        println("TX N-1L: Tx%2d InfoBuf 0x%04X-0x%04X", i, i * 32, i * 32 + remain);
      }
      int req = 0;
      while (!req)
      {

        if (WZRF_Rx()) {
          println("Receiving timeout.%d", __LINE__);  //RX 1-2
          RecvedisCmd = 2;
          return;
        }
        println("LN-1 : Rx %02X %02X %02X", data_array[0], data_array[1], data_array[2]);

        if (data_array[0] == 0xAC) //ACK
          req = 1;
        else if (data_array[0] == 0xA0) //NAK
        {
          req = 0;
          i = data_array[1];
          //delay(5);
          if (i != count)
          {
            memcpy(data_array, InfoBuf + i * 32, 32);
            if (WZRF_Tx()) {
              println("Sending failed..%d", __LINE__);
              RecvedisCmd = 2;
              return;
            }
            println("ReTXN-1: Tx%d InfoBuf 0x%04X-0x%04X", i, i * 32, i * 32 + 31);
          } else {
            memcpy(data_array, InfoBuf + i * 32, remain);
            if (WZRF_Tx()) {
              println("Sending failed..%d", __LINE__);
              RecvedisCmd = 2;
              return;
            }
            println("ReTXN-1: Tx%d InfoBuf 0x%04X-0x%04X", i, i * 32, i * 32 + remain);
          }
          //if(WZRF_Sendwait()){println("Sending failed..%d",__LINE__);RecvedisCmd=2;return;}
        }
      }
      println("Transmission was Successful!! End.");

      nrf24_powerUpRx();

    } 
    else */
    if (data_array[2] == 0x03) //if ResetBtnReq  ResetBtn3 Masterkey start!!
    {
      //Receive 1-1
      // 0x01) Requesting ResetBtn3 Interface
      //Inital        : Header 3Byte    + ComingAddr 5Byte        + SerialNum 14Byte + Extra dummy's  (Total 0xLL Byte)
      //1-1. Receive  : 0xAA 0x55 0x01  |  0xCD 0xCD 0xCD 0xCD 0xCD | "wzkkh000000001"   0x00 ... 0x00

      //1-2. Transmit : 0xB0 0xLN 0xef   (LN : Sending count , ef : eof line bytes(len = LN*32 + ef))
      //N-1. Transmit : Data~
      //LN-1A.Receive : 0xAC(ACK) 0x00 0x00 0x00 0x00.... 0x00 (It's ACK.)-------------------->End.
      //LN-1B.Receive : 0xA0(NAK) 0xNN(Request count for present Transmission) 0x00.... 0x00-->Repeat N-1.

      println("");

      //delay(10);
      //if(WZRF_Senddata()){println("Sending failed..%d",__LINE__);RecvedisCmd=2;return;}
      memcpy(tx_address, data_array + 3, 5);
      //memcpy(DevEUI, data_array + 8, 8);
      nrf24_tx_address(tx_address);
      //println("RX 1-1 : %s "   , printBin(data_array,32));
      //print  ("tx_addr : %s"   , printBin(tx_address,5));
      //println("   DEVEUI :  %s", printBin(data_array + 8,8));

      delay_ms(100);
      //TX 1-1.1
      data_array[0] = 0xBf;//Transmit 1-1.1
      data_array[1] = 0xFF;
      data_array[2] = 0xDE;
      data_array[3] = 0xAD;
      if (WZRF_Tx()) {
        println("Acknowledge failed..%d", __LINE__);
        RecvedisCmd = 2;
        return;
      }

      yield();
      println("TX 1-2 : %02X %02X %02X %02X", data_array[0], data_array[1], data_array[2], data_array[3]);

      DEV2snm(data_array + 8,DeviceSNM);
      //strcpy(DeviceSNM, "wzkkb000000001");
      println("SNM : %s",DeviceSNM);

      
      Process_Access(DeviceSNM);
      Process_getkey();
      if(Infobuflen==1){ println("Inerface Cancelled.");RecvedisCmd = 2;return;}
      if(Infobuflen==0){ println("Infobuflen is Zero.");RecvedisCmd = 2;return;}
      //sprintf(InfoBuf,"Your _snm = %s",DeviceSNM);//Buffer to transfer.
      
      println("now sending..");

      int len = Infobuflen, count = len >> 5, remain = len & 31, i = 0;

      //Transmit 1-2
      data_array[0] = 0xB0;//Transmit 1-2
      data_array[1] = count;//Sendingcount.LN.
      data_array[2] = remain;//remainebyte.ef.
      data_array[3] = 0x00;
      if (WZRF_Tx()) {
        println("Sending failed..%d", __LINE__);
        RecvedisCmd = 2;
        return;
      }
      println("TX 1-2 : %02X %02X %02X %02X", data_array[0], data_array[1], data_array[2], data_array[3]);


      while (i != count)
      {

        if (WZRF_Rx()) {
          println("Receiving timeout.%d", __LINE__);  //RX 1-2
          RecvedisCmd = 2;
          return;
        }
        println("TX Req : %#02X %#02X %#02X %#02X", data_array[0], data_array[1], data_array[2], data_array[3]);

        //Transmit N-1
        //delay(5);
        //ESP.wdtDisable();
        delay_ms(100);
        memcpy(data_array, InfoBuf+ i * 32, 32);
        //println("i=%d;InfoBuf[%d] = %s",i,i * 32,printBin(InfoBuf+ i * 32,32));
        if (WZRF_Tx()) {
          println("Sending failed..%d", __LINE__);
          RecvedisCmd = 2;
          return;
        }
        //println("data_array = %s",printBin(data_array,32));
        println("TX N-1 : Tx%2d InfoBuf 0x%04X-0x%04X", i, i * 32, i * 32 + remain);
        i++;
      }
      if (remain)
      {
        if (WZRF_Rx()) {
          println("Receiving timeout.%d", __LINE__);  //RX 1-2
          RecvedisCmd = 2;
          return;
        }
        println("TX Req : %#02X %#02X %#02X %#02X", data_array[0], data_array[1], data_array[2], data_array[3]);

        //Transmit N-1 last
        //delay(5);
        delay_ms(100);
        memcpy(data_array, &InfoBuf[i * 32], remain);
        if (WZRF_Tx()) {
          println("Sending failed..%d", __LINE__);
          RecvedisCmd = 2;
          return;
        }
        println("TX N-1L: Tx%2d InfoBuf 0x%04X-0x%04X", i, i * 32, i * 32 + remain);
      }
      int req = 0;
      while (!req)
      {

        if (WZRF_Rx()) {
          println("Receiving timeout.%d", __LINE__);  //RX 1-2
          RecvedisCmd = 2;
          return;
        }
        println("LN-1 : Rx %02X %02X %02X", data_array[0], data_array[1], data_array[2]);

        if (data_array[0] == 0xAC) //ACK
          req = 1;
        else if (data_array[0] == 0xA0) //NAK
        {
          req = 0;
          i = data_array[1];
          //delay(5);
          if (i != count)
          {
            memcpy(data_array, InfoBuf + i * 32, 32);
            delay_ms(100);
            if (WZRF_Tx()) {
              println("Sending failed..%d", __LINE__);
              RecvedisCmd = 2;
              return;
            }
            println("ReTXN-1: Tx%d InfoBuf 0x%04X-0x%04X", i, i * 32, i * 32 + 31);
          } else {
            memcpy(data_array, InfoBuf + i * 32, remain);
            delay_ms(100);
            if (WZRF_Tx()) {
              println("Sending failed..%d", __LINE__);
              RecvedisCmd = 2;
              return;
            }
            println("ReTXN-1: Tx%d InfoBuf 0x%04X-0x%04X", i, i * 32, i * 32 + remain);
          }
          //if(WZRF_Sendwait()){println("Sending failed..%d",__LINE__);RecvedisCmd=2;return;}
        }
      }
      println("Transmission was Successful!! End.");

      RtnToSerial("NDT","01");

      nrf24_powerUpRx();

    }
    /*
    else if (data_array[2] == 0x02) //if BinfileReq BINARY file
    {
      //NRF Protocol (Standard Mode : 4Byte)

      // 0x02) Requesting BINARY file
      //Inital        : Header 3Byte   + ComingAddr 5Byte      + SerialNum 14Byte + Extra dummy's  (Total 0xLL Byte)
      //1-1. Receive  : 0xAA 0x55 0x02 | 0xCD 0xCD 0xCD 0xCD 0xCD | "wzkkh000000001" | 0x00... 0x00
      //1-2A.TxIF     : 0xAA 0x55 0xNN      0xFF(Firm Version. It's no use Downloading the file. Its binary is new.)->END.
      //1-2. Transmit Data  Sending binary file...
      //Every 10 Transmission, it must return ACK. or error block to retry or break.
      //N-1. Transmit :  -> 32Byte Transmision
      //10N-1.Transmit: 0xAB 0xNN 0xBB 0xAA 0xAA 0xBB .... 0x00 (is ACK?) (NN : he number of Packets in Next Sector)
      //10N-1A.Receive: 0xAC(ACK) 0x00 0x00 0x00 0x00 .... 0x00 (It's ACK.)------>Repeat N-1.
      //10N-1B.Receive: 0xA0(NAK) 0xNN                .... (Request count for present Transmission)
      //10N-2.Transmit:  -> 32Byte reTransmision
      //10N-3A.Receive: 0xA0(NAK) 0xNN                .... (Request count for present Transmission)-->Repeat 10N-3.
      //10N-3B.Receive  0xAC(ACK) 0x00 0x00 0x00 0x00 .... 0x00 (It's ACK.)------>Repeat N-1.

      //10N-B.Transmit: 0xAB 0x04 0x00                .... 0x00 (TheTransmission is over.)


      println("BinfileReq BINARY file ");

      //delay(10);
      if (WZRF_Tx()) {
        println("Sending failed..%d", __LINE__);
        RecvedisCmd = 2;
        return;
      }

      uint8_t DeviceFirm = data_array[23];
      memcpy(tx_address, data_array + 3, 5);
      memcpy(DeviceSNM, data_array + 8, 15);
      nrf24_tx_address(tx_address);
      println("RX 1-1 : %02X %02X %02X %02X %02X %02s %02X", tx_address[4], tx_address[3], tx_address[2], tx_address[1], tx_address[0], DeviceSNM, DeviceFirm);

      File f = SPIFFS.open(LWTFile, "r");
      if (!f)
      {
        Serial.println((String)LWTFile + " open failed");
        f.close();
        RecvedisCmd = 2; return;
      }
      //file
      //binary size
      int NSectorPack = 10;
      int len = f.size(), count = len >> 5, remain = len & 31, i = 0, NremainPack = count % NSectorPack, j, k;
      uint8_t BinVersions = 0x02;
      println("len:%d count:%d  remain:%d  NremainPack:%d", len, count, remain, NremainPack);

      //DeviceFirm;
      //Transmit 1-2
      data_array[0] = 0xAA;
      data_array[1] = 0x55;//
      data_array[2] = (NSectorPack != 0) ? NSectorPack : NremainPack; //NN
      data_array[3] = (BinVersions >= DeviceFirm) ? BinVersions : 0xFF; //Version.
      //delay(20);
      //Transmit 1-2
      if (WZRF_Tx()) {
        println("Sending failed..%d", __LINE__);
        RecvedisCmd = 2;
        return;
      }
      println("TX 1-2 : %02X %02X %02X %02X", data_array[0], data_array[1], data_array[2], data_array[3]);
      if (BinVersions < DeviceFirm) //if we don't have to update software, just cancel it.
      {
        f.close();
        RecvedisCmd = 2; return;
      }
      uint8_t *SectorData = (uint8_t*)malloc(32 * NSectorPack);
      //if(WZRF_Tx()){println("Sending failed..%d",__LINE__);RecvedisCmd=2;return;}

      int req = 0;
      for (i = 0; i <= count / NSectorPack; i++)
      {
        memset(SectorData, 0, 32 * NSectorPack);
        int res = f.read(SectorData, 32 * NSectorPack);
        if (res != 32 * NSectorPack)println("fileread error. %d!=%d", res, 32 * NSectorPack);
        for (int j = 0; j < NSectorPack; j++)
        {
          //Transmit N-1
          //delay(5);
          if (WZRF_Rx()) {
            println("Receiving timeout.%d", __LINE__);  //RX 1-2
            RecvedisCmd = 2;
            return;
          }
          println("TX Req : %#02X %#02X %#02X %#02X", data_array[0], data_array[1], data_array[2], data_array[3]);

          memcpy(data_array, SectorData + j * 32, 32);
          if (WZRF_Tx()) {
            println("Sending failed..%d", __LINE__);
            RecvedisCmd = 2;
            return;
          }
          println("TX %3d-1 : Tx%2d InfoBuf 0x%04X-0x%04X", i * NSectorPack + j, j, j * 32, j * 32 + 31);
        }//
        if (WZRF_Rx()) {
          println("Receiving timeout.%d", __LINE__);  //RX 1-2
          RecvedisCmd = 2;
          return;
        }
        println("TX Req : %#02X %#02X %#02X %#02X", data_array[0], data_array[1], data_array[2], data_array[3]);

        data_array[0] = 0xAB;
        data_array[1] = 32;
        data_array[2] = 0xBB;
        data_array[3] = 0xAA;
        data_array[4] = 0xAA;
        data_array[5] = 0xBB;
        println("TX %3d-1 : %02X %02X %02X %02X %02X %02X", (i + 1)*NSectorPack, data_array[0], data_array[1], data_array[2], data_array[3], data_array[4], data_array[5]);
        if (WZRF_Tx()) {
          println("Sending failed..%d", __LINE__);
          RecvedisCmd = 2;
          return;
        }

        while (!req)
        {
          if (WZRF_Rx()) {
            println("Receiving timeout.%d", __LINE__);  //RX 1-2
            RecvedisCmd = 2;
            return;
          }
          println("%3d-1 : Rx %02X %02X %02X", (i + 1)*NSectorPack, data_array[0], data_array[1], data_array[2]);

          if (data_array[0] == 0xAC) //ACK
            req = 1;
          else if (data_array[0] == 0xA0) //NAK
          {
            req = 0;
            k = data_array[1];
            memcpy(data_array, SectorData + k * 32, 32);
            if (WZRF_Tx()) {
              println("Sending failed..%d", __LINE__);
              RecvedisCmd = 2;
              return;
            }
            println("%3d-2 : Tx%d NSectorPack 0x%04X-0x%04X", (i + 1)*NSectorPack, k, k * 32, k * 32 + 31);
          }
        }
      }
      ;
      data_array[0] = 0xAB;
      data_array[1] = 0x04;
      data_array[2] = 0x00;
      if (WZRF_Tx()) {
        println("Sending failed..%d", __LINE__);
        RecvedisCmd = 2;
        return;
      }
      println("TX %3d-B : %02X %02X %02X", NSectorPack * i, data_array[0], data_array[1], data_array[2]);


      println("Transmission was Successful!! End.");
      f.close();
      free(SectorData);

    }
    */
  }
}
//It need to be called after REQ interrupt or detection RX_DR of RF's status. (error : 1)
int WZRF_Recvdata()
{
  nrf24_getData(data_array);
  //WZRF_dataRx();// Update data_array;
  //WZRF_flushRx();
  //if(WZRF_RxB[0]==0xAC){println("NRF Seq Error.");return 1;}
  return 0;
}
int WZRF_Rx()
{
  nrf24_powerUpRx();
  uint32_t tmpTime = millis();
  while (!nrf24_dataReady() && millis() - tmpTime < 1000)delay_ms(1); //RX 1-2
  if (millis() - tmpTime >= 1000)return 1;

  nrf24_getData(data_array);
  return 0;
}
int WZRF_Tx()
{
  //println("WZRF_Tx() start");
  int i = 0;
  while (1)
  {
    uint32_t Tm = millis();
    //println("now send");
    nrf24_send(data_array);
    while (nrf24_isSending() && millis() - Tm < 1000)
    {
      //println("%d",millis() - Tm);
      delay_ms(1);
      yield();
    }
    if (millis() - Tm >= 1000) {
      println("Transmit Timeout. %dms", millis() - Tm);
      return 1;
    }

     if (nrf24_lastMessageStatus() && nrf24_retransmissionCount() == 15)
     {
       //print(" T%X",i++);
       if (i > 15)return 2;
     }
     temp = nrf24_retransmissionCount();
     break;
  } 
  return 0;
}

#ifdef __cplusplus
}
#endif
