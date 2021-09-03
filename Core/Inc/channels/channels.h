#ifndef CHANNELS_H_
#define CHANNELS_H_

#include "board_config.h"

#include "generic_channel.h"
#include "adc16_channel.h"
#include "adc16_single_channel.h"
#include "adc24_channel.h"
#include "digital_out_channel.h"
#include "servo_channel.h"
#include "pneumatic_valve_channel.h"

typedef struct
{
	uint8_t id;
	CHANNEL_TYPE type;
	union
	{
		Adc16_Channel_t adc16;
		Adc16Single_Channel_t adc16single;
		Adc24_Channel_t adc24;
		DigitalOut_Channel_t digital_out;
		Servo_Channel_t servo;
		PneumaticValve_Channel_t pneumatic_valve;
	} channel;
} Channel_t;

typedef struct
{
	uint32_t node_id;
	uint32_t firmware_version;
	Generic_Channel_t generic_channel;
	Channel_t channels[MAX_CHANNELS];
} Node_t;

extern Node_t node;

#endif
