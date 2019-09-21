/**
  ******************************************************************************
  * @file    readme.txt
  * @author  王元
  * @version V1.0
  * @date    2016-4-4
  * @brief   用于记录工程文档改变的日志
  ******************************************************************************
  * @attention
  *
  * 芯片型号: STM32F103VET6 
  *
  ******************************************************************************
  */ 
  2016.4.12  xiaoyuan
  该工程文件结构：
  USER-------stm32f10x.c,stm32f10x.h（中断服务函数）；stm32f10x_conf.h（片上外设配置头文件）；main.c（主函数）
  HARDWARE---存放用户编写的硬件资源程序
  FWLIB------STM32F10x_StdPeriph_Lib_V3.5.0 （V3.5固件库）
  CMSIS------system_stm32f10x.c,core_cm3.c  （核内外设函数文件）；stm32f10x.h （外设寄存器定义和中断向量表定义）  
             system_stm32f10x.h,core_cm3.h
  STARTCODE--启动代码
  DOC--------工程说明文档
  
  一些重要文件的介绍：
  core_cm3.c，core_cm3.h是位于CMSIS标准的核内设备函数层的CM3核通用文件，为芯片外设提供一个进入CM3内核的接口
  system_stm32f10x.c，system_stm32f10x.h 设置系统的时钟和总线时钟。
  stm32f10x.h 包含了STM32中寄存器地址和结构体类型定义，在使用到STM32固件库的地方都要包含这个头文件。
  启动文件：是任何处理器在上电复位之后最先运行的一段汇编程序（用汇编语言为C语言的运行建立一个合适的运行环境）
           （1）初始化堆栈指针SP
		   （2）初始化程序计数器指针PC
		   （3）设置堆栈的大小
		   （4）设置异常向量表的入口地址
		   （5）配置外部SRAM作为数据存储器 （由用户配置）
		   （6）设置C库的分支入口__main（最终用来调用main函数）
		   
常用类型的一些定义，定义在stdint.h文件中：
/* exact-width signed integer types */
typedef   signed          char int8_t;   //字符型变量
typedef   signed short     int int16_t;  //有符号短整形
typedef   signed           int int32_t;  //有符号长整形
typedef   signed       __INT64 int64_t;

/* exact-width unsigned integer types */
typedef unsigned          char uint8_t;   //无符号字符型 
typedef unsigned short     int uint16_t;  //无符号短整形
typedef unsigned           int uint32_t;  //无符号长整形
typedef unsigned       __INT64 uint64_t;

/* 版本更新日志 */
以V2.0.0作为Stmarlin 3D打印机固件的起始版本。

  2018/2/4 Stmarlin V2.0.0
1.该版本能够实现通过上位机控制实现打印功能（尚未完整测试），目前为不支持SD卡，不支持LCD屏幕显示。
2.该版本是从之前移植工程上继续优化而来的3D打印机固件，当前参数依旧支持自己DIY的3D打印机，端口配置依旧和当时设计的硬件匹配。
3.在步进电机控制模块和路径规划模块分别增加了调试开关，可以通过打开.h文件中定义的调试开关，来开启调试模式。通过这些调试开关可以很方便测试其中的原理。
4.新增加了fastio.h用来实现类似于原版marlin固件中的WRITE等函数。pin.h文件中只需制定引脚号（如：PF10等）即可。在后续如需增加引脚实现
  其他功能支持需要在两个文件中都增加相应的引脚定义支持，并在相应功能初始化函数中初始化该引脚。
5.去除了原版本中的include_config.h（包含了所有要用到的头文件），包含这个头文件虽会给写程序带来一些方便，但会使编译效率降低，也会使得文件
  包含关系不够名明确。文件包含要只包含该文件用到的头文件，软件部分是分层级的，文件包含要做到顶层文件包含底层文件，底层文件尽量不要包含顶层文件
	同一层级个文件之间可以互相包含。
6.规范了工程中的代码风格，按最新版marlin固件官网规范的风格。注释规范尚不标准，后面慢慢完善。
7.重新优化了工程结构，具体结构可参考Stmarlin Keil MDK 工程目录.mmap，并将代码文件做了重新分类，有些文件夹仍为空，是后面优化升级时需要慢慢填充的。
8.该版本采用C99标准，要在选项里面C/C++栏勾选上C99。此外要支持printf函数要在选项中勾选上使用微库。

 2018/2/4 Stmarlin V2.0.1
目标：为工程增加支持SD卡的代码
1.增加sdio的板级支持代码   OK
2.移植FATFS文件系统        OK
3.增加stmarlin读卡支持    OK
4.增加屏幕菜单  OK

2018/3/10
利用标准库函数实现下列内容
/**
 * Fast I/O interfaces for STM32F1
 * These use GPIO functions instead of Direct Port Manipulation, as on AVR.
 */

#ifndef _FASTIO_STM32F1_H
#define _FASTIO_STM32F1_H

#include <libmaple/gpio.h>

