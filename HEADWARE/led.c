#include "led.h"
#include "sys.h"

void Led_Init(void)
{
	RCC->APB2ENR |= 1<<3;	//ʹ��PORTBʱ��

	
	GPIOB->CRH &=0x00fffffF;
	GPIOB->CRH |=0x33000000;	//PB14/PB15�������
	GPIOB->ODR |=1<<14;			//PB14�����
	GPIOB->ODR |=1<<15;			//PB15�����
}
