#include "SBMasterKey.h"
#include "Addstr.h"
#include "print.h"
#include "cJSON.h"

char Array[2048];
char InfoBuf[1024];
int Infobuflen = 0;
uint32_t start;

#define DBG_OUTPUT_PORT Serial

int cnt = 0, no = 0;
int Received=1,count=0;
char *RecvDat;
char chBuf[2048];
int Recording;
int focus=0;
char Bitmap[968];

String Name = "";

uint32_t ProductNumber = 0;

uint32_t UARTTime = millis();

bool ReceiveEnable = false;
char USBch = 0;
char USBStr[1024] = {0,};
String USBArray = "";
char AppNum[13] = "sbkk00000000";
char* SILFile = "/SIL.json";

char* format_What[13] = {"bt","tp","ox","du","co","hu","ni","pp","hd","uv","ps","cm","mt"};

int num_isEqualText;
extern "C" int isEqualText(char* a, char* b) {
	if (strlen(a) != (num_isEqualText = strlen(b)))return false;
	if (memcmp(a, b, num_isEqualText)) return false;
		return true;
}
extern "C" int iscontainedText(char* a, char* b) {
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
void WifiInit(){
  Serial.print((String)(char)2 + "EXA" + (String)(char)5 + "CMD" + (String)(char)0x1f + "DAT" + (String)(char)3 + (String)'\n');
  
	start = millis();
  Process_getkey_preset();
	println("WifiInit() end");
  
}
void RtnSerial(String Format,String format,String rtnnum){
  Serial.print((String)(char)2 + Format + (String)(char)5 + format + (String)(char)0x1f + rtnnum + (String)(char)3);
}
void RtnToSerial(String format,String rtnnum){
  RtnSerial("RTN",format,rtnnum);
}
extern "C" void RtnToSerial(char* format,char* rtnnum){
  RtnSerial("RTN",format,rtnnum);
}

void ALV(String data){
  //DynamicJsonDocument tempJson(30);
  //DeserializationError error = deserializeJson(tempJson, data);
  //if(error)RtnToSerial("ALV","00");
  //else {
  
  //const char* contents = tempJson[(char*)"masterkey"];
  //Serial.println(data.substring(0,9) + "," + (String)(int)data[9] + "," + data.substring(10,12));
  if(data.substring(0,9) == "masterkey"){
    if((int)(data[9])==31){  //"01"
      if(data.substring(10,12) == "01")
        RtnToSerial("ALV","01");
      else RtnToSerial("ALV","00");
    }else RtnToSerial("ALV","00");
  }else RtnToSerial("ALV","00");
  
  //}
}
int Main_1ms = 0;
void MSKeyloop() {
  // put your main code here, to run repeatedly:
  yield();

}


String str_SIL;
// 센서가 가진 고유한 값
// Serialname : 예)0000000001
// DevKEY : 8바이트 길이의 코드               
// 
// 센서가 받아야 할 Datas
// AppNum     : 예)sbkk00010003
// Int_SenseTime : 데이터 보내는 주기(ms)     (16 ~ 19)
// Val_Adjust[5] : 각 센서에 대한 보정값      
// 
// 보낼 데이터 개수 : tp 1개-> 0x01           (20)
// 보낼 데이터 종류 (최대 5개) : tp ox ->          0x00      0x02      0xFF      0xFF     0xFF  (29 ~ 33)
// Val_Adjust[0-4] : 각 센서에 대한 보정값      (21 ~ 39)
// 
// 각 센서 종류에 대해서 {"b" : {"tp" : "12.32"}, "h" : {"tp" : "12.32" , "ox" : "0.23"}, "o" : {"tp" : "12.32" , "hu" : "0.23"},}
// 


// 
typedef struct
{
  uint8_t* RACString;
  uint8_t  RACStrlen;          // RAC정보  -> (str) N+1 바이트 텍스트 ( 길이(1바이트) + Text... )
  uint8_t  AdjustVal[25];      // 보정값   -> (dat) 25바이트 데이터
  uint32_t NowTime;            // 현재시간 -> (int) 8 바이트 1970년으로부터 초
  uint32_t SendTime;           // 전송주기 -> (int) 4 바이트(분) 
  uint32_t DangerTime;         // 위험기간 -> (int) 4 바이트(일) 
  uint32_t SavingTime;         // 보관기간 -> (int) 4 바이트(일)
  uint8_t  startDate[4] ;      // 시작시간 -> (dat) 4 바이트 ( YY YY MM DD )
} DatafromPC;
typedef struct 
{
  DatafromPC Data;
  uint8_t  ProductBitmap[968]; // 상품명   -> (dat) 968바이트 한글 비트맵
  uint8_t  APPKEY[16];         // APPKEY   -> (dat) 16 바이트
}SendDat;
SendDat Sendata;

char* LWTpos;
void Process_getkey_preset()
{
  //SearchSN = Name;LWTFind = false;
  int Number = Name.toInt();

  str_SIL="";
/*
  println("%s file open. %d", SILFile, SPIFFS.exists(SILFile));
  File f = SPIFFS.open(SILFile,"r");
  if(!f)
  {
    println("failed to open the file %s", SILFile);
  }
  else
  {
    println("size : %d", f.size());
    for(int i=0;i<f.size();i++)
      str_SIL += (char)f.read();
  }
  println("%s file : %s.",SILFile,str_SIL.c_str());
  f.close();
  */
  
  Apn2EUI((char*)&AppNum[0],&Sendata.APPKEY[0]);
  //println("APPKEY : %s",printBin(APPKey,16));

  //memcpy(Sendbuf,APPKey,16);

  //   ..................................................     //
  //   ..................................................     //
  //   ..................................................     //
  //   ..................................................     //
  
  //Infobuflen = sizeof(Sendata);
  //memcpy(InfoBuf,&Sendata,sizeof(Sendata));
  /*
  for(int i=0;i < (Infobuflen>>4);i++)
  {
    println("0x%04X : %s",i<<4,printBin(InfoBuf+i,16));
  }
  */
  
  //println("Process_getkey_preset end");
}
char* name_Node;
uint8_t SenseAwait;
//Send Message to PC.
extern "C" void Process_Access(char* name_cstr) {
  RtnSerial("NDT",name_cstr,"01");
  name_Node = name_cstr;
}
extern "C" void Process_getkey() {
  SenseAwait = 1;
  uint32_t now = millis();
  while(SenseAwait)
  {
    ESP.wdtDisable();
		Port_uartprocessing();
    yield();
    //println("%d",Infobuflen);
    if(millis()-now > 60000)break;
  }
  if(SenseAwait) { Infobuflen=0; return; }

  /*
  println("name_Node : %d",name_Node);
  cJSON *json_SIL = cJSON_Parse(str_SIL.c_str());
  cJSON *json_Sensor = cJSON_GetObjectItem(json_SIL,name_Node);
  if(json_Sensor == 0)
  {
    println("failed to look for %s .",name_Node);
    for(int i=0;i<5;i++)
        InfoBuf[Infobuflen+i] = 0xFF;
  }
  else 
  {
    println("json_Sensor : %s",cJSON_Print(json_Sensor));
    int len = cJSON_GetArraySize(json_Sensor);
    println("len : %d",len);
    cJSON *tmp;
    for(int i=0;i<5;i++)
    {
      if(i<len)
      {
        tmp = cJSON_GetArrayItem(json_Sensor,i);
        if(!tmp)println("json failed.334. %s",cJSON_GetErrorPtr());
        else 
        {
          char* sType = cJSON_GetObjectItemCaseSensitive(tmp,"sType")->valuestring;
          float val = atof(cJSON_GetObjectItemCaseSensitive(tmp,"val")->valuestring);
          int type = format_a(sType);
          InfoBuf[Infobuflen+i] = type;
          memcpy(InfoBuf+(Infobuflen+5+i*4),&val,sizeof(float));
          println("sType : %s , val : %f",sType,val);
        }
      }
      else
      {
        InfoBuf[Infobuflen+i] = 0xFF;
      }
    }
    ;
  }
  cJSON_Delete(json_SIL);
  println("InfobuflenB : %d",Infobuflen);
  Infobuflen += 5+5*4;
  */
  println("InfobuflenA : %d",Infobuflen);
  /*
  for(int i=0;i<Infobuflen;i++)
  {
    print(" %02X",InfoBuf[i]);
    if(i%16 == 15)println("");
  }
  */
  println("");
}

void AKY(char* data);
void PBP(char* data);
void NDT(char* data);
void Receive_e();

void MasterKeySerial()
{
  //detachInterrupt(2);
  //Serial.println("millis() - UARTTime=" + (String)(int)Time);
  //COMFilefile.close();
  // Serial.println("FreeHeap = " + String(ESP.getFreeHeap()));
  //FileShow(COMFile);
  //COMFilefile = SPIFFS.open(COMFile, "r+");
  int Available = strlen(RecvDat);//COMFilefile.size();
  char* Buf = (char*)malloc(Available+1);
  memcpy(Buf,RecvDat,Available);
  Buf[Available] = 0;
   Serial.println("\nSerial.size()=" + (String)Available);
  USBch = RecvDat[0];
   Serial.println("USBch = " + (String)(int)USBch);

  if(RecvDat[0] == 2 && RecvDat[1] == 'e' &&  RecvDat[2] == 3){
    Receive_e();
    return;
  }
  
  if(USBch == 2){
    char *ch5=strchr(RecvDat,5),*ch3=strchr(RecvDat,3);
    if(!ch5 || !ch3)return;
    *ch3 = 0;*ch5 = 0; 
    String cmd = RecvDat+1;//COMFilefile.readStringUntil(5);
    String data = ch5+1;//COMFilefile.readStringUntil(3);
    //COMFilefile.close();
    Serial.println("cmd =" + cmd);
    //Serial.println("data=" + data);
    if(cmd == "ALV"){
      ALV(data);
    }
    else if(cmd == "AKY"){
      AKY((char*)data.c_str());
    }
    else if(cmd == "PBP"){
      PBP((char*)data.c_str());
    }
    else if(cmd == "NDT"){
      NDT((char*)data.c_str());
    }
    else if(cmd == "RTN"){
      if(data.substring(0,2) == "ALV"){
        if(data.substring(data.indexOf(0x1F)+1) == "00"){
          ALV(data);
        }else if(data.substring(data.indexOf(0x1F)+1) == "01"){
          
        }
      }
    }
  }
  Process_getkey_preset();
  ReceiveEnable = false;
}
void Receive_e() //STX + e + ETX
{
  Serial.print((String)(char)2 + (String)(char)6 + (String)(char)3);
}
void AKY(char* _data) // "sbkk00010003" + US(1F) + "01" + ETX
{
  strcpy((char*)Sendata.APPKEY,_data);
  RtnToSerial("AKV","01");
}
void PBP(char* _data) // (968 Byte bitmap Data) +US(1F)+01+ETX
{
  memcpy(Sendata.ProductBitmap,_data,968);
  RtnToSerial("PBP","01");
}
uint8_t hex2int(char ch){
  if('0' <= ch && ch <= '9')return ch-'0';
  if('A' <= ch && ch <= 'F')return ch-'A'+10;
  if('a' <= ch && ch <= 'f')return ch-'A'+10;
  return 0;
}
void NDT(char* _data) // LENGTH(4Byte) +  HEX Data(LEN*2 Byte) +US(1F)+01+ETX
{
  DatafromPC *PCData;
  int len = hex2int(_data[0])<<12 | hex2int(_data[1])<<8 |hex2int(_data[2])<<4 | hex2int(_data[3]);

  println("len            : %d", len);
  println("_data          : %s", _data);
  
  if(len <= 0 || len > 1024)return;

  for(int i=0;i<len;i++)
  {
    InfoBuf[i] = hex2int(_data[2*i + 4])<<4 | hex2int(_data[2*i+5]);
  }
  SenseAwait = 0;
  
  Infobuflen = len;
  //memcpy(InfoBuf,bufs,Infobuflen);
  
  //PCData = (DatafromPC*)bufs;
  //free(bufs);
  /*
  Sendata.Data = *PCData;

  Sendata.Data.RACStrlen = strlen((char*)&PCData->RACString);
  Sendata.Data.RACString = (uint8_t*)&PCData->RACString;
  
  */
  println("Received data from Masterkley App (%d)",Infobuflen);
  int i;
  for(i=0;i < Infobuflen>>4;i++)
    println("0x%04X : %s",i<<4,printBin(InfoBuf+(i<<4),16));
  if(i<Infobuflen)
    println("0x%04X : %s",i<<4,printBin(InfoBuf+(i<<4),Infobuflen-(i<<4)));
  /*
  println("startDate      : %s", printBin(Sendata.Data.startDate,4));
  println("SavingTime     : %d", Sendata.Data.SavingTime);
  println("DangerTime     : %d", Sendata.Data.DangerTime);
  println("SendTime       : %d", Sendata.Data.SendTime);
  println("NowTime        : %d", Sendata.Data.NowTime);
  println("AdjustVal      : %s", printBin(Sendata.Data.AdjustVal,25));
  println("RACStrlen      : %d", Sendata.Data.RACStrlen);
  if(Sendata.Data.RACString)println("RACString      : %s",Sendata.Data.RACString);
  // uint32_t ;            // 현재시간 -> (int) 8 바이트 1970년으로부터 초
  // uint32_t ;           // 전송주기 -> (int) 4 바이트(분) 
  // uint32_t ;         // 위험기간 -> (int) 4 바이트(일) 
  // uint32_t ;         // 보관기간 -> (int) 4 바이트(일)
  // uint8_t  [4] ;      // 시작시간 -> (dat) 4 바이트 ( YY YY MM DD )
  */
}

void Port_uartprocessing() {
  //println("Port_uartprocessing() start");
	while(Serial.available()){
		char ch = Serial.read();
		//Serial.println((String)"avail : " + Serial.available() + "(" + ch + ")");
		if(Recording){
			if(ch == 3 || focus >= 2047){
        Serial.println((String)"Recording was ended");
        chBuf[focus++] = ch;
        chBuf[focus] = 0;
				Recording = 0;
        RecvDat = chBuf;//Addstr_return(&Recv);
				//Serial.println((String)"Buf : " + RecvDat);
				Serial.println((String)"count : " + count);
        MasterKeySerial();
				count=0;
			}
			else 
      {
        //Addstr_addlen(&Recv,&ch,1);
        chBuf[focus++] = ch;
      }
		}else if(ch==2){
      //Serial.println((String)"Started to Record..");
			Recording=1;
			focus=0;
      chBuf[focus++] = ch;
		}
    if(ch==0xEF){
      println("Received data from Masterkley App (%d)",Infobuflen);
      int i;
      for(i=0;i < Infobuflen>>4;i++)
        println("0x%04X : %s",i<<4,printBin(InfoBuf+(i<<4),16));
      if(i<Infobuflen)
        println("0x%04X : %s",i<<4,printBin(InfoBuf+(i<<4),Infobuflen-(i<<4)));
		}
	}
}

/*
//"wzkk 0000 0001"
void apn2AppEUI(char* apn)
{
  union{
    struct {
      char Project[2];
      char country[2];
      char idxCli[4];
      char idxAP[4];
      char _null;
    }sd;
    char apn[13];
  }Temp;
  int iAP,iCli;
  char buf[5] = {0,};
  memcpy(buf,Temp.sd.idxCli,sizeof(Temp.sd.idxCli));
  iCli = atoi(buf);
  memcpy(buf,Temp.sd.idxAP,sizeof(Temp.sd.idxAP));
  iAP = atoi(buf);

  memcpy(Temp.apn,apn,13);
  println("snm        : %s",Temp.apn);
  println("project    : %c%c",Temp.sd.Project[0],Temp.sd.Project[1]);
  println("country    : %c%c",Temp.sd.country[0],Temp.sd.country[1]);
  println("idxCli     : %d",iCli);
  println("idxAP      : %d",iAP);
  println("_null      : %d",Temp.sd._null);
  
  char buf1[16] = { 0xA7, 0x7B, 0x8B, 0xAB, 0x23, 0x6c, 0x00,};
  memset(buf,0,sizeof(buf));
  memcpy(buf,Temp.sd.country,2);

  buf1[0] = 0xFF;
  for(int i=0;i<sizeof(countries)/sizeof(char*);i++)
    if(isEqualText(countries[i],buf))buf1[0]=i;
  
  buf1[1] = 0xFF;
  for(int i=0;i<sizeof(sensorTypes)/sizeof(char);i++)
    if(Temp.sd.sensorType == sensorTypes[i])buf1[1] = i;

  memcpy(rx_address,buf1,2);

  uint32_t number = atoi(Temp.sd.number);
  for(int i=7;i>4;i--){buf1[i] = (number)&0xFF;number>>=8;}
  
  memcpy(rx_address+2,buf1+5,3);
  // buf1[8] = (number)&0xFF;number>>=8;
  // buf1[7] = (number)&0xFF;number>>=8;...

  print("privious va:");for(int i=0;i<8;i++)print(" %02X", DEVEUI[i]);
  println("");
  print("buf1       :");for(int i=0;i<8;i++)print(" %02X", buf1[i]);
  for(int i=0;i<8;i++)DEVEUI[7-i] = buf1[i];
  println("");
  print("DEVEUI     :");for(int i=0;i<8;i++)print(" %02X", DEVEUI[i]);
  println("");
*/
char *Projects[1]   = {"sb"};
char *countries[1]  = {"kk"};
char sensorTypes[3] = {'b','h','o'};
extern "C" void DEV2snm(uint8_t *DEVEUI,char * SNM)
{
  println("DeviceKey : %s",printBin(DEVEUI,8));
  
  memcpy(SNM+0,Projects[0],2);
  memcpy(SNM+2,countries[DEVEUI[7]],2);
  memcpy(SNM+4,&sensorTypes[DEVEUI[6]],1);
  int SenseNum;
  memcpy(&SenseNum,DEVEUI,4);
  sprintf(SNM+5,"%09d",SenseNum);
  SNM[15]=0;
  
  //sprintf(SNM,"%15d",SNM);
  println("DeviceSNM : %s",SNM);
}
extern "C"  void Apn2EUI(char *APnum,uint8_t * APPKEY)
{
  union{
    struct {
      char Project[2];
      char country[2];
      char idxCli[4];
      char idxAP[4];
      char _null;
    }sd;
    char apn[13];
  }Temp;
  int iAP,iCli;
  char buf[5] = {0,};

  memcpy(Temp.apn,APnum,13);

  memcpy(buf,Temp.sd.idxCli,sizeof(Temp.sd.idxCli));
  iCli = atoi(buf);
  memcpy(buf,Temp.sd.idxAP,sizeof(Temp.sd.idxAP));
  iAP = atoi(buf);

  // println("snm        : %s",Temp.apn);
  // println("project    : %c%c",Temp.sd.Project[0],Temp.sd.Project[1]);
  // println("country    : %c%c",Temp.sd.country[0],Temp.sd.country[1]);
  // println("idxCli     : %d",iCli);
  // println("idxAP      : %d",iAP);
  // println("_null      : %d",Temp.sd._null);
  
  char buf1[16] = { 0xA7, 0x7B, 0x8B, 0xAB, 0x23, 0x6c, 0xcd, 0x00 ,  0x00, 0x00, 0x00, 0x00 ,  0x00, 0x00, 0x00, 0x00 };
  memset(buf,0,sizeof(buf));
  memcpy(buf,Temp.sd.country,2);

  buf1[7] = 0xFF;
  int i=0;
  for(i=0;i<sizeof(countries)/sizeof(char*);i++)
    if(isEqualText(countries[i],buf))buf1[7]=i;
  
  for(i=0;i<2;i++)
  {
    buf1[i+12] = (iCli >> ( (1-i) *8)) & 0xFF;
  }
  for(i=0;i<2;i++)
  {
    buf1[i+14] = (iAP  >> ( (1-i) *8)) & 0xFF;
  }
  //memcpy(buf1+12,&iCli,2);
  //memcpy(buf1+14,&iAP ,2);
  //for(int i=7;i>4;i--){buf1[i] = (number)&0xFF;number>>=8;}
  
  // buf1[8] = (number)&0xFF;number>>=8;
  // buf1[7] = (number)&0xFF;number>>=8;...
  memcpy(APPKEY,buf1,16);
  //println("APPKEY   : %s",printBin(APPKEY,16));
}
/*ICACHE_RAM_ATTR void handleRxInterrupt()
{
  
  detachInterrupt(2);
  ReceiveEnable = true;
  if(OpenFlag){SPIFFS.remove(COMFile);COMFilefile = SPIFFS.open(COMFile, "a+");Serial.println("OpenFlag");OpenFlag = false;}
  UARTTime = millis();
  int avail = Serial.available();
  //char ch = 0;
  //for(int i=0;i<avail;i++){
  while(avail){
    print("avail : %d\n",avail);
    //ch = Serial.read();
    //Serial.println(" " + (String)ch);
    COMFilefile.write((char)Serial.read());
    avail = Serial.available();
  }
  //Serial.println("");
  //Serial.println("Time : " + (String)(millis() - UARTTime));
  UARTTime = millis();
  attachInterrupt(2, handleRxInterrupt, CHANGE);
  Serial.println("avail = " + (String)avail);
  
}*/
