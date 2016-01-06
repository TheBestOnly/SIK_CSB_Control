#include "delay.h"
#include "led.h"
#include "sys.h"
#include "sbus.h"
#include "usart.h"
#include "Catch.h"
#include "can.h"

int main()
{
	u8 mode=0;	//CAN工作模式;0,普通模式;1,环回模式
	u8 Tx_Dat[4]={0x11,0x13,0x44,0x78};
	
	Stm32_Clock_Init(9);
	delay_init(72);
	Led_Init();
	SBUS_Usart_Init();
	SBUS_Time_Init(1007,999);
	uart_init(36,115200);
	Catch_Init(0xffff,71);	//以1Mhz的频率计数
	CAN1_Init(1,8,15,15,mode);	//CAN普通模式初始化,普通模式,波特率100Kbps
	
	SBUS_Send_Start();
	while(1)
	{
		
		
//			Can_Send_Msg(Tx_Dat,4);
			printf("%d,%d,%d,%d,%d,%d,%d,%d\n",CH[0],CH[1],CH[2],CH[3],CH[4],CH[5],CH[6],CH[7]);
			LED0=~LED0;
			delay_ms(50);
	}
}
