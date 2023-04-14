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

void ResetBtn();

void DevEUI2RF(uint8_t *DevEUI,uint8_t *RF_addr);
void Masterkey_Data(char* data);
int MasterkeyInterface();


