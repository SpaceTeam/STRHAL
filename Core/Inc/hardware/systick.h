#ifndef HARDWARE_SYSTICK_H_
#define HARDWARE_SYSTICK_H_
#include <stdint.h>

void Systick_Init(void);
uint64_t Systick_GetTick(void);


#endif