#define READ(IO)              (PIN_MAP[IO].gpio_device->regs->IDR & (1U << PIN_MAP[IO].gpio_bit) ? HIGH : LOW)
#define WRITE(IO, v)          (PIN_MAP[IO].gpio_device->regs->BSRR = (1U << PIN_MAP[IO].gpio_bit) << (16 * !(bool)v))
#define TOGGLE(IO)            (PIN_MAP[IO].gpio_device->regs->ODR = PIN_MAP[IO].gpio_device->regs->ODR ^ (1U << PIN_MAP[IO].gpio_bit))
#define WRITE_VAR(IO, v)      WRITE(io, v)

#define _GET_MODE(IO)         (gpio_get_mode(PIN_MAP[IO].gpio_device, PIN_MAP[IO].gpio_bit))
#define _SET_MODE(IO,M)       do{ gpio_set_mode(PIN_MAP[IO].gpio_device, PIN_MAP[IO].gpio_bit, M); } while (0)
#define _SET_OUTPUT(IO)       _SET_MODE(IO, GPIO_OUTPUT_PP)

#define SET_INPUT(IO)         _SET_MODE(IO, GPIO_INPUT_FLOATING)
#define SET_INPUT_PULLUP(IO)  _SET_MODE(IO, GPIO_INPUT_PU)
#define SET_OUTPUT(IO)        do{ _SET_OUTPUT(IO); WRITE(IO, LOW); }while(0)

#define GET_INPUT(IO)         (_GET_MODE(IO) == GPIO_INPUT_FLOATING || _GET_MODE(IO) == GPIO_INPUT_ANALOG || _GET_MODE(IO) == GPIO_INPUT_PU || _GET_MODE(IO) == GPIO_INPUT_PD)
#define GET_OUTPUT(IO)        (_GET_MODE(IO) == GPIO_OUTPUT_PP)
#define GET_TIMER(IO)         (PIN_MAP[IO].timer_device != NULL)

#define OUT_WRITE(IO, v)      { _SET_OUTPUT(IO); WRITE(IO, v); }
/**
 * TODO: Write a macro to test if PIN is PWM or not.
 */
#define PWM_PIN(p)            true

#endif // _FASTIO_STM32F1_H

2018/4/6
测试当前程序：
    当前程序运行测试有问题，喷头在打印过程中总是莫名其妙的沿X轴或Y轴运动到最大位置或最小位置再返回。
		通过测试发现是stmarlin.c文件中process_command()函数和stepper.c文件中的步进电机中断服务函数的问题，具体原因还不清楚。
		通过将这两部分代码替换为原来的代码即可正常运行，只替换一处问题仍然存在。

2018/4/8
stmarlin 2.0.1 版本基本将原来完整的打印机固件功能已完全添加上来了，新添加的部分代码依然比较乱，尚未整理完全。经过测试可以
通过屏幕控制正常打印。因此以此版作为备份后面继续整理代码。

2018/4/8
stmarlin 2.0.2
接下来任务：使代码规范化，并使各个功能组件独立化。
           将LCD12864屏幕菜单添加完整并去除许多显示的bug，并整理出使用单色屏写菜单的心得。
修改：
1.修改fastio.h文件，将READ_INPUT替换为READ,并删除READ_OUTPUT的定义。（READ即代表了读取引脚的输入状态，和原版marlin固件中保持一致）
2.在configuration.h文件中增加了#define LCD12864_ST7920的宏名，尚需要修改代码使LCD12864可通过宏定义的方式选择是否使用，方便后面增加液晶屏。
3.lcd12864_menu.c文件中多处增加支持SDSUPPORT的宏，使之可以通过修改宏的方式选择是否支持SD卡，与此同时还可以使用LCD控制打印机。

2018/4/11


2018/4/12
1.现在的代码支持了SD卡的热插拔，但依旧还有bug，现在我之前的外壳坏掉的SD卡插上依旧不能读取到。
  解决方法：（尚未想到）
2.目前在未插入SD卡开机，和插入SD卡开机后再拔出SD卡，CardMenu菜单显示都有异常。 （已解决） 
  解决方法：昨天的解决方法不是很好，原来写的程序的功能完全可以实现，只是因为之前SD卡坏掉卡到SD卡槽里了致使SD卡插入监测引脚始终处于低电平状态（即SD卡插入状态）	

2018/4/12
写菜单的心得：
1.在当前菜单中显示输出尽量不要采用清屏函数。
2.在菜单跳转之前最好加上一次清屏处理，防止之前菜单的显示残留。
3.退出菜单时，应将当前菜单调整为初始化状态。

新的思路： 搞个按键缓冲区，需要处理时从缓冲区中提取。

明日计划：
1.修正打印状态菜单下文件名不滚动显示的问题，问题在状态菜单中的文件名还是之前的中间变量。
2.修正SD卡文件菜单文件名较长，滚动显示会是下一行产生乱码的问题。
3.在SD卡菜单中为第一个文件和最后一个文件绘制不同的选中标记。
4.代码规范化，删除不需要的代码。并注明修改日志。

2.0.2
1.修正了SD卡插入拔出的bug
2.重新修正了屏幕菜单的程序
（周末整理一下这块的更新日志，如sd卡菜单条目，如何写屏幕菜单等）
2018/4/19
2.0.3
1.在stepper.c文件中步进电机同步函数中解除了lcd_update函数的屏蔽，以修正在LCD状态菜单中按按键不反应的bug  （待测试）
2.尚需要修正在调整参数菜单改完的参数在打印完成之后恢复默认或清零（）