#ifndef __CATCH_H
#define __CATCH_H

#include "sys.h"
#include "led.h"

extern u8  TIM4CH1_CAPTURE_STA; //���벶��״̬ 
extern u16 TIM4CH1_CAPTURE_VAL;//���벶��ֵ
extern u8  TIM4CH2_CAPTURE_STA; //���벶��״̬ 
extern u16 TIM4CH2_CAPTURE_VAL;//���벶��ֵ
extern u8  TIM4CH3_CAPTURE_STA; //���벶��״̬ 
extern u16 TIM4CH3_CAPTURE_VAL;//���벶��ֵ
extern u8  TIM4CH4_CAPTURE_STA; //���벶��״̬ 
extern u16 TIM4CH4_CAPTURE_VAL;//���벶��ֵ

extern u8  TIM3CH1_CAPTURE_STA; //���벶��״̬ 
extern u16 TIM3CH1_CAPTURE_VAL;//���벶��ֵ
extern u8  TIM3CH2_CAPTURE_STA; //���벶��״̬ 
extern u16 TIM3CH2_CAPTURE_VAL;//���벶��ֵ
extern u8  TIM3CH3_CAPTURE_STA; //���벶��״̬ 
extern u16 TIM3CH3_CAPTURE_VAL;//���벶��ֵ
extern u8  TIM3CH4_CAPTURE_STA; //���벶��״̬ 
extern u16 TIM3CH4_CAPTURE_VAL;//���벶��ֵ


extern u16 CH[12];//����ң������ֵ


void Catch_Init(u16 arr,u16 psc);
void Catch_Cal();




#endif
