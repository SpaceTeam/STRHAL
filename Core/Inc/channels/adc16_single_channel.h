#ifndef ADC16_SINGLE_CHANNEL_H_
#define ADC16_SINGLE_CHANNEL_H_

#include "adc16_single_channel_def.h"
#include "measurement.h"
#include "main.h"
#include "adc.h"

typedef struct
{
	int32_t offset;
	int32_t thresholds[2];
	AdcData_t * analog_in;
	AdcData_t * last_measurement;
	GPIO_Pin_t * enable_pin;
} Adc16Single_Channel_t;

Result_t Adc16Single_ProcessMessage(uint8_t ch_id, uint8_t cmd_id, uint8_t *data, uint32_t length);
Result_t Adc16Single_GetData(uint8_t ch_id, uint8_t *data, uint32_t *length);

#endif
