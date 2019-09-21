#ifndef __FASTIO_H
#define __FASTIO_H


/*普通IO端口的定义*/
#define PA4   GPIOA,GPIO_Pin_4
#define PA5   GPIOA,GPIO_Pin_5
#define PA6   GPIOA,GPIO_Pin_6
#define PA7   GPIOA,GPIO_Pin_7

#define PB0   GPIOB,GPIO_Pin_0
#define PB1   GPIOB,GPIO_Pin_1

#define PC4   GPIOC,GPIO_Pin_4
#define PC5   GPIOC,GPIO_Pin_5

#define PE3   GPIOE,GPIO_Pin_3
#define PE4   GPIOE,GPIO_Pin_4
#define PE5   GPIOE,GPIO_Pin_5
#define PE6   GPIOE,GPIO_Pin_6

#define PF0   GPIOF,GPIO_Pin_0
#define PF1   GPIOF,GPIO_Pin_1
#define PF2   GPIOF,GPIO_Pin_2
#define PF3   GPIOF,GPIO_Pin_3
#define PF4   GPIOF,GPIO_Pin_4
#define PF5   GPIOF,GPIO_Pin_5
#define PF6   GPIOF,GPIO_Pin_6
#define PF7   GPIOF,GPIO_Pin_7
#define PF8   GPIOF,GPIO_Pin_8
#define PF9   GPIOF,GPIO_Pin_9
#define PF10  GPIOF,GPIO_Pin_10
#define PF11  GPIOF,GPIO_Pin_11
#define PF12  GPIOF,GPIO_Pin_12
#define PF13  GPIOF,GPIO_Pin_13
#define PF14  GPIOF,GPIO_Pin_14
#define PF15  GPIOF,GPIO_Pin_15

#define WRITE(IO,v)   do{if(0 == v) GPIO_ResetBits(IO); else if(1 == v) GPIO_SetBits(IO);}while(0)//GPIO_WriteBit(IO,(BitAction)v)  //要提前将该GPIO端口初始化为输出模式
//#define READ_OUTPUT(IO)   GPIO_ReadOutputDataBit(IO)  //要提前将该GPIO端口初始化为输出模式

#define READ(IO)  GPIO_ReadInputDataBit(IO)  //要提前将该GPIO端口初始化为输入模式


/* ADC端口定义 */
#define PC0  ADC3,ADC_Channel_10
#define PC1  ADC3,ADC_Channel_11
#define PC2  ADC3,ADC_Channel_12
#define PC3  ADC3,ADC_Channel_13

#define GET_ADC(IO)  (GetAdc(IO))  //要提前初始化相应的ADC端口



#endif //FASTIO_H

