
#include "WZMSBase.h"
#include "NRF/nrf24.h"

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#ifdef SystemCoreClock
#undef SystemCoreClock
#define SystemCoreClock 32000000UL
#endif
/* USER CODE END PD */

/* USER CODE BEGIN PFP */
void RFInit();
void RFloop();
int RFsend();
/*
char uart_get(void);
void uart_set(char);
char uart_getTM(uint32_t);
*/
volatile void BtnINT();
int WZRF_Tx();
int WZRF_Rx();
/* USER CODE END PFP */