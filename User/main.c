/**
  ******************************************************************************
  * @file    main.c
  * @author  xiaoyuan
  * @version V2.0.0
  * @date    2018-1-29
  * @brief   提供C语言主程序入口
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
  
#include "stmarlin.h"
#include "usart.h"
#include "timer.h"
#include "delay.h"


/**
  * @brief  C语言主函数，一切C语言函数执行起始的地方
  * @param  none
  * @retval none
  */
int main(void)
{	
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2

  delay_init();			     //延时初始化
 	Usart1Init(BAUDRATE); 	 //串口1初始化  
  TIM3_Int_Init(9,7199);  //用于返回系统运行时间（1ms定时中断）
  
 	setup(); //执行打印进程所需的初始化函数
	
	while(1) 
	{		
		loop(); //打印进程循环体
	}
  //return 0;
}
