#ifndef LID_H_
#define LID_H_

#include <LID_Oof.h>
#include "LID_Clock.h"
#include "LID_GPIO.h"
#include "LID_ADC.h"
#include "LID_OPAMP.h"
#include "LID_UART.h"
#include "LID_TIM.h"
#include "LID_QSPI.h"
#include "LID_CAN.h"
#include "LID_SysTick.h"


#ifdef __cplusplus
extern "C" {
#endif

#define LID_SYSCLK_FREQ 160000000

typedef enum {
	LID_SYSCLK_SRC_BKP, //Backup SYSCLK SRC = ever working internal HSI without PLL
	LID_SYSCLK_SRC_INT, //Internal SYSCLK SRC = internal HSI + PLL
	LID_SYSCLK_SRC_EXT, //External SYSCLK SRC = external clock (HSE_BYPASS)
} LID_SysClk_Src_t;

LID_Oof_t LID_Init(LID_SysClk_Src_t src, uint32_t freq);


#ifdef __cplusplus
}
#endif

#endif /* LID_H_ */
