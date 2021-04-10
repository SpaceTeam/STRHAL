#ifndef ADC16_CHANNEL_H_
#define ADC16_CHANNEL_H_

#include "adc16_channel_def.h"
#include "measurement.h"

typedef struct
{
	int32_t offset;
	int32_t thresholds[2];

	Measurement *measurement;

} Adc16_Channel_t;

#endif
