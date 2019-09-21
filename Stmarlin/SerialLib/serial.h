/**
  ******************************************************************************
  * @file    usart.h
  * @author  xiaoyuan
  * @version V2.0
  * @date    2018-1-28
  * @brief   提供了USART模块函数的声明
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

#ifndef __SERIAL_H
#define __SERIAL_H

#include "stm32f10x.h"


//#define RX_BUFFER_SIZE 128  //定义接收缓冲区的大小

//typedef struct ring_buffer
//{
//  unsigned char buffer[RX_BUFFER_SIZE];
//  int head;
//  int tail;
//}RingBufferStruct; //ring_buffer;


//extern  RingBufferStruct rx_buffer;	 


//void ReceiveCharToBuff(unsigned char byte);
//uint8_t ReadCharFromBuff(void);
//unsigned int GetRxBuffLen(void);

#define RX_BUFFER_SIZE 128  //定义接收缓冲区的大小

typedef struct ring_buffer
{
  unsigned char buffer[RX_BUFFER_SIZE];
  int head;
  int tail;
}ring_buffer;

extern  ring_buffer rx_buffer;	 
extern uint16_t USART_RX_STA;         		//接收状态标记	


void MYSERIAL_checkRx(void);
unsigned int MYSERIAL_available(void);
int MYSERIAL_read(void);
void MYSERIAL_flush(void);

#endif //__SERIAL_H


