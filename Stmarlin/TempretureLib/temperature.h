


#ifndef TEMPERATURE_H
#define TEMPERATURE_H 

#include "stmarlin.h"

// public functions
void tp_init(void);  //initialise the heating  //
void manage_heater(void); //it is critical that this is called periodically.

// low level conversion routines
// do not use these routines and variables outside of temperature.cpp
extern int target_temperature[EXTRUDERS];  
extern float current_temperature[EXTRUDERS];
extern int target_temperature_bed;
extern float current_temperature_bed;

#ifdef PIDTEMP
  extern float Kp,Ki,Kd,Kc;
  float scalePID_i(float i);
  float scalePID_d(float d);
  float unscalePID_i(float i);
  float unscalePID_d(float d);

#endif
#ifdef PIDTEMPBED
  extern float bedKp,bedKi,bedKd;
#endif

#ifdef FAN_SOFT_PWM
  extern unsigned char fanSpeedSoftPwm;
#endif
  
//high level conversion routines, for use outside of temperature.cpp 高水平的转换例程，用于在temperature.c文件外使用
//inline so that there is no performance decrease. //内联以便没有性能下降
//deg = degreeCelsius
extern inline float degHotend(u8 extruder); 
extern inline float degBed(void); 
extern inline float degTargetHotend(u8 extruder); 
extern inline float degTargetBed(void); 
extern inline void setTargetHotend(const float celsius, uint8_t extruder); 
extern inline void setTargetBed(const float celsius); 
extern inline bool isHeatingHotend(u8 extruder);
extern inline bool isHeatingBed(void); 
extern inline bool isCoolingHotend(u8 extruder); 
extern inline bool isCoolingBed(void); 

int getHeaterPower(int heater);
void disable_heater(void);
void setWatch(void);
void updatePID(void);

//void test(char temp);

#define degHotend0() degHotend(0)
#define degTargetHotend0() degTargetHotend(0)
#define setTargetHotend0(_celsius) setTargetHotend((_celsius), 0)
#define isHeatingHotend0() isHeatingHotend(0)
#define isCoolingHotend0() isCoolingHotend(0)
#if EXTRUDERS > 1
#define degHotend1() degHotend(1)
#define degTargetHotend1() degTargetHotend(1)
#define setTargetHotend1(_celsius) setTargetHotend((_celsius), 1)
#define isHeatingHotend1() isHeatingHotend(1)
#define isCoolingHotend1() isCoolingHotend(1)
#else
#define setTargetHotend1(_celsius) do{}while(0)
#endif
#if EXTRUDERS > 2
#define degHotend2() degHotend(2)
#define degTargetHotend2() degTargetHotend(2)
#define setTargetHotend2(_celsius) setTargetHotend((_celsius), 2)
#define isHeatingHotend2() isHeatingHotend(2)
#define isCoolingHotend2() isCoolingHotend(2)
#else
#define setTargetHotend2(_celsius) do{}while(0)
#endif

void PID_autotune(float temp, int extruder, int ncycles);
void autotempShutdown(void);

#endif

