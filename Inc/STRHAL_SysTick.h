#ifndef STRHAL_SYSTICK_H_
#define STRHAL_SYSTICK_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

void STRHAL_SysTick_Init();
void STRHAL_Systick_BusyWait(uint32_t ticks);
uint64_t STRHAL_Systick_GetTick(void);

#ifdef __cplusplus
}
#endif

#endif /* STRHAL_SYSTICK_H_ */
