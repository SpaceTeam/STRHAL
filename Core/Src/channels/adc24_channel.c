#include "adc24_channel.h"
#include "main.h"
#include "channel_util.h"
#include "channels.h"
#include "ads131.h"
#include "ui.h"

Result_t Adc24_ResetSettings(Channel_t *channel);
Result_t Adc24_Status(Channel_t *channel);
Result_t Adc24_SetVariable(Channel_t *channel, SetMsg_t *set_msg);
Result_t Adc24_GetVariable(Channel_t *channel, GetMsg_t *get_msg, ADC24_CMDs response_cmd);

int32_t* Adc24_VariableSelection(Adc24_Channel_t *adc24, uint8_t var_id)
{
	switch (var_id)
	{

		case ADC24_LOWER_THRESHOLD:
			return &adc24->thresholds[0];
		case ADC24_UPPER_THRESHOLD:
			return &adc24->thresholds[1];
		case ADC24_REFRESH_DIVIDER:
			return NULL;
		default:
			return NULL;
	}
}

Result_t Adc24_ResetSettings(Channel_t *channel)
{

	return OOF;
}

Result_t Adc24_Status(Channel_t *channel)
{

	return OOF;
}

Result_t Adc24_SetVariable(Channel_t *channel, SetMsg_t *set_msg)
{

	int32_t *var = Adc24_VariableSelection(&channel->channel.adc24, set_msg->variable_id);
	if (var == NULL)
		return OOF;
	*var = set_msg->value;
	return Adc24_GetVariable(channel, (GetMsg_t*) set_msg, ADC24_RES_SET_VARIABLE);
}

Result_t Adc24_GetVariable(Channel_t *channel, GetMsg_t *get_msg, ADC24_CMDs response_cmd)
{
	return ChannelUtil_GetVariable(channel, get_msg, response_cmd);
}

Result_t Adc24_ProcessMessage(uint8_t ch_id, uint8_t cmd_id, uint8_t *data, uint32_t length)
{

	if (node.channels[ch_id].type != CHANNEL_TYPE_ADC24)
		return OOF_WRONG_CHANNEL_TYPE;
	Channel_t *channel = &node.channels[ch_id];

	switch (cmd_id)
	{
		case ADC24_REQ_RESET_SETTINGS:
			return Adc24_ResetSettings(channel);
		case ADC24_REQ_SET_VARIABLE:
			return Adc24_SetVariable(channel, (SetMsg_t*) data);
		case ADC24_REQ_GET_VARIABLE:
			return Adc24_GetVariable(channel, (GetMsg_t*) data, ADC24_RES_GET_VARIABLE);
		case ADC24_REQ_CALIBRATE:
			return OOF;
		default:
			return OOF_UNKNOWN_CMD;
	}
}

Result_t Adc24_GetData(uint8_t ch_id, uint8_t *data, uint32_t *length)
{
	uint32_t *out = (uint32_t*) (data + *length);
	int32_t new_data = Ads131_GetData(ch_id);
	if (new_data == ADS131_NO_NEW_DATA)
		return OOF_NO_NEW_DATA;
	out[0] = (uint8_t)((new_data >>  0) & 0xFF);
	out[1] = (uint8_t)((new_data >>  8) & 0xFF);
	out[2] = (uint8_t)((new_data >> 16) & 0xFF);
/*	if (ch_id == 0)
		Serial_PrintInt(new_data);*/
#ifdef DEBUG_DATA
	Serial_PutInt(new_data);
	Serial_PutString(", ");
#endif
	*length += ADC24_DATA_N_BYTES;
	return NOICE;
}

