/**
  ******************************************************************************
  * @file    serial.c
  * @author  xiaoyuan
  * @version V2.0
  * @date    2018-1-28
  * @brief   串口中断接收，并将接收到的数据存入环形缓冲区，并实现入队、出队，溢出检测功能
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

#include "serial.h"
#include "usart.h"

///* 定义环形缓冲区（循环队列），这里环形缓冲区中的head tail 和数据结构书循环队列定义的front rear 所指的位置恰好相反 */
//RingBufferStruct rx_buffer  =  { { 0 }, 0, 0 };
////ReceiveDataToBuffer
//void ReceiveCharToBuff(unsigned char byte)  //将接收到的数据存入缓冲区
//{
//  int i = (unsigned int)(rx_buffer.head + 1) % RX_BUFFER_SIZE;
//  // if we should be storing the received character into the location  //如果我们应该存储的接收到的字符的位置刚好
//  // just before the tail (meaning that the head would advance to the  //在尾端的前面（意味着头部将要进入尾端的
//  // current location of the tail), we're about to overflow the buffer //当前位置），这样将会溢出缓冲区，因此我们
//  // and so we don't write the character or advance the head.          //不该存入这个字符或使这个头前进
//  if (i != rx_buffer.tail)  //缓冲区没有存满 
//	{
//    rx_buffer.buffer[rx_buffer.head] = byte;
//    rx_buffer.head = i;
//  }
//}

//uint8_t ReadCharFromBuff(void)  //按存入顺序逐个读取缓冲区的数据，并删除
//{ 
//	uint8_t c;
//  // if the head isn't ahead of the tail, we don't have any characters //如果头不是在尾的前面，将收不到任何字符
//  if (rx_buffer.head == rx_buffer.tail) 
//	{
//    return 0;
//  } 
//	else 
//	{
//    c = rx_buffer.buffer[rx_buffer.tail];
//    rx_buffer.tail = (unsigned int)(rx_buffer.tail + 1) % RX_BUFFER_SIZE;
//    return c;
//  }
//}

//unsigned int GetRxBuffLen(void)  //返回串口缓存区中数据的个数
//{
//   return (unsigned int)(RX_BUFFER_SIZE + rx_buffer.head - rx_buffer.tail) % RX_BUFFER_SIZE;
//}

//void CheckRxBuff(void)  //检测接收到的数据
//{ 
//	unsigned char c;
//	unsigned int i;
//	
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) 
//  { 
//		c = USART_ReceiveData(USART1);
//		i = (unsigned int)(rx_buffer.head + 1) % RX_BUFFER_SIZE;
//		// if we should be storing the received character into the location
//		// just before the tail (meaning that the head would advance to the
//		// current location of the tail), we're about to overflow the buffer
//		// and so we don't write the character or advance the head.
//		if (i != rx_buffer.tail) 
//    {
//			rx_buffer.buffer[rx_buffer.head] = c;
//			rx_buffer.head = i;
//		}
//	}
//}

////uint8_t MSerial_peek(void) //读出缓冲区的数据，不删除
////{
////    if (rx_buffer.head == rx_buffer.tail) 
////    {
////        return 0;
////    } 
////    else 
////    {
////        return rx_buffer.buffer[rx_buffer.tail];
////    }
////}


////void MSerial_flush(void)  //等待串口数据传送完毕
////{
////  // RX
////	// don't reverse this or there may be problems if the RX interrupt  //不要颠倒这个否则可能会有一些问题，如果接收中断
////  // occurs after reading the value of rx_buffer_head but before writing  //发生在读取rx_buffer_head之后但在写入rx_buffer_tail之前
////  // the value to rx_buffer_tail; the previous value of rx_buffer_head  //之前的rx_buffer_head值可能被写到rx_buffer_tail
////  // may be written to rx_buffer_tail, making it appear as if the buffer  //使它呈现缓冲区是满的而非空的状态
////  // were full, not empty.
////  rx_buffer.head = rx_buffer.tail;
////}


////串口1中断服务程序
////注意,读取USARTx->SR能避免莫名其妙的错误   	
////u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
////接收状态
////bit15，	接收完成标志
////bit14，	接收到0x0d
////bit13~0，	接收到的有效字节数目
//void USART1_IRQHandler(void)                	//串口1中断服务程序
//{
//	uint8_t rec;
//	
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d //0x0a结尾)
//	{
//		rec = USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
////		printf("%c",rec);  //测试接收
//		ReceiveCharToBuff(rec);  		//将读取到的数据存入缓存区 
//		
//  } 
//} 

ring_buffer rx_buffer  =  { { 0 }, 0, 0 };

/*static inline*/ void store_char(unsigned char c)  //将接收到的数据存入缓冲区
{
  int i = (unsigned int)(rx_buffer.head + 1) % RX_BUFFER_SIZE;
  // if we should be storing the received character into the location  //如果我们应该存储的接收到的字符的位置刚好
  // just before the tail (meaning that the head would advance to the  //在尾端的前面（意味着头部将要进入尾端的
  // current location of the tail), we're about to overflow the buffer //当前位置），这样将会溢出缓冲区，因此我们
  // and so we don't write the character or advance the head.          //不该存入这个字符或使这个头前进
  if (i != rx_buffer.tail)  //缓冲区没有存满 
	{
    rx_buffer.buffer[rx_buffer.head] = c;
    rx_buffer.head = i;
  }
}

void MYSERIAL_checkRx(void)  //检测接收到的数据
{ 
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //接收中断
	{ 
		unsigned char c = USART_ReceiveData(USART1);
		unsigned int i = (unsigned int)(rx_buffer.head + 1) % RX_BUFFER_SIZE;
		// if we should be storing the received character into the location
		// just before the tail (meaning that the head would advance to the
		// current location of the tail), we're about to overflow the buffer
		// and so we don't write the character or advance the head.
		if (i != rx_buffer.tail) 
		{
			rx_buffer.buffer[rx_buffer.head] = c;
			rx_buffer.head = i;
		}
	}
}

unsigned int MYSERIAL_available(void)  //返回串口缓存区中数据的个数
{
   return (unsigned int)(RX_BUFFER_SIZE + rx_buffer.head - rx_buffer.tail) % RX_BUFFER_SIZE;
}

int MYSERIAL_read(void)  //按存入顺序逐个读取缓冲区的数据
{ 
	uint8_t c;
  // if the head isn't ahead of the tail, we don't have any characters //如果头不是在尾的前面，将收不到任何字符
  if (rx_buffer.head == rx_buffer.tail) 
	{
    return -1;
  } 
	else 
	{
    c = rx_buffer.buffer[rx_buffer.tail];
    rx_buffer.tail = (unsigned int)(rx_buffer.tail + 1) % RX_BUFFER_SIZE;
    return c;
  }
}
void MYSERIAL_flush(void)  //清空串口数据
{
  // RX
	// don't reverse this or there may be problems if the RX interrupt  //不要颠倒这个否则可能会有一些问题，如果接收中断
  // occurs after reading the value of rx_buffer_head but before writing  //发生在读取rx_buffer_head之后但在写入rx_buffer_tail之前
  // the value to rx_buffer_tail; the previous value of rx_buffer_head  //之前的rx_buffer_head值可能被写到rx_buffer_tail
  // may be written to rx_buffer_tail, making it appear as if the buffer  //使它呈现缓冲区是满的而非空的状态
  // were full, not empty.
  rx_buffer.head = rx_buffer.tail;
}		



#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
//u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	uint8_t rec;
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d //0x0a结尾)
	{
		rec = USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
		store_char(rec);  		//将读取到的数据存入缓存区 
  } 
} 

#endif

