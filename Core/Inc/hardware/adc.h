
#ifndef HARDWARE_ADC_H__
#define HARDWARE_ADC_H__

#include <stdint.h>

void Adc_Init(void);
void Adc_Calibrate(void);
volatile uint16_t Adc_GetData(uint8_t index);


#endif
