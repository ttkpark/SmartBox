
#include "WZMSBase.h"
#include "RFInterface.h"

#ifndef _WZMS2RF_h_
#define _WZMS2RF_h_

#ifdef __cplusplus
extern "C"
{
#endif

uint8_t *RFCode;

// Initalizes the RF Module.
int  WZMSRF_init();

// Sends the data to Masterkey
int WZMSRF_Masterkey();

#ifdef __cplusplus
}
#endif

#endif //_WZMS2RF_h_