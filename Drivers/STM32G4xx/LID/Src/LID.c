#include "../Inc/LID.h"

#include <stm32g4xx_ll_rcc.h>
#include <stm32g4xx_ll_cortex.h>
#include <stm32g4xx_ll_pwr.h>
#include <stm32g4xx_ll_utils.h>
#include <stm32g4xx_ll_bus.h>
#include <stm32g4xx_ll_system.h>

#define LID_SYSCLK_INT_PLL_M 2
#define LID_SYSCLK_INT_PLL_N 32
#define LID_SYSCLK_INT_PLL_R 2

#define LID_SYSCLK_EXT_PLL_M 2
#define LID_SYSCLK_EXT_PLL_N 128000000/HSE_VALUE
#define LID_SYSCLK_EXT_PLL_R 2

#define LID_SYSCLK_FREQ 128000000

#define LID_SYSCLK_START_TOT 1600000

static LID_SysClk_Src_t _SysClk_Src = LID_SYSCLK_SRC_BKP;
static int _SysClk_LOCKED = 0;

static inline LID_SysClk_Src_t _SysClk_Init(LID_SysClk_Src_t src, uint32_t freq);
static inline LID_SysClk_Src_t _SysClk_Backup();

LID_Status_t LID_Init(LID_SysClk_Src_t src, uint32_t freq) {
	LID_Status_t status = LID_NOICE;

	NVIC_SetPriorityGrouping(0x03);
	if(_SysClk_Init(src, freq) != src)
		status |= LID_SYSCLK_ERR;

	LID_Clock_Init();
	LID_GPIO_Init();
	LID_OPAMP_Init();
	LID_UART_Init();
	LID_ADC_Init();

	return 0;
}

inline LID_SysClk_Src_t _SysClk_Init(LID_SysClk_Src_t src, uint32_t freq) {
	if(_SysClk_LOCKED)
		return _SysClk_Src;

	_SysClk_LOCKED = 1;

	uint32_t tot;
	if(src == LID_SYSCLK_SRC_INT) {
		if(!LL_SetFlashLatency(LID_SYSCLK_FREQ))
			return _SysClk_Backup();

		LL_PWR_EnableRange1BoostMode();

		LL_RCC_PLL_Disable();

		for(tot = 0; LL_RCC_PLL_IsReady() && !LL_RCC_HSE_IsReady(); ++tot) {
			if(tot > LID_SYSCLK_START_TOT)
				return _SysClk_Backup();
		}

		LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI,
				LID_SYSCLK_INT_PLL_M,
				LID_SYSCLK_INT_PLL_N,
				LID_SYSCLK_INT_PLL_R
		);
		LL_RCC_PLL_EnableDomain_SYS();
		LL_RCC_PLL_Enable();

		for(tot = 0; !LL_RCC_PLL_IsReady(); ++tot) {
			if(tot > LID_SYSCLK_START_TOT)
				return _SysClk_Backup();
		}
		LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

		for(tot = 0; LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_PLL; ++tot) {
			if(tot > LID_SYSCLK_START_TOT)
				return _SysClk_Backup();
		}

		_SysClk_Src = LID_SYSCLK_SRC_INT;
	}
	else if(src == LID_SYSCLK_SRC_EXT) {
		if(!LL_SetFlashLatency(LID_SYSCLK_FREQ))
			return _SysClk_Backup();

		LL_RCC_PLL_Disable();
		if(LL_RCC_HSE_IsReady()) {
			LL_RCC_HSE_Disable();

			for(tot = 0; LL_RCC_HSE_IsReady(); ++tot) {
				if(tot > LID_SYSCLK_START_TOT)
					return _SysClk_Backup();
			}
		}

		LL_RCC_HSE_EnableBypass();
		LL_RCC_HSE_Enable();

		for(tot = 0; LL_RCC_PLL_IsReady() && !LL_RCC_HSE_IsReady(); ++tot) {
			if(tot > LID_SYSCLK_START_TOT)
				return _SysClk_Backup();
		}

		LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE,
				LID_SYSCLK_EXT_PLL_M,
				LID_SYSCLK_EXT_PLL_N,
				LID_SYSCLK_EXT_PLL_R
		);
	    LL_RCC_PLL_EnableDomain_SYS();
	    LL_RCC_PLL_Enable();

	    for(tot = 0; !LL_RCC_PLL_IsReady(); ++tot) {
			if(tot > LID_SYSCLK_START_TOT)
				return _SysClk_Backup();
		}
		LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

		for(tot = 0; LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_PLL; ++tot) {
			if(tot > LID_SYSCLK_START_TOT)
				return _SysClk_Backup();
		}

		_SysClk_Src = LID_SYSCLK_SRC_EXT;
	}

	LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
	LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
	LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
	return _SysClk_Src;
}

inline LID_SysClk_Src_t _SysClk_Backup() {
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

	LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);
	while(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_0)
		;

	LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
	LL_PWR_DisableRange1BoostMode();

	LL_RCC_HSI_Enable();
	while(!LL_RCC_HSI_IsReady())
		;

	LL_RCC_HSI_SetCalibTrimming(64);

	LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);
	while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
		;

	/* Set AHB prescaler*/
	LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
	LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
	LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

	LL_Init1msTick(HSI_VALUE);
	LL_SetSystemCoreClock(HSI_VALUE);

	_SysClk_LOCKED = 1;
	_SysClk_Src = LID_SYSCLK_SRC_BKP;

	return _SysClk_Src;
}
