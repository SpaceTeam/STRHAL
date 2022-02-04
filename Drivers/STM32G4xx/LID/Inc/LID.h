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

typedef enum {
	LID_SYSCLK_SRC_BKP, //Backup SYSCLK SRC = ever working internal HSI without PLL
	LID_SYSCLK_SRC_INT, //Internal SYSCLK SRC = internal HSI + PLL
	LID_SYSCLK_SRC_EXT, //External SYSCLK SRC = external clock (HSE_BYPASS)
} LID_SysClk_Src_t;

LID_Oof_t LID_Init(LID_SysClk_Src_t src, uint32_t freq);

#endif /* LID_H_ */
