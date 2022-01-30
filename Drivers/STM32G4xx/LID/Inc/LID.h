#ifndef LID_H_
#define LID_H_

#include "LID_Clock.h"
#include "LID_GPIO.h"
#include "LID_ADC.h"
#include "LID_OPAMP.h"
#include "LID_UART.h"

typedef enum {
	LID_SYSCLK_SRC_BKP, //Backup SYSCLK SRC = ever working internal HSI without PLL
	LID_SYSCLK_SRC_INT, //Internal SYSCLK SRC = internal HSI + PLL
	LID_SYSCLK_SRC_EXT, //External SYSCLK SRC = external clock (HSE_BYPASS)
} LID_SysClk_Src_t;

typedef enum {
	LID_SYSCLK_ERR= (1<<0),
	LID_CLOCK_ERR = (1<<1),
	LID_GPIO_ERR = (1<<2),
	LID_NOICE
} LID_Status_t;

LID_Status_t LID_Init(LID_SysClk_Src_t src, uint32_t freq);

#endif /* LID_H_ */
