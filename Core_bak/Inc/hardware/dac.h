
#ifndef HARDWARE_DAC_H__
#define HARDWARE_DAC_H__

#include <stdint.h>
#include "stm32h7xx_ll_dac.h"

void Dac_Init(void);
void Dac_SetValue(uint16_t value);

#endif
