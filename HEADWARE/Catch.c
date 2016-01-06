#include "Catch.h"


//-----------------------------------------------------
//���ܣ����ڲ���ң������ֵ
//T3_1(PA6);T3_2(PA7);T3_3(PB0);T3_4(PB1);
//T4_1(PB6);T4_2(PB7);T4_3(PB8);T4_4(PB9);
//by:The Best
//data:2015/12/1
//------------------------------------------------------

//--------------------------------
//�Բ�����г�ʼ��
//--------------------------------
void Catch_Init(u16 arr,u16 psc)
{
		RCC->APB1ENR |= 1<<1;	//������ʱ��3ʱ��
		RCC->APB1ENR |= 1<<2;	//������ʱ��4
	
		RCC->APB2ENR |= 1<<3;	//����PBʱ��
		RCC->APB2ENR |= 1<<2;
		

		GPIOB ->CRL  &=0x00ffff00;
		GPIOB ->CRL  |=0x88000088;   //PB0/1/6/7��������
		GPIOB ->CRH  &=0xfffff00;
		GPIOB ->CRH  |=0x0000088;   //PB8/9��������
		GPIOA ->CRL  &=0x00ffffff;
		GPIOA ->CRL  |=0x88000000;   //PA6/7��������
			
		GPIOB ->ODR |=(0<<0)|(0<<1)|(0<<6)|(0<<7)|(0<<8)|(0<<9);	
		GPIOA ->ODR |=(0<<6)|(0<<7);

		TIM3->ARR=arr;				//TIM3Ԥװֵ
		TIM3->PSC=psc;				//TIM3��Ƶ
	
		TIM3->CCMR1 |=1<<8|1<<0;
		TIM3->CCMR1 |=1<<12|1<<4; //IC4F=0001 �����˲��� �� Fck_int ������ 2 ���¼�����Ч
		TIM3->CCMR1 |=0<<11|0<<3; //IC2PS=00 ���������Ƶ,����Ƶ
		
		TIM3->CCMR2 |=1<<8|1<<0;
		TIM3->CCMR2 |=1<<12|1<<4; //IC4F=0001 �����˲��� �� Fck_int ������ 2 ���¼�����Ч
		TIM3->CCMR2 |=0<<11|0<<3; //IC2PS=00 ���������Ƶ,����Ƶ
		
		TIM4->ARR=arr;				//TIM4Ԥװֵ
		TIM4->PSC=psc;				//TIM4��Ƶ
		
		TIM4->CCMR1 |=1<<8|1<<0;
		TIM4->CCMR1 |=1<<12|1<<4; //IC4F=0001 �����˲��� �� Fck_int ������ 2 ���¼�����Ч
		TIM4->CCMR1 |=0<<11|0<<3; //IC2PS=00 ���������Ƶ,����Ƶ
		
		TIM4->CCMR2 |=1<<8|1<<0;
		TIM4->CCMR2 |=1<<12|1<<4; //IC4F=0001 �����˲��� �� Fck_int ������ 2 ���¼�����Ч
		TIM4->CCMR2 |=0<<11|0<<3; //IC2PS=00 ���������Ƶ,����Ƶ
		
		
		TIM3->CCER|=0<<1; //CC1P=0 �����ز���
		TIM3->CCER|=1<<0; //CC1E=1 �������������ֵ������Ĵ�����
		TIM3->CCER|=0<<5; //CC2P=0 �����ز���
		TIM3->CCER|=1<<4; //CC2E=1 �������������ֵ������Ĵ�����
		TIM3->CCER|=0<<9; //CC3P=0 �����ز���
		TIM3->CCER|=1<<8; //CC3E=1 �������������ֵ������Ĵ�����
		TIM3->CCER|=0<<13; //CC4P=0 �����ز���
		TIM3->CCER|=1<<12; //CC4E=1 �������������ֵ������Ĵ�����
		
		TIM4->CCER|=0<<1; //CC1P=0 �����ز���
		TIM4->CCER|=1<<0; //CC1E=1 �������������ֵ������Ĵ�����
		TIM4->CCER|=0<<5; //CC2P=0 �����ز���
		TIM4->CCER|=1<<4; //CC2E=1 �������������ֵ������Ĵ�����
		TIM4->CCER|=0<<9; //CC3P=0 �����ز���
		TIM4->CCER|=1<<8; //CC3E=1 �������������ֵ������Ĵ�����
		TIM4->CCER|=0<<13; //CC4P=0 �����ز���
		TIM4->CCER|=1<<12; //CC4E=1 �������������ֵ������Ĵ�����
		
		
		TIM3->DIER|=(1<<4|1<<3|1<<2|1<<1); //�������ж�
		TIM3->DIER|=1<<0; //��������ж�
		TIM3->CR1|=0x01; //ʹ�ܶ�ʱ�� 3
		
		TIM4->DIER|=(1<<4|1<<3|1<<2|1<<1); //�������ж�
		TIM4->DIER|=1<<0; //��������ж�
		TIM4->CR1|=0x01; //ʹ�ܶ�ʱ�� 3
		
		
		MY_NVIC_Init(3,0,TIM3_IRQn,2);//��ռ 2�������ȼ� 0���� 2
		MY_NVIC_Init(3,0,TIM4_IRQn,2);//��ռ 2�������ȼ� 1���� 2
	
}


