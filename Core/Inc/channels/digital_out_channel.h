#ifndef DIGITAL_OUT_CHANNEL_H_
#define DIGITAL_OUT_CHANNEL_H_

#include "digital_out_channel_def.h"
#include "main.h"
#include "measurement.h"
#include "gpio.h"
#include "adc.h"

typedef struct
{
	uint16_t duty_cycle;
	uint16_t frequency;
	GPIO_Pin_t * enable_pin;
	AdcData_t * analog_in;
} DigitalOut_Channel_t;

Result_t DigitalOut_ProcessMessage(uint8_t ch_id, uint8_t cmd_id, uint8_t *data, uint32_t length);

#endif
