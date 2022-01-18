#include "systick.h"
#include "main.h"

static volatile uint64_t systick_count = 0;

void Systick_Init(void)
{
	LL_Init1msTick(SystemCoreClock);
	LL_SYSTICK_EnableIT();
	systick_count = 0;
}

void Systick_BusyWait(uint32_t ticks)
{
	uint64_t end = systick_count + ticks;
	while (systick_count < end);
}

inline uint32_t Systick_TimeToTicks(uint16_t h, uint8_t m, uint8_t s, uint16_t ms)
{
	return (uint32_t) (3600000 * h + 60000 * m + 1000 * s + ms);
}

uint64_t Systick_GetTick(void)
{
	return systick_count;
}

void SysTick_Handler(void)
{
	systick_count++;
}
