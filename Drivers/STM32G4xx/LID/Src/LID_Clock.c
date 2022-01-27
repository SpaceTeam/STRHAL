#include <stm32g4xx_ll_rcc.h>
#include <stm32g4xx_ll_cortex.h>
#include <stm32g4xx_ll_pwr.h>
#include <stm32g4xx_ll_utils.h>
#include <stm32g4xx_ll_bus.h>
#include <stm32g4xx_ll_system.h>
#include "../Inc/LID_Clock.h"

static LID_Clock_Status_t _SysClock_Init();

LID_Clock_Status_t LID_Clock_Init() {
	LID_Clock_Status_t e = LID_CLOCK_GOOD;

	e = _SysClock_Init();
	if(e != LID_CLOCK_GOOD)
		return e;

	return LID_CLOCK_GOOD;
}

LID_Clock_Status_t _SysClock_Init() {
	LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);
	while(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_0)
	{
	}
	LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
	LL_RCC_HSI_Enable();
	/* Wait till HSI is ready */
	while(LL_RCC_HSI_IsReady() != 1)
	{
	}

	LL_RCC_HSI_SetCalibTrimming(64);
	LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);
	/* Wait till System clock is ready */
	while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
	{
	}

	/* Set AHB prescaler*/
	LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
	LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
	LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
	LL_SetSystemCoreClock(170000000);


	LL_Init1msTick(170000000);



	return LID_CLOCK_GOOD;
}
