#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include "userardu.h"
#include "main.h"

extern UART_HandleTypeDef *Serial;
GPIO_Pin debugled;//PA8
char _vpstr[1024];
void vprint(const char *fmt, va_list argp)
{
    if(0 < vsprintf(_vpstr,fmt,argp)) // build string
    {
        HAL_UART_Transmit(Serial, (uint8_t*)_vpstr, strlen(_vpstr), 0xffffff); // send message via UART
    }
}
void xprintf(const char *fmt, ...) // custom printf() function
{
    va_list argp;
    va_start(argp, fmt);
    vprint(fmt, argp);
    va_end(argp);
}
void xprintln(const char *fmt, ...) // custom println() function
{
    va_list argp;
    va_start(argp, fmt);
    vprint(fmt, argp);
    va_end(argp);
    HAL_UART_Transmit(Serial, (uint8_t*)"\r\n", 2, 0xffffff); // send message via UART
}

void debug_init () {
	debugled = byte2GPIO_Pin(0xA8);
	pinMode(debugled,OUTPUT);
	digitalWrite(debugled,HIGH);
    // configure USART1 (115200/8N1, tx-only)

    // print banner
    xprintf("\r\n============== DEBUG STARTED ==============\r\n");
}

void debug_led (int val) {
    digitalWrite(debugled,(val ? HIGH : LOW));
}

extern volatile uint32_t uwTick;//Millis
extern volatile uint32_t Micros;

uint8_t Pin2byte(char port,uint8_t pin){
	return (port ? ((port)<<4)|pin : 0x00);
}
GPIO_Pin byte2GPIO_Pin(uint8_t pins){
  GPIO_Pin output;
  if(pins == UNUSED_PIN)output.GPIOPort = (void*)UNUSED_PIN;
  else if((pins & 0xf0) == 0xA0)output.GPIOPort = (void*)GPIOA;
  else if((pins & 0xf0) == 0xB0)output.GPIOPort = (void*)GPIOB;
  else if((pins & 0xf0) == 0xC0)output.GPIOPort = (void*)GPIOC;
  else if((pins & 0xf0) == 0xD0)output.GPIOPort = (void*)GPIOD;
  else if((pins & 0xf0) == 0x20)output.GPIOPort = (void*)GPIOH;
  else assert(0);
  output.GPIOPin = 1<<(pins&0xf);
  return output;
}

HalPinmap_t byte2Pinmap(uint8_t* pins){
	HalPinmap_t hPinmap;
	hPinmap.nss  			= byte2GPIO_Pin(pins[0]);
	hPinmap.rxtx 			= byte2GPIO_Pin(pins[1]);
	hPinmap.rst  			= byte2GPIO_Pin(pins[2]);
	hPinmap.dio[0] 			= byte2GPIO_Pin(pins[3]);
	hPinmap.dio[1] 			= byte2GPIO_Pin(pins[4]);
	hPinmap.dio[2] 			= byte2GPIO_Pin(pins[5]);
	hPinmap.rxtx_rx_active 	= pins[6];
	hPinmap.rssi_cal 		= pins[7];
	hPinmap.spi_freq 		= pins[11]<<24|pins[10]<<16|pins[9]<<8|pins[8];
	return hPinmap;
}
int isunused(GPIO_Pin pin)
{
  return pin.GPIOPort == (void*)UNUSED_PIN;
}
void pinMode(GPIO_Pin pinNumber, int mode) { 
	if(isunused(pinNumber))return;
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  uint32_t GPIOMode = 0;
  uint32_t GPIOPull = 0;
  /*Configure GPIO pin Output Level */
  if(mode == OUTPUT){
    GPIOPull = GPIO_NOPULL;
    GPIOMode = GPIO_MODE_OUTPUT_PP;
    digitalWrite(pinNumber,0);
    //HAL_GPIO_WritePin((GPIO_TypeDef*)pinNumber.GPIOPort, pinNumber.GPIOPin, GPIO_PIN_RESET);
  }else {
	  GPIOMode = GPIO_MODE_INPUT;
    if(mode == INPUT)				GPIOPull = GPIO_NOPULL;
    else if(mode == INPUT_PULLUP)	GPIOPull = GPIO_PULLUP;
    else if(mode == INPUT_PULLDOWN)	GPIOPull = GPIO_PULLDOWN;
    else assert(0);
  }
  /*Configure GPIO pin */
  GPIO_InitStruct.Pin = pinNumber.GPIOPin;
  GPIO_InitStruct.Mode = GPIOMode;
  GPIO_InitStruct.Pull = GPIOPull;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init((GPIO_TypeDef*)pinNumber.GPIOPort, &GPIO_InitStruct);
}
void digitalWrite(GPIO_Pin pinNumber, int status) {
	if(isunused(pinNumber))return;
  //HAL_GPIO_WritePin((GPIO_TypeDef*)pinNumber.GPIOPort, 1<<(pinNumber.GPIOPin), (status ? GPIO_PIN_SET : GPIO_PIN_RESET));
  ((GPIO_TypeDef*)pinNumber.GPIOPort)->BSRR = pinNumber.GPIOPin<<(status?0:16);
}
int digitalRead(GPIO_Pin pinNumber) {
	if(isunused(pinNumber))return -1;
  //return HAL_GPIO_ReadPin((GPIO_TypeDef*)pinNumber.GPIOPort, 1<<(pinNumber.GPIOPort));
  return (((GPIO_TypeDef*)pinNumber.GPIOPort)->IDR & pinNumber.GPIOPin)?1:0;
}

