# Stmarlin

这是一个将marlin移植到STM32平台的3D打印机固件，之前已经移植过一个版本，当时版本为实现功能，忽略了很多工程结构和一些细节方面的东西，这里基于当时能用的版本重新整理的一份固件。此版本定名为Stmarlin，意为基于ST公司的硬件平台移植marlin固件。

最初固件的版本为1.x，这里仍保留原固件对当前版本的贡献的价值，此处以V2.0.0为起始版本号开始，向后继续完善。

- **V2.0.0     2018/2/4**

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