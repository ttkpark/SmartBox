
#include "WZMSBase.h"
#include "WZMSLora.h"

#ifndef _WZMS2Interface_h_
#define _WZMS2Interface_h_

#ifdef __cplusplus
extern "C"
{
#endif
int WZMSInt_EmergencyTime;
// Setup for WZMS Interface. (LoRa, WiFi, etc.)
int  WZMSInt_init();

// Get from Interfacing Module.
int  WZMSInt_stat();

// Sends the packet to the Module.
void WZMSInt_Data();
void WZMSIntBase_Data();

// Module's Processing function.
void WZMSInt_Loop();

// Reconnects from Network. (after the ResetBtn)
void WZMSInt_ReConnect();

// Resetup the Module. (after the Masterkey Interfacing)
void WZMSInt_Resetup();

// Processing the packet. (Encoding Data->Packet)
void WZMSInt_DataProcess();

// set the Parameters.
void WZMSInt_preset();

#ifdef __cplusplus
}
#endif

#endif //_WZMS2Interface_h_