void interrupts(){
  __enable_irq();
}
void noInterrupts(){
  __disable_irq();
}
void Systick_Init(void){
	if (SysTick_Config (SystemCoreClock / 1000)) //1ms per interrupt
	;//while (1);

	//set systick interrupt priority
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);    //4 bits for preemp priority 0 bit for sub priority
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);//i want to make sure systick has highest priority amount all other interrupts

	//Millis = 0;//reset Millis
}
uint32_t micros(void){

    Micros = uwTick*1000 + 1000 - SysTick->VAL/(SystemCoreClock/1000000);

    // = Millis*1000+(SystemCoreClock/1000-SysTick->VAL)/72;
	return Micros;
}

uint32_t millis(void){
	return uwTick;
}

void delay(uint32_t nTime){
	HAL_Delay(nTime);
	//uint32_t curTime = Millis;
	//while((nTime-(Millis-curTime)) < 0);
}

void delayMicroseconds(uint32_t nTime){
	uint32_t var1 = 1000 - SysTick->VAL/(SystemCoreClock/1000000),var2=0;
	uint32_t _Micros = 0;
	while(nTime > _Micros){
		var2 = 1000 - SysTick->VAL/(SystemCoreClock/1000000);
		_Micros += (var2 > var1 ? var2 - var1 : var1 - var2);
		var1 = var2;
	}
}
//Usage(attachInterrupt(digitalPinToInterrupt(plmic_pins->dio[i]), interrupt_fns[i], RISING);)
//attachInterrupt(digitalPinToInterrupt(plmic_pins->dio[i]), interrupt_fns[i], RISING);
volatile void (*callbackfunc[16])() = {0,};
int FlagtoPin(uint16_t flag);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	assert(GPIO_Pin);
	uint8_t i=FlagtoPin(GPIO_Pin);
	xprintf("%d:%p\n\n",i,callbackfunc[i]);
	if(callbackfunc[i])
		callbackfunc[i]();
}
int FlagtoPin(uint16_t flag)
{
	//xprintf("FlagtoPin : 0x%04X",flag);
	int i;
	for(i=0;i<16;i++,flag>>=1)
		if(flag==1)break;
	if(i==16)i=-1;
	//xprintln(" -> %d",i);
	return i;
}
void Pinpullstate(GPIO_TypeDef *gpio,uint16_t Pin,int state)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = Pin;
	GPIO_InitStruct.Pull = state;
	HAL_GPIO_Init(gpio, &GPIO_InitStruct);
}

void attachInterrupt(IT_Pin it, void (*func)(),int state){
	if(isunused(it.Pin))return;
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	it.Func = func;
	it.state = state;
	
	if(state == LOW || state == FALLING)
		GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	else if(state == HIGH || state == RISING)
		GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	else if(state == CHANGE)
		GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
	
	GPIO_InitStruct.Pin = it.Pin.GPIOPin;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(it.Pin.GPIOPort, &GPIO_InitStruct);
	
	HAL_NVIC_SetPriority(it.IRQType, 0, 0);
	HAL_NVIC_EnableIRQ(it.IRQType);
	
	uint8_t i = FlagtoPin(it.Pin.GPIOPin);
	if(i!=-1)callbackfunc[i] = func;
	return;
}
void detachInterrupt(IT_Pin it){
	if(isunused(it.Pin))return;
	HAL_NVIC_DisableIRQ(it.IRQType);
	
	
	uint8_t i = FlagtoPin(it.Pin.GPIOPin);
	if(i!=-1)callbackfunc[i] = 0;
	//noInterrupts();
}

