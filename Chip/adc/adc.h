#ifndef __ADC_H
#define __ADC_H	

#include "stm32f10x.h"


void Adc_Init(void);
u16  Get_Adc(u8 ch); 
uint16_t GetAdc(ADC_TypeDef* ADCx, uint8_t ADC_Channel);
u16 Get_Adc_Average(u8 ch,u8 times); 
 
#endif 
