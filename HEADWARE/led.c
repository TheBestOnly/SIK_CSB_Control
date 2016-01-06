#include "led.h"
#include "sys.h"

void Led_Init(void)
{
	RCC->APB2ENR |= 1<<3;	//使能PORTB时钟

	
	GPIOB->CRH &=0x00fffffF;
	GPIOB->CRH |=0x33000000;	//PB14/PB15推挽输出
	GPIOB->ODR |=1<<14;			//PB14输出高
	GPIOB->ODR |=1<<15;			//PB15输出高
}
