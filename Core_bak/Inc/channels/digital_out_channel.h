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

uint16_t* DigitalOut_VariableSelection(DigitalOut_Channel_t *dig_out, uint8_t var_id, uint8_t ch_id);
Result_t DigitalOut_ProcessMessage(uint8_t ch_id, uint8_t cmd_id, uint8_t *data, uint32_t length);
uint8_t DigitalOut_GetState(uint8_t channel_id);
void DigitalOut_SetState(DigitalOut_Channel_t *digital_out, uint32_t state);
Result_t DigitalOut_GetData(uint8_t ch_id, uint8_t *data, uint32_t *length);

#endif
