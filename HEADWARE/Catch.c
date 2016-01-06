#include "Catch.h"


//-----------------------------------------------------
//功能：用于捕获遥控器的值
//T3_1(PA6);T3_2(PA7);T3_3(PB0);T3_4(PB1);
//T4_1(PB6);T4_2(PB7);T4_3(PB8);T4_4(PB9);
//by:The Best
//data:2015/12/1
//------------------------------------------------------

//--------------------------------
//对捕获进行初始化
//--------------------------------
void Catch_Init(u16 arr,u16 psc)
{
		RCC->APB1ENR |= 1<<1;	//开启定时器3时钟
		RCC->APB1ENR |= 1<<2;	//开启定时器4
	
		RCC->APB2ENR |= 1<<3;	//开启PB时钟
		RCC->APB2ENR |= 1<<2;
		

		GPIOB ->CRL  &=0x00ffff00;
		GPIOB ->CRL  |=0x88000088;   //PB0/1/6/7浮空输入
		GPIOB ->CRH  &=0xfffff00;
		GPIOB ->CRH  |=0x0000088;   //PB8/9浮空输入
		GPIOA ->CRL  &=0x00ffffff;
		GPIOA ->CRL  |=0x88000000;   //PA6/7浮空输入
			
		GPIOB ->ODR |=(0<<0)|(0<<1)|(0<<6)|(0<<7)|(0<<8)|(0<<9);	
		GPIOA ->ODR |=(0<<6)|(0<<7);

		TIM3->ARR=arr;				//TIM3预装值
		TIM3->PSC=psc;				//TIM3分频
	
		TIM3->CCMR1 |=1<<8|1<<0;
		TIM3->CCMR1 |=1<<12|1<<4; //IC4F=0001 配置滤波器 以 Fck_int 采样， 2 个事件后有效
		TIM3->CCMR1 |=0<<11|0<<3; //IC2PS=00 配置输入分频,不分频
		
		TIM3->CCMR2 |=1<<8|1<<0;
		TIM3->CCMR2 |=1<<12|1<<4; //IC4F=0001 配置滤波器 以 Fck_int 采样， 2 个事件后有效
		TIM3->CCMR2 |=0<<11|0<<3; //IC2PS=00 配置输入分频,不分频
		
		TIM4->ARR=arr;				//TIM4预装值
		TIM4->PSC=psc;				//TIM4分频
		
		TIM4->CCMR1 |=1<<8|1<<0;
		TIM4->CCMR1 |=1<<12|1<<4; //IC4F=0001 配置滤波器 以 Fck_int 采样， 2 个事件后有效
		TIM4->CCMR1 |=0<<11|0<<3; //IC2PS=00 配置输入分频,不分频
		
		TIM4->CCMR2 |=1<<8|1<<0;
		TIM4->CCMR2 |=1<<12|1<<4; //IC4F=0001 配置滤波器 以 Fck_int 采样， 2 个事件后有效
		TIM4->CCMR2 |=0<<11|0<<3; //IC2PS=00 配置输入分频,不分频
		
		
		TIM3->CCER|=0<<1; //CC1P=0 上升沿捕获
		TIM3->CCER|=1<<0; //CC1E=1 允许捕获计数器的值到捕获寄存器中
		TIM3->CCER|=0<<5; //CC2P=0 上升沿捕获
		TIM3->CCER|=1<<4; //CC2E=1 允许捕获计数器的值到捕获寄存器中
		TIM3->CCER|=0<<9; //CC3P=0 上升沿捕获
		TIM3->CCER|=1<<8; //CC3E=1 允许捕获计数器的值到捕获寄存器中
		TIM3->CCER|=0<<13; //CC4P=0 上升沿捕获
		TIM3->CCER|=1<<12; //CC4E=1 允许捕获计数器的值到捕获寄存器中
		
		TIM4->CCER|=0<<1; //CC1P=0 上升沿捕获
		TIM4->CCER|=1<<0; //CC1E=1 允许捕获计数器的值到捕获寄存器中
		TIM4->CCER|=0<<5; //CC2P=0 上升沿捕获
		TIM4->CCER|=1<<4; //CC2E=1 允许捕获计数器的值到捕获寄存器中
		TIM4->CCER|=0<<9; //CC3P=0 上升沿捕获
		TIM4->CCER|=1<<8; //CC3E=1 允许捕获计数器的值到捕获寄存器中
		TIM4->CCER|=0<<13; //CC4P=0 上升沿捕获
		TIM4->CCER|=1<<12; //CC4E=1 允许捕获计数器的值到捕获寄存器中
		
		
		TIM3->DIER|=(1<<4|1<<3|1<<2|1<<1); //允许捕获中断
		TIM3->DIER|=1<<0; //允许更新中断
		TIM3->CR1|=0x01; //使能定时器 3
		
		TIM4->DIER|=(1<<4|1<<3|1<<2|1<<1); //允许捕获中断
		TIM4->DIER|=1<<0; //允许更新中断
		TIM4->CR1|=0x01; //使能定时器 3
		
		
		MY_NVIC_Init(3,0,TIM3_IRQn,2);//抢占 2，子优先级 0，组 2
		MY_NVIC_Init(3,0,TIM4_IRQn,2);//抢占 2，子优先级 1，组 2
	
}


