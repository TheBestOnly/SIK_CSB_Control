#ifndef __LED_H__
#define __LED_H__

#include"sys.h"

#define LED0 PBout(14)
#define LED1 PBout(15)

void Led_Init(void);

#endif