IT_Pin digitalPinToInterrupt(GPIO_Pin pin){
	IT_Pin it,UNUSED_IT_Pin;
	memset(&UNUSED_IT_Pin,0,sizeof(IT_Pin));
	it.Pin = pin;
	if(isunused(pin))return UNUSED_IT_Pin;
	if(pin.GPIOPort == 0)assert(0);
	if(pin.GPIOPin & GPIO_PIN_0)
		it.IRQType = EXTI0_IRQn;
	else if(pin.GPIOPin & (GPIO_PIN_1))
		it.IRQType = EXTI1_IRQn;
	else if(pin.GPIOPin & (GPIO_PIN_2))
		it.IRQType = EXTI2_IRQn;
	else if(pin.GPIOPin & (GPIO_PIN_3))
		it.IRQType = EXTI3_IRQn;
	else if(pin.GPIOPin & (GPIO_PIN_4))
		it.IRQType = EXTI4_IRQn;
	else if(pin.GPIOPin & (GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9))
		it.IRQType = EXTI9_5_IRQn;
	else
		it.IRQType = EXTI15_10_IRQn;
	return it;
}

/*
//Usage : attachInterrupt(digitalPinToInterrupt(plmic_pins->dio[i]), interrupt_fns[i], RISING);
volatile void (*callbackfunc[2][3])() = {0,};
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin == GPIO_PIN_1){
		xprintf("[%lu] dio0 interrupted\n",micros());
		if(callbackfunc[0][0])
			callbackfunc[0][0]();
	}
	if(GPIO_Pin == GPIO_PIN_5){
		xprintf("[%lu] dio1 interrupted\n",micros());
		if(callbackfunc[1][0])
			callbackfunc[1][0]();
	}else if(GPIO_Pin == GPIO_PIN_6){
		xprintf("[%lu] dio2 interrupted\n",micros());
		if(callbackfunc[1][1])
			callbackfunc[1][1]();
	}
}

void attachInterrupt(IT_Pin it, void (*func)(),int state){
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	it.Func = func;
	it.state = state;
	
	if(state == LOW || state == FALLING)
		GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	else if(state == HIGH || state == RISING)
		GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	else if(state == CHANGE)
		GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
	
	GPIO_InitStruct.Pin = it.Pin.GPIOPin;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(it.Pin.GPIOPort, &GPIO_InitStruct);
	
	HAL_NVIC_SetPriority(it.IRQType, 0, 0);
	HAL_NVIC_EnableIRQ(it.IRQType);
	
	if(it.IRQType == EXTI1_IRQn){
		if(it.Pin.GPIOPin == GPIO_PIN_1)
			callbackfunc[0][0] = func;
	}else if(it.IRQType == EXTI9_5_IRQn){
		if(it.Pin.GPIOPin == GPIO_PIN_5)
			callbackfunc[1][0] = func;
		else if(it.Pin.GPIOPin == GPIO_PIN_6)
			callbackfunc[1][1] = func;
	}
	
}

void detachInterrupt(IT_Pin it){
	HAL_NVIC_DisableIRQ(it.IRQType);
	
	if(it.IRQType == EXTI1_IRQn){
		if(it.Pin.GPIOPin == GPIO_PIN_1)
			callbackfunc[0][0] = 0;
	}else if(it.IRQType == EXTI9_5_IRQn){
		if(it.Pin.GPIOPin == GPIO_PIN_5)
			callbackfunc[1][0] = 0;
		else if(it.Pin.GPIOPin == GPIO_PIN_6)
			callbackfunc[1][1] = 0;
	}
	noInterrupts();
}

IT_Pin digitalPinToInterrupt(GPIO_Pin pin){
	IT_Pin it;
	it.Pin = pin;
	if(pin.GPIOPort == 0)assert(0);
	else if(pin.GPIOPort == (void*)GPIOA)//PA
	{
	}else if(pin.GPIOPort == (void*)GPIOB)//PB
	{
		if(pin.GPIOPin == GPIO_PIN_1)
			it.IRQType = EXTI1_IRQn;
		else if(pin.GPIOPin == GPIO_PIN_5)
			it.IRQType = EXTI9_5_IRQn;
		else if(pin.GPIOPin == GPIO_PIN_6)
			it.IRQType = EXTI9_5_IRQn;
		else it.IRQType=0;
	}else if(pin.GPIOPort == (void*)GPIOC)//PC
	{
	}else if(pin.GPIOPort == (void*)GPIOD)//PD
	{
	}else if(pin.GPIOPort == (void*)GPIOH)//PH
	{
		if(pin.GPIOPin == GPIO_PIN_3)
			it.IRQType = EXTI3_IRQn;
		else it.IRQType=0;
	}
	return it;
}
*/