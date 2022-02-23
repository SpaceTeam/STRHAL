#include "../Inc/LID.h"

#include <stm32g4xx_ll_rcc.h>
#include <stm32g4xx_ll_cortex.h>
#include <stm32g4xx_ll_pwr.h>
#include <stm32g4xx_ll_utils.h>
#include <stm32g4xx_ll_bus.h>
#include <stm32g4xx_ll_system.h>

#define LID_SYSCLK_INT_PLL_M LL_RCC_PLLM_DIV_2
#define LID_SYSCLK_INT_PLL_R LL_RCC_PLLR_DIV_2

#define LID_SYSCLK_EXT_PLL_M LL_RCC_PLLM_DIV_1
#define LID_SYSCLK_EXT_PLL_R LL_RCC_PLLR_DIV_2

#define LID_SYSCLK_START_TOT 16000000

static LID_SysClk_Src_t _SysClk_Src = LID_SYSCLK_SRC_BKP;
static int _INITIALIZED = 0;
static LID_Oof_t _status = LID_NOICE;

static inline LID_SysClk_Src_t _SysClk_Init(LID_SysClk_Src_t src, uint32_t freq);
static inline LID_SysClk_Src_t _SysClk_Backup();

LID_Oof_t LID_Init(LID_SysClk_Src_t src, uint32_t freq) {
	if(_INITIALIZED)
		return _status;

	_status = LID_NOICE;

	NVIC_SetPriorityGrouping(0x03);
	if(_SysClk_Init(src, freq) != src)
		_status |= LID_OOF_SYSCLK;

	LID_Clock_Init();
	LID_SysTick_Init();
	LID_GPIO_Init();
	LID_OPAMP_Init();
	LID_UART_Init();
	LID_ADC_Init();
	LID_TIM_Init();
	_status |= LID_CAN_Init();
	LID_QSPI_Init(24, 7, 0);

	_INITIALIZED = 1;

	return _status;
}

inline LID_SysClk_Src_t _SysClk_Init(LID_SysClk_Src_t src, uint32_t freq) {
	uint32_t tot;
	if(src == LID_SYSCLK_SRC_INT) {
		if(LL_SetFlashLatency(LID_SYSCLK_FREQ) != SUCCESS)
			return _SysClk_Backup();

		LL_PWR_EnableRange1BoostMode();

		LL_RCC_PLL_Disable();
		LL_RCC_HSI_Enable();

		for(tot = 0; LL_RCC_PLL_IsReady() && !LL_RCC_HSI_IsReady(); ++tot) {
			if(tot > LID_SYSCLK_START_TOT)
				return _SysClk_Backup();
		}

		LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI,
				LID_SYSCLK_INT_PLL_M,
				4*LID_SYSCLK_FREQ/HSI_VALUE,
				LID_SYSCLK_INT_PLL_R
		);

		LL_RCC_PLL_EnableDomain_SYS();
		LL_RCC_PLL_Enable();

		for(tot = 0; !LL_RCC_PLL_IsReady(); ++tot) {
			if(tot > LID_SYSCLK_START_TOT)
				return _SysClk_Backup();
		}
		LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

		for(tot = 0; LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL; ++tot) {
			if(tot > LID_SYSCLK_START_TOT)
				return _SysClk_Backup();
		}

		LL_Init1msTick(LID_SYSCLK_FREQ);
		LL_SetSystemCoreClock(LID_SYSCLK_FREQ);
		_SysClk_Src = LID_SYSCLK_SRC_INT;
	}
	else if(src == LID_SYSCLK_SRC_EXT) {
		if(LL_SetFlashLatency(LID_SYSCLK_FREQ) != SUCCESS)
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
				2*LID_SYSCLK_FREQ/freq,
				LID_SYSCLK_EXT_PLL_R
		);
	    LL_RCC_PLL_EnableDomain_SYS();
	    LL_RCC_PLL_Enable();

	    for(tot = 0; !LL_RCC_PLL_IsReady(); ++tot) {
			if(tot > LID_SYSCLK_START_TOT)
				return _SysClk_Backup();
		}
		LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

		for(tot = 0; LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL; ++tot) {
			if(tot > LID_SYSCLK_START_TOT)
				return _SysClk_Backup();
		}

		LL_Init1msTick(LID_SYSCLK_FREQ);
		LL_SetSystemCoreClock(LID_SYSCLK_FREQ);
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

	_SysClk_Src = LID_SYSCLK_SRC_BKP;

	return _SysClk_Src;
}
