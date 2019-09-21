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

#include "key.h"
#include "exfuns.h"
#include "flash.h"
#include "lcd12864.h"
#include "lcd12864_menu.h"
#include "malloc.h"

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
  TIM2_Int_Init(99,7199); //用于按键的定时检测（10ms定时中断）
  
   	 KEY_Init();				//按键初始化 
	 //AT24CXX_Init();    //AT24CXX初始化 
	 SPI_Flash_Init();  //W25Qxx初始化
	 LCD12864_Init();   //ST7920 lcd12864底层驱动的初始化
	 //*****************************初始化软件层驱动	****************************// 										  
   mem_init();				      //内存池初始化
   exfuns_init();		        //为fatfs相关变量申请内存
   f_mount(fs[0],"0:",1); 	//挂载SD卡  
//   f_mount(fs[1],"1:",1); 	//挂载挂载FLASH.	
	 lcd_menuInit();          //屏幕菜单初始化
  
 	setup(); //执行打印进程所需的初始化函数
  
	while(1) 
	{		
		loop(); //打印进程循环体
	}
  //return 0;
}

/////测试文件系统
//#include "exfuns.h"
//#include "ff.h"
//#include "malloc.h"
//#include "cardreader.h"

//int main(void)
//{
//  u32 total,free;
//  Usart1Init(BAUDRATE); 	 //串口1初始化
//  exfuns_init();							//为fatfs相关变量申请内存				 
//  f_mount(fs[0],"0:",1); 					//挂载SD卡 
//  
//  while(exf_getfree("0",&total,&free))	//得到SD卡的总容量和剩余容量
//	{
//		printf("SD Card Fatfs Error!\n");
//	}								
//  printf("FATFS OK!\n");
//  printf("SD Total Size: %ldMB",total>>10); //显示SD卡总容量 MB
//  printf("SD  Free Size: %ldMB",free>>10); //显示SD卡剩余容量 MB
//  
////  gcode_ls("0:\\GCODE");
//  
//  while(1)
//  {
//    
//  }
//}
