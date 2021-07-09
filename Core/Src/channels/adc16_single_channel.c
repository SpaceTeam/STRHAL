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
uint16_t* Adc16Single_VariableSelection(Adc16Single_Channel_t *adc16, uint8_t var_id, uint8_t ch_id);

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

	Can_MessageId_t message_id =
	{ 0 };
	ChannelUtil_DefaultMessageId(&message_id);

	Can_MessageData_t data =
	{ 0 };

	data.bit.cmd_id = response_cmd;
	data.bit.info.channel_id = channel->id;
	data.bit.info.buffer = DIRECT_BUFFER;

	SetMsg_t *set_msg = (SetMsg_t*) &data.bit.data;
	uint16_t *var = Adc16Single_VariableSelection(&channel->channel.adc16single, get_msg->variable_id, channel->id);
	if (var == NULL)
		return OOF;
	set_msg->variable_id = get_msg->variable_id;
	set_msg->value = *var;
	/*
	//TODO @ANDI Move to Debug_PrintInt or something like that
	char serial_str[20] =
	{ 0 };
	//sprintf(serial_str,"%d: %d, ",channel->id, *var);
	sprintf(serial_str,"%d,", *var);
	Serial_PutString(serial_str);
*/

	return Ui_SendCanMessage( MAIN_CAN_BUS, message_id, &data, sizeof(SetMsg_t));
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

/*Result_t Adc16_GetData(uint8_t ch_id, int32_t *data, uint32_t *length)
{
//TODO @ANDI
	return OOF;
}*/
