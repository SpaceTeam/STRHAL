#ifndef IOB_H_
#define IOB_H_

#define MAX_IOB_CHANNELS 12
#include "gpio.h"
//#define MAX_CHANNELS MAX_IOB_CHANNELS

#define IOB_MAIN_CAN_BUS 0


typedef struct
{
	GPIO_Pin_t enable;
	GPIO_Pin_t select;
	GPIO_Pin_t input;
} IOB_Pins_t;

void IOB_main(void);

#endif
