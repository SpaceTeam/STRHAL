#include "digital_out_channel.h"
#include "main.h"
#include "channel_util.h"
#include "channels.h"
#include "adc.h"
#include "ui.h"

#include "serial.h"
#include <string.h>
#include <stdio.h>

Result_t DigitalOut_ResetSettings(Channel_t *channel);
Result_t DigitalOut_Status(Channel_t *channel);
Result_t DigitalOut_SetVariable(Channel_t *channel, SetMsg_t *set_msg);
Result_t DigitalOut_GetVariable(Channel_t *channel, GetMsg_t *get_msg, DIGITAL_OUT_CMDs response_cmd);
uint16_t* DigitalOut_VariableSelection(DigitalOut_Channel_t *dig_out, uint8_t var_id, uint8_t ch_id);

uint16_t* DigitalOut_VariableSelection(DigitalOut_Channel_t *dig_out, uint8_t var_id, uint8_t ch_id)
{
	switch (var_id)
	{

		case DIGITAL_OUT_STATE:
			return &dig_out->state;
		case DIGITAL_OUT_DUTY_CYCLE:
			return &dig_out->duty_cycle;
		case DIGITAL_OUT_FREQUENCY:
			return &dig_out->frequency;
		case DIGITAL_OUT_MEASUREMENT:
			return dig_out->analog_in;
		case DIGITAL_OUT_SENSOR_REFRESH_DIVIDER:
			return NULL;
		default:
			return NULL;
	}
}

Result_t DigitalOut_ResetSettings(Channel_t *channel)
{

	return OOF;
}

Result_t DigitalOut_Status(Channel_t *channel)
{

	return OOF;
}

Result_t DigitalOut_SetVariable(Channel_t *channel, SetMsg_t *set_msg)
{
	if (set_msg->variable_id == DIGITAL_OUT_STATE || set_msg->variable_id == DIGITAL_OUT_MEASUREMENT) //cannot set state, read-only
		return OOF;

	if (set_msg->variable_id == DIGITAL_OUT_DUTY_CYCLE)
	{
		GPIO_Pin_t *enable = channel->channel.digital_out.enable_pin;
		if (set_msg->value == 0)
		{
			channel->channel.digital_out.state = 0;
			LL_GPIO_ResetOutputPin(enable->port,enable->pin);
		}
		else if (set_msg->value == 0xFFFFFFFF)
		{
			channel->channel.digital_out.state = 1;
			LL_GPIO_SetOutputPin(enable->port,enable->pin);
		}
		else
		{
			//TODO: Add PWM
		}
	}

	uint16_t *var = DigitalOut_VariableSelection(&channel->channel.digital_out, set_msg->variable_id, channel->id);
	if (var == NULL)
		return OOF;
	*var = set_msg->value;
	return DigitalOut_GetVariable(channel, (GetMsg_t*) set_msg, DIGITAL_OUT_RES_SET_VARIABLE);
}

Result_t DigitalOut_GetVariable(Channel_t *channel, GetMsg_t *get_msg, DIGITAL_OUT_CMDs response_cmd)
{

	Can_MessageId_t message_id =
	{ 0 };
	ChannelUtil_DefaultMessageId(&message_id);

	Can_MessageData_t data =
	{ 0 };

	data.bit.cmd_id = response_cmd;
	data.bit.info.channel_id = channel->id;
	data.bit.info.buffer = DIRECT_BUFFER;

	SetMsg_t *set_msg = (SetMsg_t*) &data.bit.data;
	uint16_t *var = DigitalOut_VariableSelection(&channel->channel.digital_out, get_msg->variable_id, channel->id);
	if (var == NULL)
		return OOF;
	set_msg->variable_id = get_msg->variable_id;
	set_msg->value = *var;
	char serial_str[20] =
	{ 0 };

	sprintf(serial_str,"ADC%d: %d", channel->id, *var);
	Serial_PrintString(serial_str);


	return Ui_SendCanMessage( MAIN_CAN_BUS, message_id, &data, sizeof(SetMsg_t));
}


Result_t DigitalOut_ProcessMessage(uint8_t ch_id, uint8_t cmd_id, uint8_t *data, uint32_t length)
{
	if (node.channels[ch_id].type != CHANNEL_TYPE_DIGITAL_OUT)
		return OOF_WRONG_CHANNEL_TYPE;
	Channel_t *channel = &node.channels[ch_id];

	switch (cmd_id)
	{
		case DIGITAL_OUT_REQ_RESET_SETTINGS:
			return DigitalOut_ResetSettings(channel);
		case DIGITAL_OUT_REQ_STATUS:
			return DigitalOut_Status(channel);
		case DIGITAL_OUT_REQ_SET_VARIABLE:
			return DigitalOut_SetVariable(channel, (SetMsg_t*) data);
		case DIGITAL_OUT_REQ_GET_VARIABLE:
			return DigitalOut_GetVariable(channel, (GetMsg_t*) data, DIGITAL_OUT_RES_GET_VARIABLE);
		default:
			return OOF_UNKNOWN_CMD;
	}
}
