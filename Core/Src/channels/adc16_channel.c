#include "adc16_channel.h"
#include "main.h"
#include "channel_util.h"
#include "channels.h"
#include "ui.h"

#include "serial.h"
#include <string.h>
#include <stdio.h>

Result_t Adc16_ResetSettings(Channel_t *channel);
Result_t Adc16_Status(Channel_t *channel);
Result_t Adc16_SetVariable(Channel_t *channel, SetMsg_t *set_msg);
Result_t Adc16_GetVariable(Channel_t *channel, GetMsg_t *get_msg, ADC16_CMDs response_cmd);
uint16_t* Adc16_VariableSelection(Adc16_Channel_t *adc16, uint8_t var_id, uint8_t ch_id);

static uint16_t readonly_var = 0;
uint16_t* Adc16_VariableSelection(Adc16_Channel_t *adc16, uint8_t var_id, uint8_t ch_id)
{
	readonly_var = 0;
	switch (var_id)
	{
		case ADC16_MEASUREMENT:
			readonly_var = *adc16->analog_in;
			///readonly_var = (adc16->is_single) ? adc16->data : *adc16->analog_in;
			return &readonly_var;
		case ADC16_REFRESH_DIVIDER:
			return NULL;
		default:
			return NULL;
	}
}

Result_t Adc16_ResetSettings(Channel_t *channel)
{

	return OOF;
}

Result_t Adc16_Status(Channel_t *channel)
{

	return OOF;
}

Result_t Adc16_SetVariable(Channel_t *channel, SetMsg_t *set_msg)
{
	if (set_msg->variable_id == ADC16_MEASUREMENT) //cannot set state, read-only
		return OOF;

	uint16_t *var = Adc16_VariableSelection(&channel->channel.adc16, set_msg->variable_id, channel->id);
	if (var == NULL)
		return OOF;
	*var = set_msg->value;
	return Adc16_GetVariable(channel, (GetMsg_t*) set_msg, ADC16_RES_SET_VARIABLE);
}

Result_t Adc16_GetVariable(Channel_t *channel, GetMsg_t *get_msg, ADC16_CMDs response_cmd)
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
	uint16_t *var = Adc16_VariableSelection(&channel->channel.adc16, get_msg->variable_id, channel->id);
	if (var == NULL)
		return OOF;
	set_msg->variable_id = get_msg->variable_id;
	set_msg->value = *var;
	char serial_str[20] =
	{ 0 };

	sprintf(serial_str,"%d, ", *var);
	Serial_PutString(serial_str);


	return Ui_SendCanMessage( MAIN_CAN_BUS, message_id, &data, sizeof(SetMsg_t));
}

Result_t Adc16_ProcessMessage(uint8_t ch_id, uint8_t cmd_id, uint8_t *data, uint32_t length)
{

	if (node.channels[ch_id].type != CHANNEL_TYPE_ADC16)
		return OOF_WRONG_CHANNEL_TYPE;
	Channel_t *channel = &node.channels[ch_id];

	switch (cmd_id)
	{
		case ADC16_REQ_RESET_SETTINGS:
			return Adc16_ResetSettings(channel);
		case ADC16_REQ_STATUS:
			return Adc16_Status(channel);
		case ADC16_REQ_SET_VARIABLE:
			return Adc16_SetVariable(channel, (SetMsg_t*) data);
		case ADC16_REQ_GET_VARIABLE:
			return Adc16_GetVariable(channel, (GetMsg_t*) data, ADC16_RES_GET_VARIABLE);
		case ADC16_REQ_CALIBRATE:
			return OOF;
		default:
			return OOF_UNKNOWN_CMD;
	}
}

/*Result_t Adc16_GetData(uint8_t ch_id, int32_t *data, uint32_t *length)
{
	return OOF;
}*/
