#ifndef IOB_H_
#define IOB_H_

#define MAX_STANDARD_IOB_CHANNELS 12
#define PNEUMATIC_VALVE_1_CHANNEL_ID (MAX_STANDARD_IOB_CHANNELS)
#define PNEUMATIC_VALVE_2_CHANNEL_ID (MAX_STANDARD_IOB_CHANNELS + 1)

#define MAX_IOB_CHANNELS (MAX_STANDARD_IOB_CHANNELS + 2)
#include "gpio.h"

#define IOB_MAIN_CAN_BUS 0


typedef struct
{
	GPIO_Pin_t enable;
	GPIO_Pin_t select;
	GPIO_Pin_t input;
} IOB_Pins_t;

IOB_Pins_t iob_channels[12];

void IOB_TIM2_IRQHandler(void);
void IOB_main(void);

#endif
