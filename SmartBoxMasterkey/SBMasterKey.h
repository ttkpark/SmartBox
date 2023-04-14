#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "RFInterface.h"
#include <stdarg.h>
#include <FS.h>
#include <ArduinoJson.h>
#include "JsonStreamingParser.h"
#include "JsonListener.h"


#define Debug println
#define Debug2 println
#define Debug3 println
#define Debug4 print

#define MAX_BASE_CLIENTS 5
#define MAX_SRV_CLIENTS 4


typedef struct{
  uint8_t a[4];
} IPAddr; 

typedef struct
{
  uint8_t BinVersion;
  //uint32_t filelen;
}Struct_Bininfo;

void WifiInit();
int handleNet();
int acceptClient(int Ncli);
IPAddr _IPs(IPAddress ip);

int isConnedtedNet();
int SearchNet();
int ConnectNet(char* name, char* pass);
int myAPShut();
int disConnectNet();
int commitport(IPAddr addr);
int commithost(char* Str);
;
int serverOpen(int port);
int open(int ch, char* addr, int port);
int close(int ch);
int sendMessage(int ch, uint8_t *data, int len);
int receiveDetect(int ch, int timeout);
int receiveMessage(int ch, uint8_t *data);
int DataAvailable(int ch);
;
void Process_getkey_preset();
extern "C" void Process_getkey();
extern "C" void Process_Access(char* name_cstr);
void Process_Root();
void MSKeyloop();
void Port_uartprocessing();

void senduart(uint8_t);
int Server_Connects(const char * ssid, const char * ssid_pw);

extern "C" void Apn2EUI(char *APnum,uint8_t * APPKEY);
extern "C" void DEV2snm(uint8_t *DEVEUI,char * SNM);
