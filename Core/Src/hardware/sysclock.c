#include "main.h"

void Sysclock_InitSystemClock(void)
{
	LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);
		while (LL_FLASH_GetLatency() != LL_FLASH_LATENCY_1) {
		}
		LL_PWR_ConfigSupply(LL_PWR_LDO_SUPPLY);
		LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE2);
		LL_RCC_HSE_Enable();

		/* Wait till HSE is ready */
		while (LL_RCC_HSE_IsReady() != 1) {

		}
		LL_RCC_PLL_SetSource(LL_RCC_PLLSOURCE_HSE);
		LL_RCC_PLL1P_Enable();
		LL_RCC_PLL1Q_Enable();
		LL_RCC_PLL1R_Enable();
		LL_RCC_PLL1_SetVCOInputRange(LL_RCC_PLLINPUTRANGE_8_16);
		LL_RCC_PLL1_SetVCOOutputRange(LL_RCC_PLLVCORANGE_WIDE);
		LL_RCC_PLL1_SetM(1);
		LL_RCC_PLL1_SetN(60);
		LL_RCC_PLL1_SetP(2);
		LL_RCC_PLL1_SetQ(2);
		LL_RCC_PLL1_SetR(2);
		LL_RCC_PLL1_Enable();

		/* Wait till PLL is ready */
		while (LL_RCC_PLL1_IsReady() != 1) {
		}

		LL_RCC_PLL2P_Enable();
		LL_RCC_PLL2Q_Enable();
		LL_RCC_PLL2_SetVCOInputRange(LL_RCC_PLLINPUTRANGE_8_16);
		LL_RCC_PLL2_SetVCOOutputRange(LL_RCC_PLLVCORANGE_WIDE);
		LL_RCC_PLL2_SetM(1);
		LL_RCC_PLL2_SetN(36);
		LL_RCC_PLL2_SetP(1);
		LL_RCC_PLL2_SetQ(4);
		LL_RCC_PLL2_SetR(1);
		LL_RCC_PLL2_Enable();

		/* Wait till PLL is ready */
		while (LL_RCC_PLL2_IsReady() != 1) {
		}

		/* Intermediate AHB prescaler 2 when target frequency clock is higher than 80 MHz */
		LL_RCC_SetAHBPrescaler(LL_RCC_AHB_DIV_2);

		LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL1);
		LL_RCC_SetSysPrescaler(LL_RCC_SYSCLK_DIV_1);
		LL_RCC_SetAHBPrescaler(LL_RCC_AHB_DIV_2);
		LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
		LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
		LL_RCC_SetAPB3Prescaler(LL_RCC_APB3_DIV_1);
		LL_RCC_SetAPB4Prescaler(LL_RCC_APB4_DIV_1);
		LL_SetSystemCoreClock(240000000);


	LL_Init1msTick(240000000);

	LL_SetSystemCoreClock(240000000);
	LL_RCC_HSE_EnableCSS();
}
void Sysclock_InitPeripheralClock(void)
{


	LL_RCC_SetQSPIClockSource(LL_RCC_QSPI_CLKSOURCE_HCLK);
	LL_RCC_SetSPIClockSource(LL_RCC_SPI123_CLKSOURCE_PLL1Q);
	LL_RCC_SetUSARTClockSource(LL_RCC_USART234578_CLKSOURCE_PCLK1);
	LL_RCC_SetADCClockSource(LL_RCC_ADC_CLKSOURCE_PLL2P);
	LL_RCC_SetFDCANClockSource(LL_RCC_FDCAN_CLKSOURCE_PLL2Q);

}

void Sysclock_Init(void)
{

	LL_APB4_GRP1_EnableClock(LL_APB4_GRP1_PERIPH_SYSCFG);
	NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	Sysclock_InitSystemClock();
	Sysclock_InitPeripheralClock();
}
