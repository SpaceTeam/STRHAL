#include "../Inc/STRHAL.h"

#include <stm32h7xx_ll_rcc.h>
#include <stm32h7xx_ll_cortex.h>
#include <stm32h7xx_ll_pwr.h>
#include <stm32h7xx_ll_utils.h>
#include <stm32h7xx_ll_bus.h>
#include <stm32h7xx_ll_system.h>

#define STRHAL_SYSCLK_INT_PLL1_M 8
#define STRHAL_SYSCLK_EXT_PLL1_M 1
#define STRHAL_SYSCLK_INT_PLL2_M 8
#define STRHAL_SYSCLK_EXT_PLL2_M 1
#define STRHAL_SYSCLK_INT_PLL3_M 8
#define STRHAL_SYSCLK_EXT_PLL3_M 1

#define STRHAL_SYSCLK_PLL1_P 2
#define STRHAL_SYSCLK_PLL1_Q 4 //2
#define STRHAL_SYSCLK_PLL1_R 2

#define STRHAL_SYSCLK_PLL2_P 4 //1
#define STRHAL_SYSCLK_PLL2_Q 4 //2
#define STRHAL_SYSCLK_PLL2_R 2 //1

#define STRHAL_SYSCLK_PLL3_P 1
#define STRHAL_SYSCLK_PLL3_Q 1
#define STRHAL_SYSCLK_PLL3_R 1

#define STRHAL_SYSCLK_START_TOT 16000000

static STRHAL_SysClk_Src_t _SysClk_Src = STRHAL_SYSCLK_SRC_BKP;
static int _INITIALIZED = 0;
static STRHAL_Oof_t _status = STRHAL_NOICE;

static inline STRHAL_SysClk_Src_t _SysClk_Init(STRHAL_SysClk_Src_t src, uint32_t freq);
static inline STRHAL_SysClk_Src_t _SysClk_Backup();


STRHAL_Oof_t STRHAL_Init(STRHAL_SysClk_Src_t src, uint32_t freq) {
	if(_INITIALIZED)
		return _status;

	_status = STRHAL_NOICE;

	LL_APB4_GRP1_EnableClock(LL_APB4_GRP1_PERIPH_SYSCFG);
	LL_PWR_ConfigSupply(LL_PWR_LDO_SUPPLY);

	NVIC_SetPriorityGrouping(0x03);


	if(_SysClk_Init(src, freq) != src)
		_status |= STRHAL_OOF_SYSCLK;

/*
	LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);
			while (LL_FLASH_GetLatency() != LL_FLASH_LATENCY_1) {
			}
			LL_PWR_ConfigSupply(LL_PWR_LDO_SUPPLY);
			LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE3);
			LL_RCC_HSE_Enable();

			// Wait till HSE is ready
			while (LL_RCC_HSE_IsReady() != 1) {

			}
			LL_RCC_PLL_SetSource(LL_RCC_PLLSOURCE_HSE);
			LL_RCC_PLL1P_Enable();
			LL_RCC_PLL1Q_Enable();
			LL_RCC_PLL1R_Enable();
			LL_RCC_PLL1_SetVCOInputRange(LL_RCC_PLLINPUTRANGE_8_16);
			LL_RCC_PLL1_SetVCOOutputRange(LL_RCC_PLLVCORANGE_WIDE);
			LL_RCC_PLL1_SetM(1);
			LL_RCC_PLL1_SetN(20);
			LL_RCC_PLL1_SetP(2);
			LL_RCC_PLL1_SetQ(2);
			LL_RCC_PLL1_SetR(2);
			LL_RCC_PLL1_Enable();

			// Wait till PLL is ready
			while (LL_RCC_PLL1_IsReady() != 1) {
			}

			LL_RCC_PLL2P_Enable();
			LL_RCC_PLL2Q_Enable();
			LL_RCC_PLL2_SetVCOInputRange(LL_RCC_PLLINPUTRANGE_8_16);
			LL_RCC_PLL2_SetVCOOutputRange(LL_RCC_PLLVCORANGE_WIDE);
			LL_RCC_PLL2_SetM(1);
			LL_RCC_PLL2_SetN(20);
			LL_RCC_PLL2_SetP(1);
			LL_RCC_PLL2_SetQ(2);
			LL_RCC_PLL2_SetR(1);
			LL_RCC_PLL2_Enable();

			// Wait till PLL is ready
			while (LL_RCC_PLL2_IsReady() != 1) {
			}

			// Intermediate AHB prescaler 2 when target frequency clock is higher than 80 MHz
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
*/

	//peripheral clock configuration according to Table 59, reference manual
	STRHAL_Clock_Init();
	STRHAL_SysTick_Init();
	STRHAL_GPIO_Init();
	STRHAL_OPAMP_Init();
	STRHAL_UART_Init();
	STRHAL_ADC_Init();
	STRHAL_TIM_Init();
	STRHAL_SPI_Init();
	_status |= STRHAL_CAN_Init();
	STRHAL_QSPI_Init();

	_INITIALIZED = 1;

	return _status;
}


