#ifndef HARDWARE_SYSTICK_H_
#define HARDWARE_SYSTICK_H_
#include <stdint.h>

void Systick_Init(void);
void Systick_BusyWait(uint32_t ticks);
uint64_t Systick_GetTick(void);


#endif