u8  TIM3CH1_CAPTURE_STA=0; //输入捕获状态 
u16 TIM3CH1_CAPTURE_VAL;//输入捕获值
u8  TIM3CH2_CAPTURE_STA=0; //输入捕获状态 
u16 TIM3CH2_CAPTURE_VAL;//输入捕获值
u8  TIM3CH3_CAPTURE_STA=0; //输入捕获状态 
u16 TIM3CH3_CAPTURE_VAL;//输入捕获值
u8  TIM3CH4_CAPTURE_STA=0; //输入捕获状态 
u16 TIM3CH4_CAPTURE_VAL;//输入捕获值


//定时器 3 中断服务程序
void TIM3_IRQHandler(void)
{ 
	u16 tsr;

	tsr=TIM3->SR;
//------------------------------------------------------------------
//T3-4	
	if((TIM3CH4_CAPTURE_STA&0X80)==0)//还未成功捕获
	{
		if(tsr&0X01)//溢出
		{ 
			if(TIM3CH4_CAPTURE_STA&0X40)//已经捕获到高电平了
			{
				if((TIM3CH4_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
				{
					TIM3CH4_CAPTURE_STA|=0X80;//标记成功捕获了一次
					TIM3CH4_CAPTURE_VAL=0XFFFF;
				}else TIM3CH4_CAPTURE_STA++;
			}
		}
		if(tsr&0x10)//捕获 4 发生捕获事件
		{
			if(TIM3CH4_CAPTURE_STA&0X40) //捕获到一个下降沿
			{ 
				TIM3CH4_CAPTURE_STA|=0X80; //标记成功捕获到一次高电平脉宽
				TIM3CH4_CAPTURE_VAL = TIM3->CCR4;//获取当前的捕获值.
				TIM3->CCER&=~(1<<13); //CC4P=0 设置为上升沿捕获
			}else //还未开始,第一次捕获上升沿
			{
					TIM3CH4_CAPTURE_VAL=0;
					TIM3CH4_CAPTURE_STA=0X40; //标记捕获到了上升沿
					TIM3->CNT=0; //计数器清空
					TIM3->CCER|=1<<13; //CC4P=1 设置为下降沿捕获
			} 
		} 
	}
//------------------------------------------------------------------
//T3-3		
		if((TIM3CH3_CAPTURE_STA&0X80)==0)//还未成功捕获
	{
		if(tsr&0X01)//溢出
		{ 
			if(TIM3CH3_CAPTURE_STA&0X40)//已经捕获到高电平了
			{
				if((TIM3CH3_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
				{
					TIM3CH3_CAPTURE_STA|=0X80;//标记成功捕获了一次
					TIM3CH3_CAPTURE_VAL=0XFFFF;
				}else TIM3CH3_CAPTURE_STA++;
			}
		}
		if(tsr&0x08)//捕获 4 发生捕获事件
		{
			if(TIM3CH3_CAPTURE_STA&0X40) //捕获到一个下降沿
			{ 
				TIM3CH3_CAPTURE_STA|=0X80; //标记成功捕获到一次高电平脉宽
				TIM3CH3_CAPTURE_VAL = TIM3->CCR3;//获取当前的捕获值.
				TIM3->CCER&=~(1<<9); //CC3P=0 设置为上升沿捕获
			}else //还未开始,第一次捕获上升沿
			{
					TIM3CH3_CAPTURE_VAL=0;
					TIM3CH3_CAPTURE_STA=0X40; //标记捕获到了上升沿
					TIM3->CNT=0; //计数器清空
					TIM3->CCER|=1<<9; //CC4P=1 设置为下降沿捕获
			} 
		} 
	}
//------------------------------------------------------------------
//T3-2		
		if((TIM3CH2_CAPTURE_STA&0X80)==0)//还未成功捕获
	{
		if(tsr&0X01)//溢出
		{ 
			if(TIM3CH2_CAPTURE_STA&0X40)//已经捕获到高电平了
			{
				if((TIM3CH2_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
				{
					TIM3CH2_CAPTURE_STA|=0X80;//标记成功捕获了一次
					TIM3CH2_CAPTURE_VAL=0XFFFF;
				}else TIM3CH2_CAPTURE_STA++;
			}
		}
		if(tsr&0x04)//捕获 4 发生捕获事件
		{
			if(TIM3CH2_CAPTURE_STA&0X40) //捕获到一个下降沿
			{ 
				TIM3CH2_CAPTURE_STA|=0X80; //标记成功捕获到一次高电平脉宽
				TIM3CH2_CAPTURE_VAL = TIM3->CCR2;//获取当前的捕获值.
				TIM3->CCER&=~(1<<5); //CC2P=0 设置为上升沿捕获
			}else //还未开始,第一次捕获上升沿
			{
					TIM3CH2_CAPTURE_VAL=0;
					TIM3CH2_CAPTURE_STA=0X40; //标记捕获到了上升沿
					TIM3->CNT=0; //计数器清空
					TIM3->CCER|=1<<5; //CC4P=1 设置为下降沿捕获
			} 
		} 
	}
//------------------------------------------------------------------
//T3-1		
		if((TIM3CH1_CAPTURE_STA&0X80)==0)//还未成功捕获
	{
		if(tsr&0X10)//溢出
		{ 
			if(TIM3CH1_CAPTURE_STA&0X40)//已经捕获到高电平了
			{
				if((TIM3CH1_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
				{
					TIM3CH1_CAPTURE_STA|=0X80;//标记成功捕获了一次
					TIM3CH1_CAPTURE_VAL=0XFFFF;
				}else TIM3CH1_CAPTURE_STA++;
			}
		}
		if(tsr&0x02)//捕获 4 发生捕获事件
		{
			if(TIM3CH1_CAPTURE_STA&0X40) //捕获到一个下降沿
			{ 
				TIM3CH1_CAPTURE_STA|=0X80; //标记成功捕获到一次高电平脉宽
				TIM3CH1_CAPTURE_VAL = TIM3->CCR1;//获取当前的捕获值.
				TIM3->CCER&=~(1<<1); //CC4P=0 设置为上升沿捕获
			}else //还未开始,第一次捕获上升沿
			{
					TIM3CH1_CAPTURE_VAL=0;
					TIM3CH1_CAPTURE_STA=0X40; //标记捕获到了上升沿
					TIM3->CNT=0; //计数器清空
					TIM3->CCER|=1<<1; //CC4P=1 设置为下降沿捕获
			} 
		} 
	}
	
//-----------------------------------------------------------------------------------	
	TIM3->SR=0;//清除中断标志位 
}


u8  TIM4CH1_CAPTURE_STA=0; //输入捕获状态 
u16 TIM4CH1_CAPTURE_VAL;//输入捕获值
u8  TIM4CH2_CAPTURE_STA=0; //输入捕获状态 
u16 TIM4CH2_CAPTURE_VAL;//输入捕获值
u8  TIM4CH3_CAPTURE_STA=0; //输入捕获状态 
u16 TIM4CH3_CAPTURE_VAL;//输入捕获值
u8  TIM4CH4_CAPTURE_STA=0; //输入捕获状态 
u16 TIM4CH4_CAPTURE_VAL;//输入捕获值


//定时器 3 中断服务程序
void TIM4_IRQHandler(void)
{ 
	u16 tsr;
	tsr=TIM4->SR;
//------------------------------------------------------------------
//T4-4	
	if((TIM4CH4_CAPTURE_STA&0X80)==0)//还未成功捕获
	{
		if(tsr&0X01)//溢出
		{ 
			if(TIM4CH4_CAPTURE_STA&0X40)//已经捕获到高电平了
			{
				if((TIM4CH4_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
				{
					TIM4CH4_CAPTURE_STA|=0X80;//标记成功捕获了一次
					TIM4CH4_CAPTURE_VAL=0XFFFF;
				}else TIM4CH4_CAPTURE_STA++;
			}
		}
		if(tsr&0x10)//捕获 4 发生捕获事件
		{
			if(TIM4CH4_CAPTURE_STA&0X40) //捕获到一个下降沿
			{ 
				TIM4CH4_CAPTURE_STA|=0X80; //标记成功捕获到一次高电平脉宽
				TIM4CH4_CAPTURE_VAL = TIM4->CCR4;//获取当前的捕获值.
				TIM4->CCER&=~(1<<13); //CC4P=0 设置为上升沿捕获
			}else //还未开始,第一次捕获上升沿
			{
					TIM4CH4_CAPTURE_VAL=0;
					TIM4CH4_CAPTURE_STA=0X40; //标记捕获到了上升沿
					TIM4->CNT=0; //计数器清空
					TIM4->CCER|=1<<13; //CC4P=1 设置为下降沿捕获
			} 
		} 
	}
//------------------------------------------------------------------
//T4-3		
		if((TIM4CH3_CAPTURE_STA&0X80)==0)//还未成功捕获
	{
		if(tsr&0X01)//溢出
		{ 
			if(TIM4CH3_CAPTURE_STA&0X40)//已经捕获到高电平了
			{
				if((TIM4CH3_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
				{
					TIM4CH3_CAPTURE_STA|=0X80;//标记成功捕获了一次
					TIM4CH3_CAPTURE_VAL=0XFFFF;
				}else TIM4CH3_CAPTURE_STA++;
			}
		}
		if(tsr&0x08)//捕获 4 发生捕获事件
		{
			if(TIM4CH3_CAPTURE_STA&0X40) //捕获到一个下降沿
			{ 
				TIM4CH3_CAPTURE_STA|=0X80; //标记成功捕获到一次高电平脉宽
				TIM4CH3_CAPTURE_VAL = TIM4->CCR3;//获取当前的捕获值.
				TIM4->CCER&=~(1<<9); //CC3P=0 设置为上升沿捕获
			}else //还未开始,第一次捕获上升沿
			{
					TIM4CH3_CAPTURE_VAL=0;
					TIM4CH3_CAPTURE_STA=0X40; //标记捕获到了上升沿
					TIM4->CNT=0; //计数器清空
					TIM4->CCER|=1<<9; //CC4P=1 设置为下降沿捕获
			} 
		} 
	}
//------------------------------------------------------------------
//T4-2		
		if((TIM4CH2_CAPTURE_STA&0X80)==0)//还未成功捕获
	{
		if(tsr&0X01)//溢出
		{ 
			if(TIM4CH2_CAPTURE_STA&0X40)//已经捕获到高电平了
			{
				if((TIM4CH2_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
				{
					TIM4CH2_CAPTURE_STA|=0X80;//标记成功捕获了一次
					TIM4CH2_CAPTURE_VAL=0XFFFF;
				}else TIM4CH2_CAPTURE_STA++;
			}
		}
		if(tsr&0x04)//捕获 4 发生捕获事件
		{
			if(TIM4CH2_CAPTURE_STA&0X40) //捕获到一个下降沿
			{ 
				TIM4CH2_CAPTURE_STA|=0X80; //标记成功捕获到一次高电平脉宽
				TIM4CH2_CAPTURE_VAL = TIM4->CCR2;//获取当前的捕获值.
				TIM4->CCER&=~(1<<5); //CC2P=0 设置为上升沿捕获
			}else //还未开始,第一次捕获上升沿
			{
					TIM4CH2_CAPTURE_VAL=0;
					TIM4CH2_CAPTURE_STA=0X40; //标记捕获到了上升沿
					TIM4->CNT=0; //计数器清空
					TIM4->CCER|=1<<5; //CC4P=1 设置为下降沿捕获
			} 
		} 
	}
//------------------------------------------------------------------
//T4-1		
		if((TIM4CH1_CAPTURE_STA&0X80)==0)//还未成功捕获
	{
		if(tsr&0X10)//溢出
		{ 
			if(TIM4CH1_CAPTURE_STA&0X40)//已经捕获到高电平了
			{
				if((TIM4CH1_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
				{
					TIM4CH1_CAPTURE_STA|=0X80;//标记成功捕获了一次
					TIM4CH1_CAPTURE_VAL=0XFFFF;
				}else TIM4CH1_CAPTURE_STA++;
			}
		}
		if(tsr&0x02)//捕获 4 发生捕获事件
		{
			if(TIM4CH1_CAPTURE_STA&0X40) //捕获到一个下降沿
			{ 
				TIM4CH1_CAPTURE_STA|=0X80; //标记成功捕获到一次高电平脉宽
				TIM4CH1_CAPTURE_VAL = TIM4->CCR1;//获取当前的捕获值.
				TIM4->CCER&=~(1<<1); //CC4P=0 设置为上升沿捕获
			}else //还未开始,第一次捕获上升沿
			{
					TIM4CH1_CAPTURE_VAL=0;
					TIM4CH1_CAPTURE_STA=0X40; //标记捕获到了上升沿
					TIM4->CNT=0; //计数器清空
					TIM4->CCER|=1<<1; //CC4P=1 设置为下降沿捕获
			} 
		} 
	}
	
//-----------------------------------------------------------------------------------	
	TIM4->SR=0;//清除中断标志位 
}

//--------------------------------------
u16 CH[12];//捕获到的值将放到这里面
u16 CH_L[12];
void Catch_Cal()
{
	u16 temp;
			if(TIM3CH4_CAPTURE_STA&0X80)//成功捕获到了一次高电平
		{
				temp=TIM3CH4_CAPTURE_STA&0X3F;
				temp*=65536; //溢出时间总和
				temp+=TIM3CH4_CAPTURE_VAL; //得到总的高电平时间
				
				if(((temp-CH_L[0])<2500)||((CH_L[0]-temp)>2500))
				{
					CH[0]=temp;
					CH_L[0]=temp;
				}
			TIM3CH4_CAPTURE_STA=0;
		}
			if(TIM3CH3_CAPTURE_STA&0X80)//成功捕获到了一次高电平
		{	
				temp=TIM3CH3_CAPTURE_STA&0X3F;
				temp*=65536; //溢出时间总和
				temp+=TIM3CH3_CAPTURE_VAL; //得到总的高电平时间
			
				if(((temp-CH_L[1])<2500)||((CH_L[1]-temp)>2500))
				{
					CH[1]=temp;
					CH_L[1]=temp;
				}
				
			TIM3CH3_CAPTURE_STA=0;
		}
			if(TIM3CH2_CAPTURE_STA&0X80)//成功捕获到了一次高电平
		{	
				temp=TIM3CH2_CAPTURE_STA&0X3F;
				temp*=65536; //溢出时间总和
				temp+=TIM3CH2_CAPTURE_VAL; //得到总的高电平时间
							if(((temp-CH_L[2])<2500)||((CH_L[2]-temp)>2500))
				{
					CH[2]=temp;
					CH_L[2]=temp;
				}
				
			TIM3CH2_CAPTURE_STA=0;
		}
			if(TIM3CH1_CAPTURE_STA&0X80)//成功捕获到了一次高电平
		{	
				temp=TIM3CH1_CAPTURE_STA&0X3F;
				temp*=65536; //溢出时间总和
				temp+=TIM3CH1_CAPTURE_VAL; //得到总的高电平时间
							if(((temp-CH_L[3])<2500)||((CH_L[3]-temp)>2500))
				{
					CH[3]=temp;
					CH_L[3]=temp;
				}
				
			TIM3CH1_CAPTURE_STA=0;
		}
			if(TIM4CH1_CAPTURE_STA&0X80)//成功捕获到了一次高电平
		{	
				temp=TIM4CH1_CAPTURE_STA&0X3F;
				temp*=65536; //溢出时间总和
				temp+=TIM4CH1_CAPTURE_VAL; //得到总的高电平时间
							if(((temp-CH_L[4])<2500)||((CH_L[4]-temp)>2500))
				{
					CH[4]=temp;
					CH_L[4]=temp;
				}
				
			TIM4CH1_CAPTURE_STA=0;
		}
			if(TIM4CH2_CAPTURE_STA&0X80)//成功捕获到了一次高电平
		{	
				temp=TIM4CH2_CAPTURE_STA&0X3F;
				temp*=65536; //溢出时间总和
				temp+=TIM4CH2_CAPTURE_VAL; //得到总的高电平时间
							if(((temp-CH_L[5])<2500)||((CH_L[5]-temp)>2500))
				{
					CH[5]=temp;
					CH_L[5]=temp;
				}
				
			TIM4CH2_CAPTURE_STA=0;
		}
			if(TIM4CH3_CAPTURE_STA&0X80)//成功捕获到了一次高电平
		{	
				temp=TIM4CH3_CAPTURE_STA&0X3F;
				temp*=65536; //溢出时间总和
				temp+=TIM4CH3_CAPTURE_VAL; //得到总的高电平时间
				if(((temp-CH_L[6])<2500)||((CH_L[6]-temp)>2500))
				{
					CH[6]=temp;
					CH_L[6]=temp;
				}
				
			TIM4CH3_CAPTURE_STA=0;
		}
			if(TIM4CH4_CAPTURE_STA&0X80)//成功捕获到了一次高电平
		{	
				temp=TIM4CH4_CAPTURE_STA&0X3F;
				temp*=65536; //溢出时间总和
				temp+=TIM4CH4_CAPTURE_VAL; //得到总的高电平时间
							if(((temp-CH_L[7])<2500)||((CH_L[7]-temp)>2500))
				{
					CH[7]=temp;
					CH_L[7]=temp;
				}
				
			TIM4CH4_CAPTURE_STA=0;
		}
		
}

