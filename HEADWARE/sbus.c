#include "sbus.h"

//=======================================================
//sbus信号产生说明
//用串口1来发送数据，具体帧的格式见下面
//定时器2来定时发送，周期为14ms
//by：The Best
//data:2015/11/30
//=======================================================


//------------------------------------------------------
//用于产生SBUS信号的串口初始化
//串口配置为波特率100kbps，8位数据，偶校验(even)，2位停止位，无流控
//初始化IO 串口1
//pclk2:PCLK2时钟频率(Mhz)
//bound:波特率
//CHECK OK
//注意：STM32的数据位是包括校验位的，所以，当我设置为USART_WordLength_8b的时候，我的数据位实际上就7位，校验位1位，所以正确的设置应该为9位，希望大家不要和我犯一样的错误。
void SBUS_Usart_Init()
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(72*1000000)/(100000*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   //使能PORTA口时钟  
	RCC->APB2ENR|=1<<14;  //使能串口时钟 
	GPIOA->CRH&=0XFFFFF00F;//IO状态设置
	GPIOA->CRH|=0X000008B0;//IO状态设置
		  
	RCC->APB2RSTR|=1<<14;   //复位串口1
	RCC->APB2RSTR&=~(1<<14);//停止复位	   	   
	//波特率设置
 	USART1->BRR=mantissa; // 波特率设置	 
	USART1->CR1 |=0X340C;  //2位停止,偶校验位，8位数据位，物流控.
	USART1->CR2 |= 1<<13; 	//2位停止位
//	USART1->CR1|=0X200C;
//#if EN_USART1_RX		  //如果使能了接收
//	//使能接收中断
	USART1->CR1|=1<<8;    //PE中断使能
	USART1->CR1|=1<<5;    //接收缓冲区非空中断使能	    	
	MY_NVIC_Init(2,1,USART1_IRQn,2);//组2，最低优先级 
//#endif


}

//用于定时发送sbus数据，定时器2的初始化
//14ms发送一次
//arr=71;psc=999每帧间隔1ms
//SBUS_Time_Init(71,999);
void SBUS_Time_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR |=1<<0;
	
	TIM2 ->ARR = arr;
	TIM2 ->PSC =psc;
	TIM2 ->DIER |=1<<0;
//	TIM2 ->CR1 |=0x01;
	
	MY_NVIC_Init(1,3,TIM2_IRQn,2);
}

//开始发送SBUS
void SBUS_Send_Start()
{
	TIM2 ->CR1 |=0x01;
}
//结束发送SBUS
void SBUS_Send_End()
{
	TIM2 ->CR1 |=0x00;
}

//定时发送中断
u16 data1[16]={1500,190,200};
u16 data2[16]={500,190,200};
u8 flag_14ms=0;
u8 flag_6ms=0;
//u8 R_data[25]={0x0f,0x00,0x24,0x20,0x58,0xfa,0x47,0x36,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
void TIM2_IRQHandler(void)
{
	u8 i;
	if(TIM2->SR & 0x0001)	
	{
		flag_14ms++;
		flag_6ms++;
		Catch_Cal();
		if(flag_14ms==14)
		{
			flag_14ms=0;
			i++;
//			SBUS_Convert(R_data);
			if(i<100)
			{SBUS_Send_Data(data1);}
			else if(i>101)
			SBUS_Send_Data(data2);
			
		}
		
	}
	
	TIM2->SR &=~(1<<0);	//清除中断
}

//发送单个数据
void SBUS_SendOne(u8 ch)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      

}


//发送一帧数据;
//sbus帧格式;
//startbyte=0x0f;
//endbyte=0x00;
//[startbyte] [data1] [data2] .... [data22] [flags][endbyte];

unsigned char SBUS_Data[22];
unsigned char SBUS_Flag;
unsigned char flagbit=0x00;
void SBUS_Send_Data(u16 *ch)
{
	unsigned char i,k=15;	
    char n = 21;
    char m = 0;
	char j=8;
//------------------------------------------------	
    for (i=0;i<176;i++) 
    {
		if(((ch[k])&(1<<(10-m))))
		{
			SBUS_Data[n] |= (1<<(j-1));
        }
        m++; 
		j--;
		
		if(j==0) 
		{
			j=8;
			n--;
		}		
        if(m == 11)
        {
            m = 0;
			k--;
        }
     
    }

//------------------------------------------------	
	SBUS_SendOne(Startbit);
	for(j=0;j<22;j++)
	{
		
		SBUS_SendOne(SBUS_Data[j]);
	}
	SBUS_SendOne(flagbit);
	SBUS_SendOne(Endbit);
}

u8 R_data[25];
u16 res=0;	
//void USART1_IRQHandler(void)
//{

//	if(USART1->SR&(1<<5))//接收到数据
//	{	 
//		if(flag_6ms>9)	
//		{
//			res=0;flag_6ms=0;
//			SBUS_Convert(R_data);
//		}
//		R_data[res]=USART1->DR; 
//			res++;
//	}
//	
//	
//}



//-----------------------------------------------------------
//sbus数据的读取
//-----------------------------------------------------------
u8 failsafe_status,byte_in_sbus,bit_in_sbus,ch,bit_in_channel;
u16 channel_sbus[20];
u8 sbus_data[30];
u8 SBUS_IN_OK;

u8 SBUS_Convert(u8 *data_in)

{ 
	
		u8 i;
	byte_in_sbus = 1;
	bit_in_sbus = 0;
	ch = 0;
	bit_in_channel = 0;
	
	if ((data_in[0]==0x0F) && (data_in[24] == 0)) 

	{

			// clear channel_sbus[]

//			for (i=0; i<16; i++) {channel_sbus[i] = 0;}
			// reset counters

			// process actual sbus data
			for (i=0; i<176; i++) 
			{
				if (data_in[byte_in_sbus] & (1 << bit_in_sbus))
				{
					channel_sbus[ch] |= (1 << bit_in_channel);
				}
				
				
				bit_in_sbus++;
				bit_in_channel++;
				
				
				if (bit_in_sbus == 8) 
				{
					bit_in_sbus = 0;
					byte_in_sbus++;
				}
				if (bit_in_channel == 11)
				{
					bit_in_channel = 0;
					ch++;
				}
				
				
			}
			// DigiChannel 1
			if (data_in[23] & (1<<0)) 
			{
				channel_sbus[16] = 1;
			}
			else
			{
				channel_sbus[16] = 0;
			}
			// DigiChannel 2
			if (data_in[23] & (1<<1)) 
			{
				channel_sbus[17] = 1;
			}
			else
			{
				channel_sbus[17] = 0;
			}
			// check radio signal
				failsafe_status = SBUS_SIGNAL_OK;
			if (data_in[23] & (1<<2)) 
			{
				failsafe_status = SBUS_SIGNAL_LOST;
			}
			if (data_in[23] & (1<<3)) 
			{
				failsafe_status = SBUS_SIGNAL_FAILSAFE;
			}
			return(0);
		}
		else
			{return(0xFF);
		}
}

//---------------------------------------------------------------------