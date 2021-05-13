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

	Can_MessageId_t message_id =
	{ 0 };
	ChannelUtil_DefaultMessageId(&message_id);

	Can_MessageData_t data =
	{ 0 };

	data.bit.cmd_id = response_cmd;
	data.bit.info.channel_id = channel->id;
	data.bit.info.buffer = DIRECT_BUFFER;

	SetMsg_t *set_msg = (SetMsg_t*) &data.bit.data;
	int32_t *var = Adc24_VariableSelection(&channel->channel.adc24, get_msg->variable_id);
	if (var == NULL)
		return OOF;
	set_msg->variable_id = get_msg->variable_id;
	set_msg->value = *var;

	return Ui_SendCanMessage( MAIN_CAN_BUS, message_id, &data, sizeof(SetMsg_t));
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

Result_t Adc24_GetData(uint8_t ch_id, int32_t *data, uint32_t *length)
{
	int32_t new_data = Ads131_GetData(ch_id);
	if (new_data == ADS131_NO_NEW_DATA)
		return OOF_NO_NEW_DATA;
	*data = new_data;
	*length += ADC24_DATA_N_BYTES;
	return NOICE;
}

