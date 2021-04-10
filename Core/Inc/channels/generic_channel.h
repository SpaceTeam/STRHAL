#ifndef GENERIC_CHANNEL_H_
#define GENERIC_CHANNEL_H_

#include "generic_channel_def.h"
#include "main.h"

typedef struct
{
	int32_t bus1_voltage;
	int32_t bus2_voltage;
	int32_t power_voltage;
	int32_t power_current;
	int32_t refresh_divider;
	int32_t refresh_rate;
} Generic_Channel_t;

Result_t Generic_Data(void);
Result_t Generic_NodeInfo();

Result_t Generic_ProcessMessage(uint8_t channel, uint8_t cmd_id, uint8_t *data, uint32_t length);

#endif
