#include <SPI.h>
#include "Addstr.h"
#include "SBMasterKey.h"
#include "RFInterface.h"
#include "nrf24.h"
#include "print.h"

void ReceivingData();
void delay_ms(int delay);
void ICACHE_RAM_ATTR RF_INTERRUPT();
void ICACHE_RAM_ATTR Sensor_Interrupt();
volatile int RecvINT;
volatile int TransmitINT = 0;

extern void RtnToSerial(String format,String rtnnum);
extern int Infobuflen;

extern "C" void RFInit();

void setup() {
  // put your setup code here, to run once:
  ESP.wdtDisable();
  // put your setup code here, to run once:
  Serial.begin(2000000);
  Serial.println("\nStart!!\n");
  //xfunc_out = senduart;

  //     PINMAP
  //GPIO0  : CE
  //GPIO4  : CSN
  //GPIO5  : IRQ
  //GPIO12 : MISO
  //GPIO13 : MOSI
  //GPIO14 : SCK
  //..
  println("0");
  pinMode(0, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, INPUT);
  pinMode(12, INPUT);
  pinMode(13, OUTPUT);
  pinMode(14, OUTPUT);
  println("1");
  SPI.begin();
  SPI.pins(14, 12, 13, 4);
  SPI.setDataMode(0);
  println("2");
  SPI.setFrequency(8000000);//8MHz

  pinMode(15, INPUT);
  attachInterrupt(15, Sensor_Interrupt, RISING);
  /* simple greeting message */

  RFInit();

  attachInterrupt( 5, RF_INTERRUPT, FALLING);

  //nrf24_ce_digitalWrite(0);
}

void senduart(uint8_t ch)
{
  Serial.print(ch);
}
void sendstr(char *str)
{
  Serial.print(str);
}
void sendstrln(char *str)
{
  yield();
  Serial.println(str);
}
//int count;
void loop() {
  //println("loop() start");
  ESP.wdtDisable();
  yield();

  if(TransmitINT)
  {
    ResetBtn();
    TransmitINT = 0;
  }

}
//NRF Protocol (Standard Mode : 4Byte)

// 0x02) Requesting BINARY file
//Inital        : Header 3Byte   + ComingAddr 5Byte      + SerialNum 14Byte + Extra dummy's  (Total 0xLL Byte)
//1-1. Receive  : 0xAA 0x55 0x02 | 0xCD 0xCD 0xCD 0xCD 0xCD | "wzkkh000000001" | 0x00... 0x00
//1-2A.TxIF     : 0xAA 0x55 0xNN      0xFF(Firm Version. It's no use Downloading the file. Its binary is new.)->END.
//1-2. Transmit Data  Sending binary file...
//Every 10 Transmission, it must return ACK. or error block to retry or break.
//N-1. Transmit :  -> 32Byte Transmision
//10N-1.Transmit: 0xAB 0xBA 0xAB 0xBA 0x01 0xNN.... 0x00 (is ACK?) (NN : he number of Packets in Next Sector)
//10N-1A.Receive: 0xAC(ACK) 0x00 0x00 0x00 0x00.... 0x00 (It's ACK.)------>Repeat N-1.
//10N-1B.Receive: 0xA0(NAK) 0xNN(Request count for present Transmission)
//10N-2.Transmit:  -> 32Byte reTransmision
//10N-3A.Receive: 0xA0(NAK) 0xNN(Request count for present Transmission)-->Repeat 10N-3.
//10N-3B.Receive  0xAC(ACK) 0x00 0x00 0x00 0x00.... 0x00 (It's ACK.)------>Repeat N-1.

//10N-B.Transmit: 0xAB 0xBA 0xAB 0xBA 0x04 0x00.... 0x00 (TheTransmission is over.)

//Receive 1-1
// 0x01) Requesting ResetBtn Interface
//Inital        : Header 3Byte    + ComingAddr 5Byte        + SerialNum 14Byte + Extra dummy's  (Total 0xLL Byte)
//1-1. Receive  : 0xAA 0x55 0x01  |  0xCD 0xCD 0xCD 0xCD 0xCD | "wzkkh000000001"   0x00 ... 0x00

//1-2. Transmit : 0xB0 0xLN 0xef   (LN : Sending count , ef : eof line bytes(len = LN*32 + ef))
//N-1. Transmit : Data~
//LN-1A.Receive : 0xAC(ACK) 0x00 0x00 0x00 0x00.... 0x00 (It's ACK.)-------------------->End.
//LN-1B.Receive : 0xA0(NAK) 0xNN(Request count for present Transmission) 0x00.... 0x00-->Repeat N-1.

void ICACHE_RAM_ATTR RF_INTERRUPT()
{
  RecvINT = 1;
}
void ICACHE_RAM_ATTR Sensor_Interrupt()
{
  TransmitINT = 1;
}


extern "C" void delay_us(uint64_t us) {
  uint64_t start = micros64();
  while (micros64() - start <= us)
  {
    yield();
  }
}

uint8_t spi_transfer(uint8_t tx)
{
  return SPI.transfer(tx);
}
void spi_transfers(uint8_t *tx, uint8_t *rx, int len)
{
  SPI.transferBytes(rx, tx, len);
}
void nrf24_ce_digitalWrite(int pin)
{
  digitalWrite(0, pin);
}
void nrf24_csn_digitalWrite(int pin)
{
  digitalWrite(4, pin);
}

void delay_ms(int delays)
{
  if(delays == -1)ESP.wdtDisable();
  else delay(delays);
}
