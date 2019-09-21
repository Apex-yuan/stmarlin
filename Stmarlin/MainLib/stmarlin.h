#ifndef __STMARLIN_H
#define __STMARLIN_H

/* Stmarlin底层配置头文件 */
#include "Configuration.h"
#include "pins.h"
#include "fastio.h"
/*ST标准库头文件*/
#include "stm32f10x.h"
/* C标准库函数头文件 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
//#include <stdint.h>
#include <stdbool.h>
#include <string.h>


#define VERSION_STRING  "2.0.0"

//avr单片机采用哈弗结构，将程序存储器和数据存储器分开，而数据存储器RAM通常比较小，而程序存储器Flash空间比较大，
//因此就需要将占用空间较大的不需要改变的数据放在Flash中。
//#define __ATTR_PROGMEM__ __attribute__((__progmem__))
//#define PROGMEM __ATTR_PROGMEM__
//# define PSTR(s) ((const PROGMEM char *)(s)) //用于声明一个静态指针，该指针指向程序存储器空间中的一串字符串

#define	PSTR(s) ((const char*)(s)) //声明一个静态指针，该指针指向数据存储区的一串字符串常量s //P代表指针，STR代表字符串 
//#define true 0x1
//#define false 0x00
//#define bool u8
 
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
//#define round(x) ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define square(x) x*x
//#define abs(x) ((x)>0?(x):-(x))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt))) //限幅函数



#define OK_KEY (Get_Adc_Average(ADC_Channel_15,10)>3250)

void get_command(void);
void process_commands(void);
void manage_inactivity(void);

#define  enable_x()  WRITE(X_ENABLE_PIN, X_ENABLE_ON)  //X_ENABLE_PIN = X_ENABLE_ON
#define disable_x()	 WRITE(X_ENABLE_PIN, !X_ENABLE_ON)  //X_ENABLE_PIN = !X_ENABLE_ON

#define  enable_y()  WRITE(Y_ENABLE_PIN, Y_ENABLE_ON)  //Y_ENABLE_PIN=Y_ENABLE_ON
#define disable_y()	 WRITE(Y_ENABLE_PIN, !Y_ENABLE_ON)  //Y_ENABLE_PIN=!Y_ENABLE_ON

#define  enable_z()  WRITE(Z_ENABLE_PIN, Z_ENABLE_ON)  //Z_ENABLE_PIN=Z_ENABLE_ON
#define disable_z()	 WRITE(Z_ENABLE_PIN, !Z_ENABLE_ON)  //Z_ENABLE_PIN=!Z_ENABLE_ON

#define  enable_e0() WRITE(E0_ENABLE_PIN, E_ENABLE_ON)  //E0_ENABLE_PIN=E_ENABLE_ON
#define disable_e0() WRITE(E0_ENABLE_PIN, !E_ENABLE_ON)  //E0_ENABLE_PIN=!E_ENABLE_ON

#define  enable_e1() WRITE(E1_ENABLE_PIN, E_ENABLE_ON)  //E1_ENABLE_PIN=E_ENABLE_ON
#define disable_e1() WRITE(E1_ENABLE_PIN, !E_ENABLE_ON)  //E1_ENABLE_PIN=!E_ENABLE_ON

enum AxisEnum {X_AXIS=0, Y_AXIS=1, Z_AXIS=2, E_AXIS=3};

#define SERIAL_ERROR_START	printf("Error:")
#define SERIAL_ECHO_START	printf("echo:")
void loop(void);
void setup(void);
void FlushSerialRequestResend(void);
void ClearToSend(void);

//extern inline void gcode_G4(void);


void get_coordinates(void);
void prepare_move(void);
void kill(void);
void Stop(void);

bool IsStopped(void);

void enquecommand(const char *cmd); //put an ascii command at the end of the current buffer.
//void enquecommand_P(const char *cmd); //put an ascii command at the end of the current buffer, read from flash
void prepare_arc_move(uint8_t isclockwise);

void clamp_to_software_endstops(float target[3]);

#define CRITICAL_SECTION_START  __disable_irq();
#define CRITICAL_SECTION_END    __enable_irq();
extern float homing_feedrate[];
extern bool axis_relative_modes[];
extern int feedmultiply;
extern int extrudemultiply; // Sets extrude multiply factor (in percent)
extern float current_position[NUM_AXIS] ;
extern float add_homeing[3];
extern float min_pos[3];
extern float max_pos[3];
extern int fanSpeed;
extern unsigned long starttime;
#ifdef BARICUDA
extern int ValvePressure;
extern int EtoPPressure;
#endif

#ifdef FWRETRACT
extern bool autoretract_enabled;
extern bool retracted;
extern float retract_length, retract_feedrate, retract_zlift;
extern float retract_recover_length, retract_recover_feedrate;
#endif

extern unsigned long starttime;
extern unsigned long stoptime;

extern bool lcd_contiune_print_after_poweroff;
extern unsigned long poweroff_sdpos;
extern float poweroff_position_z;
extern char poweroff_printing_filename[30];

// Handling multiple extruders pins
extern uint8_t active_extruder;
#endif
