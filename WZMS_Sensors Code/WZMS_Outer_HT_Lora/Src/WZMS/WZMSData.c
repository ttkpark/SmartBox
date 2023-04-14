#include "WZMSData.h"
#include "WZMSCore.h"


#ifdef __cplusplus
extern "C"
{
#endif

#define Debug println


DATA_SystemConst data_SystemConst;
DATA_SystemInfo data_SystemInfo;

uint8_t initConstField[12] = { 0xE2 ,0x0E ,0xF0 ,0xFE  ,0x04 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00};
char* sensorInfo = "_0tp_1hu_3bt";

// Setup the Data Repository
void WZMSData_init()
{
    Data_SystemConst = &data_SystemConst;
	Data_SystemInfo = &data_SystemInfo;
	//Data_APL = &data_APL;
	File_SystemInfo = "SystemInfo.wzc";
	//File_APList = "APL.wzc";
	File_SystemConst = "SystemConst.wzc"; 

	//Data_SystemInfo  = (DATA_SystemInfo*)Str_SysNameInit;
	//Data_SystemConst = (DATA_SystemConst*)Sense_Info;
	//Data_APL         = (DATA_APL*)&len_APList;
	Debug("Data_SystemInfo  : %d", (int)Data_SystemInfo);
	Debug("Data_SystemConst : %d", (int)Data_SystemConst);
	
	/*
	if (SPIFFS.begin()) {
		println("SPIFFS is Supported!");
	}
	if (!SPIFFS.exists("Dummy"))
		SPIFFS_FileUpdate("Dummy", PortData, 320);
	if (!SPIFFS.exists("Num.t")) {
		println("File Creating4"); resp = 0;
		SPIFFS_FileUpdate("Num.t", &resp, 4);
	}
	if (!SPIFFS.exists(File_SystemConst)) {
		println("File Creating3");
		//resp = Store_SystemConst();
		Debug("resp = %d", resp);
	}
	if (!SPIFFS.exists(File_SystemInfo)) {
		println("File Creating1");
		//resp = Store_SystemInfo();
		Debug("resp = %d", resp);
	}
	if (!SPIFFS.exists(File_APList)) {
		println("File Creating2");
		//resp = Store_APL();
		Debug("resp = %d", resp);
	}
	*/

	
	Read_SystemInfo();
	Read_SystemConst();

	//int num = -1;
	//num=2;//Resetcount=1;
	//println("Reset Count : %d",num); 
	
	
	println("EEPROM Test");
	uint8_t testval = 0,ret = 0;

	WriteEEPROM(0xFF,&testval,1);
	ReadEEPROM(0xFF,&testval,1);
	if(testval != 0)println("EEPROM Clear Error!!");

	testval = 0xE5;
	WriteEEPROM(0xFF,&testval,1);
	ReadEEPROM(0xFF,&ret,1);
	if(ret != testval)println("EEPROM Writing Error!!");

	/*
	char* text = "Haha this is many-bytes long term.Hahaha!!!!";
	WriteEEPROM(0xB2,text,strlen(text));

	char* txtbuff = (char*)malloc(strlen(text)+1);
	ReadEEPROM(0xB2,txtbuff,strlen(text)); txtbuff[strlen(text)]=0;
	println("Teststr : %s",txtbuff);
	if(strcmp(txtbuff,text) != 0)println("EEPROM simmilarity Error!!");
	*/

	println("AT24C16 Memory");

    char Array[256];
	ReadEEPROM(0x00,Array,256);
	PrintMemoryMap(0x00,Array, 256, 16);

	//Addstr_destroy(&PrintMemoryMap_str);

	//LOG file creating
	// sprintf(LOGFILENAME, "LOG_%d.log", num);
	// SPIFFS_FileDelete(LOGFILENAME);
	// SPIFFS_FileUpdate(LOGFILENAME, (void*)"LOG Start!!\n", 12);

	//Debug_fileopen();
	//delay_ms(500);

    
}

int Store_SystemInfo() {
	return Store_systemInfo();
}
int Read_SystemInfo() {
	return Read_systemInfo();
}
int Read_SystemConst() {
	return Read_systemConst();
}
/*
int Store_systemInfo() {
	println("Store_systemInfo()");
	WriteEEPROM(FLASH_ADDR_SYSINFO,Data_SystemInfo,sizeof(DATA_SystemInfo));

	return 0;
}
int Read_systemInfo() {
	println("Read_systemInfo()");
	ReadEEPROM(FLASH_ADDR_SYSINFO,Data_SystemInfo,sizeof(DATA_SystemInfo));
	//memcpy(Data_SystemInfo,FLASH_ADDR_SYSINFO,sizeof(DATA_SystemInfo));
	
	return 0;
}
*/

int Store_systemInfo() {
	Revision Revisiondata;
	println("Store_systemInfo()");
	int index = 0;
	int systeminfoLen = 16+8+4+4+4+1+ + Data_SystemInfo->StrProductbytesize +1+ Data_SystemInfo->RACllbytesize
	+1 + Data_SystemInfo->RACmmbytesize+1 + Data_SystemInfo->RACssbytesize+4+4+sizeof(Revision)+sizeof(StatusRange);

	println("systeminfoLen : %d", systeminfoLen);

	uint8_t *WriteMemory = (uint8_t*)malloc(systeminfoLen + 2);//SystemInfo LEn(1Byte) + systeminfoLen + Null protect(1Byte)
	println("WriteMemory : %p", WriteMemory);
	WriteMemory[systeminfoLen + 1] = 0;
	WriteMemory[index++] = systeminfoLen;

	memcpy(WriteMemory+index, &Data_SystemInfo->APPKEY     , 16);index+=16;
	memcpy(WriteMemory+index, &Data_SystemInfo->startdate  , 8);index+=8;
	memcpy(WriteMemory+index, &Data_SystemInfo->initdate   , 8);index+=8;
	memcpy(WriteMemory+index, &Data_SystemInfo->warningmin , 4);index+=4;
	memcpy(WriteMemory+index, &Data_SystemInfo->dangermin  , 4);index+=4;
	memcpy(WriteMemory+index, &Data_SystemInfo->Savetotmin , 4);index+=4;

	WriteMemory[index++] = Data_SystemInfo->StrProductbytesize;memcpy(WriteMemory+index, Data_SystemInfo->StrProduct , Data_SystemInfo->StrProductbytesize);index+=Data_SystemInfo->StrProductbytesize;
	WriteMemory[index++] = Data_SystemInfo->RACllbytesize;     memcpy(WriteMemory+index, Data_SystemInfo->RACll      , Data_SystemInfo->RACllbytesize);     index+=Data_SystemInfo->RACllbytesize;
	WriteMemory[index++] = Data_SystemInfo->RACmmbytesize;     memcpy(WriteMemory+index, Data_SystemInfo->RACmm      , Data_SystemInfo->RACmmbytesize);     index+=Data_SystemInfo->RACmmbytesize;
	WriteMemory[index++] = Data_SystemInfo->RACssbytesize;     memcpy(WriteMemory+index, Data_SystemInfo->RACss	     , Data_SystemInfo->RACssbytesize);     index+=Data_SystemInfo->RACssbytesize;
	
	memcpy(WriteMemory+index, &Data_SystemInfo->sendtime  , 4);index+=4;
	memcpy(WriteMemory+index, &Data_SystemInfo->isreplace , 4);index+=4;
	
	for(int i=0;i<2;i++){
		Revisiondata.Type[i] = Data_SystemInfo->Type_Adjust[i];
		Revisiondata.Val[i] =  Data_SystemInfo->Val_Adjust[i];
	}
	memcpy(WriteMemory+index, &Revisiondata 			, sizeof(Revision));   index+=sizeof(Revision);
	memcpy(WriteMemory+index, &Data_SystemInfo->staRange, sizeof(StatusRange));//index+=sizeof(StatusRange);

	
	PrintMemoryMap(1, WriteMemory+1, systeminfoLen, 16);
	Sen_WriteEEPROM(FLASH_ADDR_SYSINFO,WriteMemory,systeminfoLen + 1);
	free(WriteMemory);
	return 0;
}
int zerobuf[256] = {0,};
int Read_systemInfo() {
	Revision Revisiondata;
	println("Read_systemInfo()");
	uint8_t systeminfoLen;
	int index = 0;
	ReadEEPROM(FLASH_ADDR_SYSINFO,&systeminfoLen,1);
	println("systeminfoLen : %d", systeminfoLen);
	Data_SystemInfo->isAvailable = systeminfoLen != 0 && systeminfoLen < 0xA0;
	if(systeminfoLen >= 0xA0) return -1;//Memory Error
	if(systeminfoLen == 0)return -1;
	uint8_t *ReadMemory = (uint8_t*)malloc(systeminfoLen + 1);//systeminfoLen + Null protect(1Byte)
	ReadMemory[systeminfoLen] = 0;//Memory protect

	ReadEEPROM(FLASH_ADDR_SYSINFO + 1,ReadMemory,systeminfoLen);
	PrintMemoryMap(1, ReadMemory, systeminfoLen, 16);
	
	memcpy(&Data_SystemInfo->APPKEY     ,ReadMemory+index,  16);index+=16;
	memcpy(&Data_SystemInfo->startdate  ,ReadMemory+index,  8); index+=8;
	memcpy(&Data_SystemInfo->initdate   ,ReadMemory+index,  8); index+=8;
	memcpy(&Data_SystemInfo->warningmin ,ReadMemory+index,  4); index+=4;
	memcpy(&Data_SystemInfo->dangermin  ,ReadMemory+index,  4); index+=4;
	memcpy(&Data_SystemInfo->Savetotmin ,ReadMemory+index,  4); index+=4;

	Data_SystemInfo->StrProductbytesize = ReadMemory[index++];
	if(Data_SystemInfo->StrProductbytesize>40){WriteEEPROM(0x00,zerobuf,256);return;}
	Data_SystemInfo->StrProduct = (uint16_t*)realloc(Data_SystemInfo->StrProduct, Data_SystemInfo->StrProductbytesize);
	memcpy(Data_SystemInfo->StrProduct ,ReadMemory+index,  Data_SystemInfo->StrProductbytesize);
	index+=Data_SystemInfo->StrProductbytesize;

	Data_SystemInfo->RACllbytesize = ReadMemory[index++];
	if(Data_SystemInfo->RACllbytesize>40){WriteEEPROM(0x00,zerobuf,256);return;}
	Data_SystemInfo->RACll = (uint16_t*)realloc(Data_SystemInfo->RACll, Data_SystemInfo->RACllbytesize);
	memcpy(Data_SystemInfo->RACll, ReadMemory+index,  Data_SystemInfo->RACllbytesize);
	index+=Data_SystemInfo->RACllbytesize;

	Data_SystemInfo->RACmmbytesize = ReadMemory[index++];
	if(Data_SystemInfo->RACmmbytesize>40){WriteEEPROM(0x00,zerobuf,256);return;}
	Data_SystemInfo->RACmm = (uint16_t*)realloc(Data_SystemInfo->RACmm, Data_SystemInfo->RACmmbytesize);
	memcpy(Data_SystemInfo->RACmm, ReadMemory+index,  Data_SystemInfo->RACmmbytesize);
	index+=Data_SystemInfo->RACmmbytesize;

	Data_SystemInfo->RACssbytesize = ReadMemory[index++];
	if(Data_SystemInfo->RACssbytesize>40){WriteEEPROM(0x00,zerobuf,256);return;}
	Data_SystemInfo->RACss = (uint16_t*)realloc(Data_SystemInfo->RACss, Data_SystemInfo->RACssbytesize);
	memcpy(Data_SystemInfo->RACss, ReadMemory+index, Data_SystemInfo->RACssbytesize);
	index+=Data_SystemInfo->RACssbytesize;
	
	memcpy(&Data_SystemInfo->sendtime  ,ReadMemory+index,  4);index+=4;
	memcpy(&Data_SystemInfo->isreplace ,ReadMemory+index,  4);index+=4;

	memcpy(&Revisiondata ,ReadMemory+index,  sizeof(Revision));index+=sizeof(Revision);
	for(int i=0;i<2;i++){
		Data_SystemInfo->Type_Adjust[i] = Revisiondata.Type[i];
		Data_SystemInfo->Val_Adjust[i]  = Revisiondata.Val[i];
	}
	memcpy(&Data_SystemInfo->staRange ,ReadMemory+index,  sizeof(StatusRange));//index+=RACssLen;
	free(ReadMemory);
	Data_SystemInfo->EEPROM_Len = systeminfoLen;
	return 0;
}

int Read_systemConst() {
	println("Read_systemConst()");
	// memcpy(Data_SystemConst,initConstField,sizeof(DATA_SystemConst));
    // ReadEEPROM(FLASH_ADDR_SYSCONST,Data_SystemConst,sizeof(DATA_SystemConst));
	Data_SystemConst->Code_SystemCode = 0;
	memcpy(Data_SystemConst->DEVEUI, initConstField + 4, 8);
	memcpy(Data_SystemConst->senseinfo, sensorInfo, strlen(sensorInfo));
	Data_SystemConst->bt_high = 4.1;
	Data_SystemConst->bt_low = 2.7;
	PrintMemoryMap(Data_SystemConst,Data_SystemConst,sizeof(DATA_SystemConst), 16);
	return 0;
}

void SavedInfo() {
	
	print("SavedInfo() start");

	DATA_SystemConst 	*sysconst = Data_SystemConst;
	DATA_SystemInfo 	*sysinfo = Data_SystemInfo;
	print( "File Requesting Result\r\n");

	// print("File 1:%s\n size :%d\r\n", File_SystemConst, sizeof(DATA_SystemConst));
	// print("File 2:%s\n size :%d\r\n", File_SystemInfo, sizeof(DATA_SystemInfo));

	char* fmt_0 = "\t %-15s  : %s\r\n";
	char* fmt_1 = "\t %-15s  : %d\r\n";
    
	// print( "File %s   Datas\r\n", File_SystemConst);
	print( fmt_0, "Code_SystemCode", printBin(&sysconst->Code_SystemCode, 4));
	print( fmt_0, "DevEUI", printBin(sysconst->DEVEUI, 8));
	print( fmt_0, "senseinfo", sysconst->senseinfo);
	print( fmt_0, "bt_high", float_print(sysconst->bt_high));
	print( fmt_0, "bt_low", float_print(sysconst->bt_low));
	
	if(/*Data_SystemInfo->isAvailable*/1){
		// print( fmt_1, "Int_SenseTime", sysinfo->Int_SenseTime);

		print( fmt_1,  "startdate" , sysinfo->startdate);
		print( fmt_1,  "initdate"  , sysinfo->initdate);
		print( fmt_1,  "warningmin", sysinfo->warningmin);
		print( fmt_1,  "dangermin" , sysinfo->dangermin);
		print( fmt_1,  "Savetotmin", sysinfo->Savetotmin);
		// print( fmt_1,  "StrProduct", sysinfo->StrProductbytesize);
		// print( fmt_1,  "RACll     ", sysinfo->RACllbytesize);
		// print( fmt_1,  "RACmm     ", sysinfo->RACmmbytesize);
		// print( fmt_1,  "RACss     ", sysinfo->RACssbytesize);
		print( fmt_0,  "StrProduct", printBin(sysinfo->StrProduct, sysinfo->StrProductbytesize));
		print( fmt_0,  "RACll     ", printBin(sysinfo->RACll     , sysinfo->RACllbytesize));
		print( fmt_0,  "RACmm     ", printBin(sysinfo->RACmm     , sysinfo->RACmmbytesize));
		print( fmt_0,  "RACss     ", printBin(sysinfo->RACss     , sysinfo->RACssbytesize));
		print( fmt_1,  "sendtime"  , sysinfo->sendtime);
		print( fmt_1,  "isreplace" , sysinfo->isreplace);

		print( fmt_0, "AppKey", printBin(sysinfo->APPKEY,16));
		
		for(int i=0;i<2;i++)
		{
			print( "\t %-14s[%d]:(0x%02X) %s\r\n", "Val_Adjust", i, sysinfo->Type_Adjust[i], float_print(sysinfo->Val_Adjust[i]));
		}
		
		print( fmt_0, "Range.Type     ", printBin(&sysinfo->staRange.Type, 1));
		print( fmt_0, "Range.d01fr    ", float_print(sysinfo->staRange.d01fr));
		print( fmt_0, "Range.d01to    ", float_print(sysinfo->staRange.d01to));
		print( fmt_0, "Range.d02fr    ", float_print(sysinfo->staRange.d02fr));
		print( fmt_0, "Range.d02to    ", float_print(sysinfo->staRange.d02to));
		print( fmt_0, "Range.w01fr    ", float_print(sysinfo->staRange.w01fr));
		print( fmt_0, "Range.w01to    ", float_print(sysinfo->staRange.w01to));
		print( fmt_0, "Range.w02fr    ", float_print(sysinfo->staRange.w02fr));
		print( fmt_0, "Range.w02to    ", float_print(sysinfo->staRange.w02to));
	}else println("Systeminfo isn`t Available!!");

	/*

	//delay_ms(-1);
	Debug("SavedInfo() start");
	//SPIFFS_FileRead(File_SystemConst, (void*)Data_SystemConst, sizeof(DATA_SystemConst));
	//SPIFFS_FileRead(File_SystemInfo, (void*)Data_SystemInfo, sizeof(DATA_SystemInfo));
	//SPIFFS_FileRead(File_APList, (void*)Data_APL, sizeof(DATA_APL));

	DATA_SystemConst 	*sysconst = Data_SystemConst;
	DATA_SystemInfo 	*sysinfo = Data_SystemInfo;
	//DATA_APL		 	*sysapl = Data_APL;
	print( "File Requesting Result\r\n");

	//str += (String)"Data_SystemInfo->Str_Client       : " + (int)Data_SystemInfo->Str_Client + "," + (int)sysinfo->Data_SystemInfo->Str_Client + "\n";
	//str += (String)"Data_SystemInfo->Str_cloudAddr    : " + (int)Data_SystemInfo->Str_cloudAddr + "," + (int)sysinfo->Data_SystemInfo->Str_cloudAddr + "\n";
	//str += (String)"Data_SystemInfo->Str_sip          : " + (int)Data_SystemInfo->Str_sip + "," + (int)sysinfo->Data_SystemInfo->Str_sip + "\n";
	//str += (String)"Data_SystemInfo->Str_WkzoneCode   : " + (int)Data_SystemInfo->Str_WkzoneCode + "," + (int)sysinfo->Data_SystemInfo->Str_WkzoneCode + "\n";
	//str += (String)"Data_SystemConst->Str_ComType      : " + (int)Data_SystemConst->Str_ComType + "," + (int)sysinfo->Data_SystemConst->Str_ComType + "\n";
	//str += (String)"Data_SystemConst->Int_ComType      : " + (int)&Data_SystemConst->Int_ComType + "," + (int)&(sysinfo->Data_SystemConst->Int_ComType) + "\n";
	//str += (String)"Data_SystemInfo->Int_SenseTime    : " + (int)&Data_SystemInfo->Int_SenseTime + "," + (int)&(sysinfo->Data_SystemInfo->Int_SenseTime) + "\n";
	//str += (String)"Data_SystemInfo->Int_ServerPort   : " + (int)&Data_SystemInfo->Int_ServerPort + "," + (int)&(sysinfo->Data_SystemInfo->Int_ServerPort) + "\n";
	//str += (String)"Data_SystemInfo->Int_NowAPNum     : " + (int)&Data_SystemInfo->Int_NowAPNum + "," + (int)&(sysinfo->Data_SystemInfo->Int_NowAPNum) + "\n";
	//str += (String)"Data_SystemInfo->Int_GiveupCountS : " + (int)&Data_SystemInfo->Int_GiveupCountS + "," + (int)&(sysinfo->Data_SystemInfo->Int_GiveupCountS) + "\n";
	//str += (String)"Data_SystemInfo->Int_GiveupCountC : " + (int)&Data_SystemInfo->Int_GiveupCountC + "," + (int)&(sysinfo->Data_SystemInfo->Int_GiveupCountC) + "\n";
	//str += (String)"Data_SystemInfo->Int_RetryCountS  : " + (int)&Data_SystemInfo->Int_RetryCountS + "," + (int)&(sysinfo->Data_SystemInfo->Int_RetryCountS) + "\n";
	//str += (String)"Data_SystemInfo->Int_RetryCountC  : " + (int)&Data_SystemInfo->Int_RetryCountC + "," + (int)&(sysinfo->Data_SystemInfo->Int_RetryCountC) + "\n";
	//
	print("File 1:%s\n size :%d\r\n", File_SystemConst, sizeof(DATA_SystemConst));
	print("File 2:%s\n size :%d\r\n", File_SystemInfo, sizeof(DATA_SystemInfo));
	//print("File 3:%s\n size :%d\r\n", File_APList, sizeof(DATA_APL));

	char* fmt_0 = "\t %-15s  : %s\r\n";
	char* fmt_1 = "\t %-15s  : %d\r\n";
	//char* fmt_2 = "\t %-15s  : \r\n";
	print( "File %s   Datas\r\n", File_SystemConst);
	print( fmt_0, "DevEUI", printBin(sysconst->DEVEUI, 8));
	print( fmt_0, "senseinfo", sysconst->senseinfo);
	print( fmt_0, "bt_low", float_print(sysconst->bt_low));
	print( fmt_0, "bt_high", float_print(sysconst->bt_high));
	print( fmt_0, "Code_SystemCode", printBin(&sysconst->Code_SystemCode, 4));
	//print( fmt_0, "Str_SN", sysconst->Str_SN);
	//print( fmt_0, "Str_SysNameInit", sysconst->Str_SysNameInit);
	//print( fmt_0, "Str_ComType", sysconst->Str_ComType);
	//print( fmt_1, "Int_ComType", sysconst->Int_ComType);



	//print( "File %s   Datas\r\n", File_SystemInfo);
	
	// print( fmt_0, "Str_Client", sysinfo->Str_Client);
	// print( fmt_0, "Str_cloudAddr", sysinfo->Str_cloudAddr);
	// print( fmt_0, "Str_sip", sysinfo->Str_sip);
	// print( fmt_0, "Str_WkzoneCode", sysinfo->Str_WkzoneCode);
	
	print( fmt_0, "AppKey", printBin(sysinfo->APPKEY,16));
	print( fmt_1, "Int_SenseTime", sysinfo->sendtime);

	
	for(int i=0;i<5;i++)
	{
		print( "\t %-15s[%d]  : %s\r\n", "Val_Adjust", i, float_print(sysinfo->Val_Adjust[i]));
	}
	// print( fmt_1, "Int_ServerPort", sysinfo->Int_ServerPort);
	// print( fmt_1, "Int_NowAPNum", sysinfo->Int_NowAPNum);
	// print( fmt_1, "Int_GiveupCountS", sysinfo->Int_GiveupCountS);
	// print( fmt_1, "Int_GiveupCountC", sysinfo->Int_GiveupCountC);
	// print( fmt_1, "Int_RetryCountS", sysinfo->Int_RetryCountS);
	// print( fmt_1, "Int_RetryCountC", sysinfo->Int_RetryCountC);
	

	// PrintAPL();
	// print( "File %s   Datas\r\n", File_APList);
	// print( fmt_1, "len_APList", sysapl->len_APList);
	// print( fmt_1, "len_APListField", sysapl->len_APListField);
	// int _Count_ListAP = sysapl->len_APList / sizeof(APLIST);
	// print( fmt_1, "len_APList", sysapl->len_APList);
	// print( fmt_1, "_Count_ListAP", _Count_ListAP);
	// print( fmt_1, "Count_ListAP", Count_ListAP);
	// if (_Count_ListAP < 128)for (int i = 0; i < _Count_ListAP; i++) {
	// 	Debug("979");
	// 	print("\t AP %d apcode : %s\r\n", i, sysapl->APList_Field + sysapl->APLists[i].apcode);
	// 	print("\t AP %d apssid : %s\r\n", i, sysapl->APList_Field + sysapl->APLists[i].apssid);
	// 	print("\t AP %d appwd  : %s\r\n", i, sysapl->APList_Field + sysapl->APLists[i].appwd);
	// 	print( "\t \r\n");
	// 	Debug("984");
	// }
	//Debug("121:listnet.num = %d", listnet.num);
	Debug("985");
	//Debug("124:\tlistnet.num = %d", listnet.num);
	print("File 1:%s\n raw data :", File_SystemConst);
	PrintMemoryMap(0,sysconst, sizeof(DATA_SystemConst), 16);//it's size is grater than 128.(549)
	print( "\r\n");
	//Debug("124:\tlistnet.num = %d", listnet.num);

	print("File 2:%s\n raw data :", File_SystemInfo);
	PrintMemoryMap(0,sysinfo, sizeof(DATA_SystemInfo), 16);
	print( "\r\n");
	//Debug("124:\tlistnet.num = %d", listnet.num);

	// print( PrintMemoryMap(0,sysapl->APLists, 136, 16));
	// print( "\r\n"); 
	*/

	print( "File log Complpted.\r\n");
}


#ifdef __cplusplus
}
#endif