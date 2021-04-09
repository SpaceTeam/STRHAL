#ifndef GENERIC_CHANNEL_H_
#define GENERIC_CHANNEL_H_

#include "generic_channel_def.h"
#include "main.h"

typedef struct
{
	uint32_t bus1_voltage;
	uint32_t bus2_voltage;
	uint32_t power_voltage;
	uint32_t power_current;
	uint32_t refresh_divider;
	uint32_t refresh_rate;
} Generic_Channel_t;

Result_t Generic_ProcessMessage(uint8_t channel, uint8_t cmd_id, uint8_t *data, uint32_t length);

#endif
