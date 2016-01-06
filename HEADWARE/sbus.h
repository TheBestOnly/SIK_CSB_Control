#ifndef __SBUS_H
#define __SBUS_H

#include "sys.h"
#include "led.h"
#include "Catch.h"

#define Startbit 0X0F
#define Endbit 0X00

#define SBUS_SIGNAL_OK 0
#define SBUS_SIGNAL_LOST 1
#define SBUS_SIGNAL_FAILSAFE 2

void SBUS_Usart_Init();
void SBUS_Time_Init(u16 arr,u16 psc);
void SBUS_SendOne(u8 ch);
void SBUS_Send_Start();
void SBUS_Send_End();
void SBUS_Send_Data(u16 *ch);
u8 SBUS_Convert(u8 *data_in);

#endif