inline STRHAL_SysClk_Src_t _SysClk_Init(STRHAL_SysClk_Src_t src, uint32_t freq) {
	uint32_t tot;

	if(LL_SetFlashLatency(LL_FLASH_LATENCY_1) != SUCCESS)
				return _SysClk_Backup();

	LL_RCC_PLL1_Disable();
	LL_RCC_PLL2_Disable();
	LL_RCC_PLL3_Disable();

	if(src == STRHAL_SYSCLK_SRC_INT) {

		//TODO: compare HSI_VALUE with LL_RCC_PLLINPUTRANGE

		//TODO: Change voltage scaling?
		LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE3);

		LL_RCC_HSI_Enable();

		for(tot = 0; LL_RCC_PLL1_IsReady() && !LL_RCC_HSI_IsReady(); ++tot) {
			if(tot > STRHAL_SYSCLK_START_TOT)
				return _SysClk_Backup();
		}

		//pll init according to figure 48, reference manual
		LL_RCC_PLL_SetSource(LL_RCC_PLLSOURCE_HSI);

		LL_RCC_PLL1_SetM(STRHAL_SYSCLK_INT_PLL1_M);
		LL_RCC_PLL1_SetVCOInputRange(LL_RCC_PLLINPUTRANGE_8_16);
		LL_RCC_PLL1_SetVCOOutputRange(LL_RCC_PLLVCORANGE_WIDE);
		LL_RCC_PLL1FRACN_Disable();
		LL_RCC_PLL1_SetN(4*STRHAL_SYSCLK_FREQ/(HSI_VALUE/4));
		LL_RCC_PLL1P_Enable();
		LL_RCC_PLL1Q_Enable();
		LL_RCC_PLL1R_Enable();
		LL_RCC_PLL1_SetP(STRHAL_SYSCLK_PLL1_P);
		LL_RCC_PLL1_SetQ(STRHAL_SYSCLK_PLL1_Q);
		LL_RCC_PLL1_SetR(STRHAL_SYSCLK_PLL1_R);
		LL_RCC_PLL1_Enable();

		for(tot = 0; !LL_RCC_PLL1_IsReady(); ++tot) {
			if(tot > STRHAL_SYSCLK_START_TOT)
				return _SysClk_Backup();
		}
		LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL1);

		for(tot = 0; LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL1; ++tot) {
			if(tot > STRHAL_SYSCLK_START_TOT)
				return _SysClk_Backup();
		}

		LL_Init1msTick(STRHAL_SYSCLK_FREQ);
		LL_SetSystemCoreClock(STRHAL_SYSCLK_FREQ);
		_SysClk_Src = STRHAL_SYSCLK_SRC_INT;

		LL_RCC_PLL2_SetM(STRHAL_SYSCLK_INT_PLL2_M);
		LL_RCC_PLL2_SetVCOInputRange(LL_RCC_PLLINPUTRANGE_8_16);
		LL_RCC_PLL2_SetVCOOutputRange(LL_RCC_PLLVCORANGE_WIDE);
		LL_RCC_PLL2FRACN_Disable();
		LL_RCC_PLL2_SetN(4*STRHAL_SYSCLK_FREQ/(3*HSI_VALUE/4));
		LL_RCC_PLL2P_Enable();
		LL_RCC_PLL2Q_Enable();
		LL_RCC_PLL2R_Enable();
		LL_RCC_PLL2_SetP(STRHAL_SYSCLK_PLL2_P);
		LL_RCC_PLL2_SetQ(STRHAL_SYSCLK_PLL2_Q);
		LL_RCC_PLL2_SetR(STRHAL_SYSCLK_PLL2_R);
		LL_RCC_PLL2_Enable();

		for(tot = 0; !LL_RCC_PLL2_IsReady(); ++tot) {
			if(tot > STRHAL_SYSCLK_START_TOT)
				return _SysClk_Backup();
		}

		/*
		LL_RCC_PLL3_SetM(STRHAL_SYSCLK_INT_PLL3_M);
		LL_RCC_PLL3_SetVCOInputRange(LL_RCC_PLLINPUTRANGE_8_16);
		LL_RCC_PLL3_SetVCOOutputRange(LL_RCC_PLLVCORANGE_WIDE);
		LL_RCC_PLL3FRACN_Disable();
		LL_RCC_PLL3_SetN(4*STRHAL_SYSCLK_FREQ/(HSI_VALUE/4));
		LL_RCC_PLL3P_Enable();
		LL_RCC_PLL3Q_Enable();
		LL_RCC_PLL3R_Enable();
		LL_RCC_PLL3_SetP(STRHAL_SYSCLK_PLL3_P);
		LL_RCC_PLL3_SetQ(STRHAL_SYSCLK_PLL3_Q);
		LL_RCC_PLL3_SetR(STRHAL_SYSCLK_PLL3_R);
		LL_RCC_PLL3_Enable();

		for(tot = 0; !LL_RCC_PLL3_IsReady(); ++tot) {
			if(tot > STRHAL_SYSCLK_START_TOT)
				return _SysClk_Backup();
		}
		*/
	}
	else if(src == STRHAL_SYSCLK_SRC_EXT) {

		//TODO: compare freq with LL_RCC_PLLINPUTRANGE

		//TODO: Change voltage scaling?
		LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE3);


		if(LL_RCC_HSE_IsReady()) {
			LL_RCC_HSE_Disable();

			for(tot = 0; LL_RCC_HSE_IsReady(); ++tot) {
				if(tot > STRHAL_SYSCLK_START_TOT)
					return _SysClk_Backup();
			}
		}

		//LL_RCC_HSE_EnableBypass();
		LL_RCC_HSE_Enable();

		for(tot = 0; LL_RCC_PLL1_IsReady() && !LL_RCC_HSE_IsReady(); ++tot) {
			if(tot > STRHAL_SYSCLK_START_TOT)
				return _SysClk_Backup();
		}


		//pll init according to figure 48, reference manual
		LL_RCC_PLL_SetSource(LL_RCC_PLLSOURCE_HSE);

		LL_RCC_PLL1_SetM(STRHAL_SYSCLK_EXT_PLL1_M);
		LL_RCC_PLL1_SetVCOInputRange(LL_RCC_PLLINPUTRANGE_8_16);
		LL_RCC_PLL1_SetVCOOutputRange(LL_RCC_PLLVCORANGE_WIDE);
		LL_RCC_PLL1FRACN_Disable();
		LL_RCC_PLL1_SetN(2*STRHAL_SYSCLK_FREQ/freq);
		LL_RCC_PLL1P_Enable();
		LL_RCC_PLL1Q_Enable();
		LL_RCC_PLL1R_Enable();
		LL_RCC_PLL1_SetP(STRHAL_SYSCLK_PLL1_P);
		LL_RCC_PLL1_SetQ(STRHAL_SYSCLK_PLL1_Q);
		LL_RCC_PLL1_SetR(STRHAL_SYSCLK_PLL1_R);
		LL_RCC_PLL1_Enable();

	    for(tot = 0; !LL_RCC_PLL1_IsReady(); ++tot) {
			if(tot > STRHAL_SYSCLK_START_TOT)
				return _SysClk_Backup();
		}


		LL_RCC_PLL2_SetM(STRHAL_SYSCLK_EXT_PLL2_M);
		LL_RCC_PLL2_SetVCOInputRange(LL_RCC_PLLINPUTRANGE_8_16);
		LL_RCC_PLL2_SetVCOOutputRange(LL_RCC_PLLVCORANGE_WIDE);
		LL_RCC_PLL2FRACN_Disable();
		//LL_RCC_PLL2_SetN(2*STRHAL_SYSCLK_FREQ/(3*freq));
		LL_RCC_PLL2_SetN(2*STRHAL_SYSCLK_FREQ/freq);
		LL_RCC_PLL2P_Enable();
		LL_RCC_PLL2Q_Enable();
		LL_RCC_PLL2R_Enable();
		LL_RCC_PLL2_SetP(STRHAL_SYSCLK_PLL2_P);
		LL_RCC_PLL2_SetQ(STRHAL_SYSCLK_PLL2_Q);
		LL_RCC_PLL2_SetR(STRHAL_SYSCLK_PLL2_R);
		LL_RCC_PLL2_Enable();

		for(tot = 0; !LL_RCC_PLL2_IsReady(); ++tot) {
			if(tot > STRHAL_SYSCLK_START_TOT)
				return _SysClk_Backup();
		}


		/*
		LL_RCC_PLL3_SetM(STRHAL_SYSCLK_INT_PLL3_M);
		LL_RCC_PLL3_SetVCOInputRange(LL_RCC_PLLINPUTRANGE_8_16);
		LL_RCC_PLL3_SetVCOOutputRange(LL_RCC_PLLVCORANGE_WIDE);
		LL_RCC_PLL3FRACN_Disable();
		LL_RCC_PLL3_SetN(4*STRHAL_SYSCLK_FREQ/(HSI_VALUE/4));
		LL_RCC_PLL3P_Enable();
		LL_RCC_PLL3Q_Enable();
		LL_RCC_PLL3R_Enable();
		LL_RCC_PLL3_SetP(STRHAL_SYSCLK_PLL3_P);
		LL_RCC_PLL3_SetQ(STRHAL_SYSCLK_PLL3_Q);
		LL_RCC_PLL3_SetR(STRHAL_SYSCLK_PLL3_R);
		LL_RCC_PLL3_Enable();

		for(tot = 0; !LL_RCC_PLL3_IsReady(); ++tot) {
			if(tot > STRHAL_SYSCLK_START_TOT)
				return _SysClk_Backup();
		}
		*/

	}
	//Set AHB prescaler, fmax = 240 MHz! (reference manual, Figure 49)
		LL_RCC_SetAHBPrescaler(LL_RCC_AHB_DIV_2);

	LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL1);

	for(tot = 0; LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL1; ++tot) {
		if(tot > STRHAL_SYSCLK_START_TOT)
			return _SysClk_Backup();
	}

	LL_Init1msTick(STRHAL_SYSCLK_FREQ);
	LL_SetSystemCoreClock(STRHAL_SYSCLK_FREQ);
	_SysClk_Src = STRHAL_SYSCLK_SRC_EXT;

	LL_RCC_HSE_EnableCSS(); //Clock fault -> generate NMI
	//TODO: NMI handler? clear HSECSSF



	//Set APB prescalers, fmax = 100 MHz! (datasheet, Figure 1)
	LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
	LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
	LL_RCC_SetAPB3Prescaler(LL_RCC_APB3_DIV_1);
	LL_RCC_SetAPB4Prescaler(LL_RCC_APB4_DIV_1);
	return _SysClk_Src;
}

inline STRHAL_SysClk_Src_t _SysClk_Backup() {
	LL_APB4_GRP1_EnableClock(LL_APB4_GRP1_PERIPH_SYSCFG);
	LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);
	while(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_0)
		;

	LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE3);

	LL_RCC_HSI_Enable();
	while(!LL_RCC_HSI_IsReady())
		;

	LL_RCC_HSI_SetCalibTrimming(64);

	LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);
	while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
		;

	//Set AHB prescaler, fmax = 240 MHz! (reference manual, Figure 49)
	LL_RCC_SetAHBPrescaler(LL_RCC_AHB_DIV_2);

	//Set APB prescalers, fmax = 100 MHz! (datasheet, Figure 1)
	LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
	LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
	LL_RCC_SetAPB3Prescaler(LL_RCC_APB3_DIV_1);
	LL_RCC_SetAPB4Prescaler(LL_RCC_APB4_DIV_1);

	LL_Init1msTick(HSI_VALUE);
	LL_SetSystemCoreClock(HSI_VALUE);

	_SysClk_Src = STRHAL_SYSCLK_SRC_BKP;

	return _SysClk_Src;
}



