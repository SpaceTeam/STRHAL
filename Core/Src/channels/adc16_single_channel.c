#include "adc16_single_channel.h"
#include "main.h"
#include "channel_util.h"
#include "channels.h"
#include "ui.h"

#include "serial.h"
#include <string.h>
#include <stdio.h>

Result_t Adc16Single_ResetSettings(Channel_t *channel);
Result_t Adc16Single_Status(Channel_t *channel);
Result_t Adc16Single_SetVariable(Channel_t *channel, SetMsg_t *set_msg);
Result_t Adc16Single_GetVariable(Channel_t *channel, GetMsg_t *get_msg, ADC16_SINGLE_CMDs response_cmd);

static uint16_t readonly_var = 0;
uint16_t* Adc16Single_VariableSelection(Adc16Single_Channel_t *adc16_single, uint8_t var_id, uint8_t ch_id)
{
	readonly_var = 0;
	switch (var_id)
	{
		case ADC16_SINGLE_MEASUREMENT:
			readonly_var = *adc16_single->analog_in;
			return &readonly_var;
		case ADC16_SINGLE_DATA:
			readonly_var = *adc16_single->last_measurement;
			return &readonly_var;
		case ADC16_SINGLE_REFRESH_DIVIDER:
			return NULL;
		default:
			return NULL;
	}
}

Result_t Adc16Single_ResetSettings(Channel_t *channel)
{

	return OOF;
}

Result_t Adc16Single_Status(Channel_t *channel)
{

	return OOF;
}

Result_t Adc16Single_SetVariable(Channel_t *channel, SetMsg_t *set_msg)
{
	if (set_msg->variable_id == ADC16_SINGLE_MEASUREMENT) //cannot set state, read-only
		return OOF;

	uint16_t *var = Adc16Single_VariableSelection(&channel->channel.adc16single, set_msg->variable_id, channel->id);
	if (var == NULL)
		return OOF;
	*var = set_msg->value;
	return Adc16Single_GetVariable(channel, (GetMsg_t*) set_msg, ADC16_RES_SET_VARIABLE);
}

Result_t Adc16Single_GetVariable(Channel_t *channel, GetMsg_t *get_msg, ADC16_SINGLE_CMDs response_cmd)
{
	return ChannelUtil_GetVariable(channel, get_msg, response_cmd);
}

Result_t Adc16Single_ProcessMessage(uint8_t ch_id, uint8_t cmd_id, uint8_t *data, uint32_t length)
{

	if (node.channels[ch_id].type != CHANNEL_TYPE_ADC16_SINGLE)
		return OOF_WRONG_CHANNEL_TYPE;
	Channel_t *channel = &node.channels[ch_id];

	switch (cmd_id)
	{
		case ADC16_SINGLE_REQ_RESET_SETTINGS:
			return Adc16Single_ResetSettings(channel);
		case ADC16_SINGLE_REQ_STATUS:
			return Adc16Single_Status(channel);
		case ADC16_SINGLE_REQ_SET_VARIABLE:
			return Adc16Single_SetVariable(channel, (SetMsg_t*) data);
		case ADC16_SINGLE_REQ_GET_VARIABLE:
			return Adc16Single_GetVariable(channel, (GetMsg_t*) data, ADC16_SINGLE_RES_GET_VARIABLE);
		case ADC16_SINGLE_REQ_CALIBRATE:
			return OOF;
		default:
			return OOF_UNKNOWN_CMD;
	}
}

static Result_t Adc16Single_GetRawData(uint8_t channel_id, uint16_t *data)
{
	*data = (uint16_t) *node.channels[channel_id].channel.adc16single.last_measurement;
	//TODO @ANDI if (No new data and/or refresh divider count)  return OOF_NO_NEW_DATA;
	return NOICE;
}

Result_t Adc16Single_GetData(uint8_t ch_id, uint8_t *data, uint32_t *length)
{
	uint16_t *out = (uint16_t*) (data + *length);
	uint16_t new_data;
	Result_t result = Adc16Single_GetRawData(ch_id, &new_data);
	if (result != NOICE)
		return result;
	*out = new_data;

#ifdef DEBUG_DATA
	Serial_PutInt(new_data);
	Serial_PutString(", ");
#endif
	(*length) += ADC16_DATA_N_BYTES;
	return NOICE;
}
