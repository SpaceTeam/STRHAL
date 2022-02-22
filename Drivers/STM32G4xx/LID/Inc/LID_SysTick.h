#ifndef LID_SYSTICK_H_
#define LID_SYSTICK_H_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

void LID_SysTick_Init();
void LID_Systick_BusyWait(uint32_t ticks);
uint64_t LID_Systick_GetTick(void);


#ifdef __cplusplus
}
#endif

#endif /* LID_SYSTICK_H_ */
