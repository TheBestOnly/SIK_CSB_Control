#ifndef __CAN_H__
#define __CAN_H__

#include "sys.h"
#include "led.h"


										 							 				    
u8 CAN1_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);//CAN初始化
u8 Can_Tx_Msg(u32 id,u8 ide,u8 rtr,u8 len,u8 *dat);		//发送数据
u8 Can_Msg_Pend(u8 fifox);								//查询邮箱报文
void Can_Rx_Msg(u8 fifox,u32 *id,u8 *ide,u8 *rtr,u8 *len,u8 *dat);//接收数据
u8 Can_Tx_Staus(u8 mbox);  								//返回发送状态
u8 Can_Send_Msg(u8* msg,u8 len);						//发送数据
u8 Can_Receive_Msg(u8 *buf);							//接收数据

#endif
