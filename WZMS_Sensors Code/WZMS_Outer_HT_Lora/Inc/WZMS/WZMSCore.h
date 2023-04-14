#include "WZMSRF.h"
#include "WZMSSensor.h"
#include "WZMSInterface.h"
#include "WZMSData.h"
#include "WZMSBase.h"

#ifndef _WZMS2Core_h_
#define _WZMS2Core_h_

#ifdef __cplusplus
extern "C"
{
#endif


// Initalizes the WZMS Core.
void WZMS_Init();

// Manages the WZMS Core.
void WZMSLoop();

// Reconnects to the Network.
int Resetup();

void ResetBtn();

#ifdef __cplusplus
}
#endif

#endif //_WZMS2Core_h_