u8  TIM3CH1_CAPTURE_STA=0; //���벶��״̬ 
u16 TIM3CH1_CAPTURE_VAL;//���벶��ֵ
u8  TIM3CH2_CAPTURE_STA=0; //���벶��״̬ 
u16 TIM3CH2_CAPTURE_VAL;//���벶��ֵ
u8  TIM3CH3_CAPTURE_STA=0; //���벶��״̬ 
u16 TIM3CH3_CAPTURE_VAL;//���벶��ֵ
u8  TIM3CH4_CAPTURE_STA=0; //���벶��״̬ 
u16 TIM3CH4_CAPTURE_VAL;//���벶��ֵ


//��ʱ�� 3 �жϷ������
void TIM3_IRQHandler(void)
{ 
	u16 tsr;

	tsr=TIM3->SR;
//------------------------------------------------------------------
//T3-4	
	if((TIM3CH4_CAPTURE_STA&0X80)==0)//��δ�ɹ�����
	{
		if(tsr&0X01)//���
		{ 
			if(TIM3CH4_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
			{
				if((TIM3CH4_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
				{
					TIM3CH4_CAPTURE_STA|=0X80;//��ǳɹ�������һ��
					TIM3CH4_CAPTURE_VAL=0XFFFF;
				}else TIM3CH4_CAPTURE_STA++;
			}
		}
		if(tsr&0x10)//���� 4 ���������¼�
		{
			if(TIM3CH4_CAPTURE_STA&0X40) //����һ���½���
			{ 
				TIM3CH4_CAPTURE_STA|=0X80; //��ǳɹ�����һ�θߵ�ƽ����
				TIM3CH4_CAPTURE_VAL = TIM3->CCR4;//��ȡ��ǰ�Ĳ���ֵ.
				TIM3->CCER&=~(1<<13); //CC4P=0 ����Ϊ�����ز���
			}else //��δ��ʼ,��һ�β���������
			{
					TIM3CH4_CAPTURE_VAL=0;
					TIM3CH4_CAPTURE_STA=0X40; //��ǲ�����������
					TIM3->CNT=0; //���������
					TIM3->CCER|=1<<13; //CC4P=1 ����Ϊ�½��ز���
			} 
		} 
	}
//------------------------------------------------------------------
//T3-3		
		if((TIM3CH3_CAPTURE_STA&0X80)==0)//��δ�ɹ�����
	{
		if(tsr&0X01)//���
		{ 
			if(TIM3CH3_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
			{
				if((TIM3CH3_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
				{
					TIM3CH3_CAPTURE_STA|=0X80;//��ǳɹ�������һ��
					TIM3CH3_CAPTURE_VAL=0XFFFF;
				}else TIM3CH3_CAPTURE_STA++;
			}
		}
		if(tsr&0x08)//���� 4 ���������¼�
		{
			if(TIM3CH3_CAPTURE_STA&0X40) //����һ���½���
			{ 
				TIM3CH3_CAPTURE_STA|=0X80; //��ǳɹ�����һ�θߵ�ƽ����
				TIM3CH3_CAPTURE_VAL = TIM3->CCR3;//��ȡ��ǰ�Ĳ���ֵ.
				TIM3->CCER&=~(1<<9); //CC3P=0 ����Ϊ�����ز���
			}else //��δ��ʼ,��һ�β���������
			{
					TIM3CH3_CAPTURE_VAL=0;
					TIM3CH3_CAPTURE_STA=0X40; //��ǲ�����������
					TIM3->CNT=0; //���������
					TIM3->CCER|=1<<9; //CC4P=1 ����Ϊ�½��ز���
			} 
		} 
	}
//------------------------------------------------------------------
//T3-2		
		if((TIM3CH2_CAPTURE_STA&0X80)==0)//��δ�ɹ�����
	{
		if(tsr&0X01)//���
		{ 
			if(TIM3CH2_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
			{
				if((TIM3CH2_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
				{
					TIM3CH2_CAPTURE_STA|=0X80;//��ǳɹ�������һ��
					TIM3CH2_CAPTURE_VAL=0XFFFF;
				}else TIM3CH2_CAPTURE_STA++;
			}
		}
		if(tsr&0x04)//���� 4 ���������¼�
		{
			if(TIM3CH2_CAPTURE_STA&0X40) //����һ���½���
			{ 
				TIM3CH2_CAPTURE_STA|=0X80; //��ǳɹ�����һ�θߵ�ƽ����
				TIM3CH2_CAPTURE_VAL = TIM3->CCR2;//��ȡ��ǰ�Ĳ���ֵ.
				TIM3->CCER&=~(1<<5); //CC2P=0 ����Ϊ�����ز���
			}else //��δ��ʼ,��һ�β���������
			{
					TIM3CH2_CAPTURE_VAL=0;
					TIM3CH2_CAPTURE_STA=0X40; //��ǲ�����������
					TIM3->CNT=0; //���������
					TIM3->CCER|=1<<5; //CC4P=1 ����Ϊ�½��ز���
			} 
		} 
	}
//------------------------------------------------------------------
//T3-1		
		if((TIM3CH1_CAPTURE_STA&0X80)==0)//��δ�ɹ�����
	{
		if(tsr&0X10)//���
		{ 
			if(TIM3CH1_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
			{
				if((TIM3CH1_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
				{
					TIM3CH1_CAPTURE_STA|=0X80;//��ǳɹ�������һ��
					TIM3CH1_CAPTURE_VAL=0XFFFF;
				}else TIM3CH1_CAPTURE_STA++;
			}
		}
		if(tsr&0x02)//���� 4 ���������¼�
		{
			if(TIM3CH1_CAPTURE_STA&0X40) //����һ���½���
			{ 
				TIM3CH1_CAPTURE_STA|=0X80; //��ǳɹ�����һ�θߵ�ƽ����
				TIM3CH1_CAPTURE_VAL = TIM3->CCR1;//��ȡ��ǰ�Ĳ���ֵ.
				TIM3->CCER&=~(1<<1); //CC4P=0 ����Ϊ�����ز���
			}else //��δ��ʼ,��һ�β���������
			{
					TIM3CH1_CAPTURE_VAL=0;
					TIM3CH1_CAPTURE_STA=0X40; //��ǲ�����������
					TIM3->CNT=0; //���������
					TIM3->CCER|=1<<1; //CC4P=1 ����Ϊ�½��ز���
			} 
		} 
	}
	
//-----------------------------------------------------------------------------------	
	TIM3->SR=0;//����жϱ�־λ 
}


u8  TIM4CH1_CAPTURE_STA=0; //���벶��״̬ 
u16 TIM4CH1_CAPTURE_VAL;//���벶��ֵ
u8  TIM4CH2_CAPTURE_STA=0; //���벶��״̬ 
u16 TIM4CH2_CAPTURE_VAL;//���벶��ֵ
u8  TIM4CH3_CAPTURE_STA=0; //���벶��״̬ 
u16 TIM4CH3_CAPTURE_VAL;//���벶��ֵ
u8  TIM4CH4_CAPTURE_STA=0; //���벶��״̬ 
u16 TIM4CH4_CAPTURE_VAL;//���벶��ֵ


//��ʱ�� 3 �жϷ������
void TIM4_IRQHandler(void)
{ 
	u16 tsr;
	tsr=TIM4->SR;
//------------------------------------------------------------------
//T4-4	
	if((TIM4CH4_CAPTURE_STA&0X80)==0)//��δ�ɹ�����
	{
		if(tsr&0X01)//���
		{ 
			if(TIM4CH4_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
			{
				if((TIM4CH4_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
				{
					TIM4CH4_CAPTURE_STA|=0X80;//��ǳɹ�������һ��
					TIM4CH4_CAPTURE_VAL=0XFFFF;
				}else TIM4CH4_CAPTURE_STA++;
			}
		}
		if(tsr&0x10)//���� 4 ���������¼�
		{
			if(TIM4CH4_CAPTURE_STA&0X40) //����һ���½���
			{ 
				TIM4CH4_CAPTURE_STA|=0X80; //��ǳɹ�����һ�θߵ�ƽ����
				TIM4CH4_CAPTURE_VAL = TIM4->CCR4;//��ȡ��ǰ�Ĳ���ֵ.
				TIM4->CCER&=~(1<<13); //CC4P=0 ����Ϊ�����ز���
			}else //��δ��ʼ,��һ�β���������
			{
					TIM4CH4_CAPTURE_VAL=0;
					TIM4CH4_CAPTURE_STA=0X40; //��ǲ�����������
					TIM4->CNT=0; //���������
					TIM4->CCER|=1<<13; //CC4P=1 ����Ϊ�½��ز���
			} 
		} 
	}
//------------------------------------------------------------------
//T4-3		
		if((TIM4CH3_CAPTURE_STA&0X80)==0)//��δ�ɹ�����
	{
		if(tsr&0X01)//���
		{ 
			if(TIM4CH3_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
			{
				if((TIM4CH3_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
				{
					TIM4CH3_CAPTURE_STA|=0X80;//��ǳɹ�������һ��
					TIM4CH3_CAPTURE_VAL=0XFFFF;
				}else TIM4CH3_CAPTURE_STA++;
			}
		}
		if(tsr&0x08)//���� 4 ���������¼�
		{
			if(TIM4CH3_CAPTURE_STA&0X40) //����һ���½���
			{ 
				TIM4CH3_CAPTURE_STA|=0X80; //��ǳɹ�����һ�θߵ�ƽ����
				TIM4CH3_CAPTURE_VAL = TIM4->CCR3;//��ȡ��ǰ�Ĳ���ֵ.
				TIM4->CCER&=~(1<<9); //CC3P=0 ����Ϊ�����ز���
			}else //��δ��ʼ,��һ�β���������
			{
					TIM4CH3_CAPTURE_VAL=0;
					TIM4CH3_CAPTURE_STA=0X40; //��ǲ�����������
					TIM4->CNT=0; //���������
					TIM4->CCER|=1<<9; //CC4P=1 ����Ϊ�½��ز���
			} 
		} 
	}
//------------------------------------------------------------------
//T4-2		
		if((TIM4CH2_CAPTURE_STA&0X80)==0)//��δ�ɹ�����
	{
		if(tsr&0X01)//���
		{ 
			if(TIM4CH2_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
			{
				if((TIM4CH2_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
				{
					TIM4CH2_CAPTURE_STA|=0X80;//��ǳɹ�������һ��
					TIM4CH2_CAPTURE_VAL=0XFFFF;
				}else TIM4CH2_CAPTURE_STA++;
			}
		}
		if(tsr&0x04)//���� 4 ���������¼�
		{
			if(TIM4CH2_CAPTURE_STA&0X40) //����һ���½���
			{ 
				TIM4CH2_CAPTURE_STA|=0X80; //��ǳɹ�����һ�θߵ�ƽ����
				TIM4CH2_CAPTURE_VAL = TIM4->CCR2;//��ȡ��ǰ�Ĳ���ֵ.
				TIM4->CCER&=~(1<<5); //CC2P=0 ����Ϊ�����ز���
			}else //��δ��ʼ,��һ�β���������
			{
					TIM4CH2_CAPTURE_VAL=0;
					TIM4CH2_CAPTURE_STA=0X40; //��ǲ�����������
					TIM4->CNT=0; //���������
					TIM4->CCER|=1<<5; //CC4P=1 ����Ϊ�½��ز���
			} 
		} 
	}
//------------------------------------------------------------------
//T4-1		
		if((TIM4CH1_CAPTURE_STA&0X80)==0)//��δ�ɹ�����
	{
		if(tsr&0X10)//���
		{ 
			if(TIM4CH1_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
			{
				if((TIM4CH1_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
				{
					TIM4CH1_CAPTURE_STA|=0X80;//��ǳɹ�������һ��
					TIM4CH1_CAPTURE_VAL=0XFFFF;
				}else TIM4CH1_CAPTURE_STA++;
			}
		}
		if(tsr&0x02)//���� 4 ���������¼�
		{
			if(TIM4CH1_CAPTURE_STA&0X40) //����һ���½���
			{ 
				TIM4CH1_CAPTURE_STA|=0X80; //��ǳɹ�����һ�θߵ�ƽ����
				TIM4CH1_CAPTURE_VAL = TIM4->CCR1;//��ȡ��ǰ�Ĳ���ֵ.
				TIM4->CCER&=~(1<<1); //CC4P=0 ����Ϊ�����ز���
			}else //��δ��ʼ,��һ�β���������
			{
					TIM4CH1_CAPTURE_VAL=0;
					TIM4CH1_CAPTURE_STA=0X40; //��ǲ�����������
					TIM4->CNT=0; //���������
					TIM4->CCER|=1<<1; //CC4P=1 ����Ϊ�½��ز���
			} 
		} 
	}
	
//-----------------------------------------------------------------------------------	
	TIM4->SR=0;//����жϱ�־λ 
}

//--------------------------------------
u16 CH[12];//���񵽵�ֵ���ŵ�������
u16 CH_L[12];
void Catch_Cal()
{
	u16 temp;
			if(TIM3CH4_CAPTURE_STA&0X80)//�ɹ�������һ�θߵ�ƽ
		{
				temp=TIM3CH4_CAPTURE_STA&0X3F;
				temp*=65536; //���ʱ���ܺ�
				temp+=TIM3CH4_CAPTURE_VAL; //�õ��ܵĸߵ�ƽʱ��
				
				if(((temp-CH_L[0])<2500)||((CH_L[0]-temp)>2500))
				{
					CH[0]=temp;
					CH_L[0]=temp;
				}
			TIM3CH4_CAPTURE_STA=0;
		}
			if(TIM3CH3_CAPTURE_STA&0X80)//�ɹ�������һ�θߵ�ƽ
		{	
				temp=TIM3CH3_CAPTURE_STA&0X3F;
				temp*=65536; //���ʱ���ܺ�
				temp+=TIM3CH3_CAPTURE_VAL; //�õ��ܵĸߵ�ƽʱ��
			
				if(((temp-CH_L[1])<2500)||((CH_L[1]-temp)>2500))
				{
					CH[1]=temp;
					CH_L[1]=temp;
				}
				
			TIM3CH3_CAPTURE_STA=0;
		}
			if(TIM3CH2_CAPTURE_STA&0X80)//�ɹ�������һ�θߵ�ƽ
		{	
				temp=TIM3CH2_CAPTURE_STA&0X3F;
				temp*=65536; //���ʱ���ܺ�
				temp+=TIM3CH2_CAPTURE_VAL; //�õ��ܵĸߵ�ƽʱ��
							if(((temp-CH_L[2])<2500)||((CH_L[2]-temp)>2500))
				{
					CH[2]=temp;
					CH_L[2]=temp;
				}
				
			TIM3CH2_CAPTURE_STA=0;
		}
			if(TIM3CH1_CAPTURE_STA&0X80)//�ɹ�������һ�θߵ�ƽ
		{	
				temp=TIM3CH1_CAPTURE_STA&0X3F;
				temp*=65536; //���ʱ���ܺ�
				temp+=TIM3CH1_CAPTURE_VAL; //�õ��ܵĸߵ�ƽʱ��
							if(((temp-CH_L[3])<2500)||((CH_L[3]-temp)>2500))
				{
					CH[3]=temp;
					CH_L[3]=temp;
				}
				
			TIM3CH1_CAPTURE_STA=0;
		}
			if(TIM4CH1_CAPTURE_STA&0X80)//�ɹ�������һ�θߵ�ƽ
		{	
				temp=TIM4CH1_CAPTURE_STA&0X3F;
				temp*=65536; //���ʱ���ܺ�
				temp+=TIM4CH1_CAPTURE_VAL; //�õ��ܵĸߵ�ƽʱ��
							if(((temp-CH_L[4])<2500)||((CH_L[4]-temp)>2500))
				{
					CH[4]=temp;
					CH_L[4]=temp;
				}
				
			TIM4CH1_CAPTURE_STA=0;
		}
			if(TIM4CH2_CAPTURE_STA&0X80)//�ɹ�������һ�θߵ�ƽ
		{	
				temp=TIM4CH2_CAPTURE_STA&0X3F;
				temp*=65536; //���ʱ���ܺ�
				temp+=TIM4CH2_CAPTURE_VAL; //�õ��ܵĸߵ�ƽʱ��
							if(((temp-CH_L[5])<2500)||((CH_L[5]-temp)>2500))
				{
					CH[5]=temp;
					CH_L[5]=temp;
				}
				
			TIM4CH2_CAPTURE_STA=0;
		}
			if(TIM4CH3_CAPTURE_STA&0X80)//�ɹ�������һ�θߵ�ƽ
		{	
				temp=TIM4CH3_CAPTURE_STA&0X3F;
				temp*=65536; //���ʱ���ܺ�
				temp+=TIM4CH3_CAPTURE_VAL; //�õ��ܵĸߵ�ƽʱ��
				if(((temp-CH_L[6])<2500)||((CH_L[6]-temp)>2500))
				{
					CH[6]=temp;
					CH_L[6]=temp;
				}
				
			TIM4CH3_CAPTURE_STA=0;
		}
			if(TIM4CH4_CAPTURE_STA&0X80)//�ɹ�������һ�θߵ�ƽ
		{	
				temp=TIM4CH4_CAPTURE_STA&0X3F;
				temp*=65536; //���ʱ���ܺ�
				temp+=TIM4CH4_CAPTURE_VAL; //�õ��ܵĸߵ�ƽʱ��
							if(((temp-CH_L[7])<2500)||((CH_L[7]-temp)>2500))
				{
					CH[7]=temp;
					CH_L[7]=temp;
				}
				
			TIM4CH4_CAPTURE_STA=0;
		}
		
}

