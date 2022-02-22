#include "../Inc/LID_SysTick.h"

#include <stm32g4xx_ll_utils.h>
#include <stm32g4xx_ll_cortex.h>

static volatile uint64_t systick_count = 0;

void LID_SysTick_Init() {
	//LL_Init1msTick(SystemCoreClock);
	//1ms tick already in LID.c sysclock init
	LL_SYSTICK_EnableIT();
	systick_count = 0;
}

void LID_Systick_BusyWait(uint32_t ticks) {
	uint64_t end = systick_count + ticks;
	while (systick_count < end);
}

uint64_t LID_Systick_GetTick() {
	return systick_count;
}

void SysTick_Handler() {
	systick_count++;
}
