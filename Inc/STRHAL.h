

#ifndef STM32H7XX_STRHAL_INC_STRHAL_H_
#define STM32H7XX_STRHAL_INC_STRHAL_H_


#include "STRHAL_Oof.h"
#include "STRHAL_Clock.h"
#include "STRHAL_GPIO.h"
#include "STRHAL_ADC.h"
#include "STRHAL_OPAMP.h"
#include "STRHAL_UART.h"
#include "STRHAL_TIM.h"
#include "STRHAL_SPI.h"
#include "STRHAL_QSPI.h"
#include "STRHAL_CAN.h"
#include "STRHAL_SysTick.h"


#ifdef __cplusplus
extern "C" {
#endif

//when increasing frequency, peripheral clock configuration must be changed!
#define STRHAL_SYSCLK_FREQ 240000000

typedef enum {
	STRHAL_SYSCLK_SRC_BKP, //Backup SYSCLK SRC = ever working internal HSI without PLL
	STRHAL_SYSCLK_SRC_INT, //Internal SYSCLK SRC = internal HSI + PLL
	STRHAL_SYSCLK_SRC_EXT, //External SYSCLK SRC = external clock (HSE_BYPASS)
} STRHAL_SysClk_Src_t;

STRHAL_Oof_t STRHAL_Init(STRHAL_SysClk_Src_t src, uint32_t freq);


#ifdef __cplusplus
}
#endif


#endif /* STM32H7XX_STRHAL_INC_STRHAL_H_ */
