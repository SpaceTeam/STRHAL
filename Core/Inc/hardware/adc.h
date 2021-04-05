
#ifndef HARDWARE_ADC_H__
#define HARDWARE_ADC_H__

#include <stdint.h>

void ADC_Init(void);
void ADC_Calibrate(void);
volatile uint16_t ADC_GetData(uint8_t index);


#endif
