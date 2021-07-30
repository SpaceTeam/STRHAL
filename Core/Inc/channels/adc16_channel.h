#ifndef ADC16_CHANNEL_H_
#define ADC16_CHANNEL_H_

#include "adc16_channel_def.h"
#include "measurement.h"
#include "main.h"
#include "adc.h"

typedef struct
{
	int32_t offset;
	int32_t thresholds[2];
	AdcData_t * analog_in;
	GPIO_Pin_t * enable_pin;
} Adc16_Channel_t;

Result_t Adc16_ProcessMessage(uint8_t ch_id, uint8_t cmd_id, uint8_t *data, uint32_t length);
Result_t Adc16_GetData(uint8_t ch_id, uint8_t *data, uint32_t *length);

#endif
