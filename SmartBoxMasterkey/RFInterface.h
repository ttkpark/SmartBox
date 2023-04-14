
#ifdef  __RFINTERFACE_H__
#define __RFINTERFACE_H__

#ifdef __cplusplus
extern "C"
{
#endif
uint8_t *RFCode;

// Initalizes the RF Module.
void RFInit();

// Sends the data to Masterkey
void RFLoop();

int WZRF_Recvdata();

int WZRF_Rx();
int WZRF_Tx();

#ifdef __cplusplus
}
#endif

#endif //__RFINTERFACE_H__