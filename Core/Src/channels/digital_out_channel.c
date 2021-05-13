#include "digital_out_channel.h"
#include "main.h"
#include "channel_util.h"
#include "channels.h"
#include "ui.h"

Result_t DigitalOut_ResetSettings(Channel_t *channel);
Result_t DigitalOut_SetVariable(Channel_t *channel, SetMsg_t *set_msg);
Result_t DigitalOut_GetVariable(Channel_t *channel, GetMsg_t *get_msg, ADC24_CMDs response_cmd);
int32_t* DigitalOut_VariableSelection(DigitalOut_Channel_t *dig_out, uint8_t var_id);

int32_t* DigitalOut_VariableSelection(DigitalOut_Channel_t *dig_out, uint8_t var_id)
{
	switch (var_id)
	{

		case DIGITAL_OUT_STATE:
			return &dig_out->state;
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

Result_t DigitalOut_SetVariable(Channel_t *channel, SetMsg_t *set_msg)
{

	int32_t *var = DigitalOut_VariableSelection(&channel->channel.digital_out, set_msg->variable_id);
	if (var == NULL)
		return OOF;
	*var = set_msg->value;
	return DigitalOut_GetVariable(channel, (GetMsg_t*) set_msg, DIGITAL_OUT_RES_SET_VARIABLE);
}

Result_t DigitalOut_GetVariable(Channel_t *channel, GetMsg_t *get_msg, ADC24_CMDs response_cmd)
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
	int32_t *var = DigitalOut_VariableSelection(&channel->channel.digital_out, get_msg->variable_id);
	if (var == NULL)
		return OOF;
	set_msg->variable_id = get_msg->variable_id;
	set_msg->value = *var;

	return Ui_SendCanMessage( MAIN_CAN_BUS, message_id, &data, sizeof(SetMsg_t));
}


Result_t DigitalOut_ProcessMessage(uint8_t ch_id, uint8_t cmd_id, uint8_t *data, uint32_t length)
{
	if (node.channels[ch_id].type != CHANNEL_TYPE_DIGITAL_OUT)
		return OOF_WRONG_CHANNEL_TYPE;
	Channel_t *channel = &node.channels[ch_id];

	switch (cmd_id)
	{
		case DIGITAL_OUT_RESET_SETTINGS:
			return DigitalOut_ResetSettings(channel);
		case DIGITAL_OUT_REQ_SET_VARIABLE:
			return DigitalOut_SetVariable(channel, (SetMsg_t*) data);
		case DIGITAL_OUT_REQ_GET_VARIABLE:
			return DigitalOut_GetVariable(channel, (GetMsg_t*) data, DIGITAL_OUT_RES_GET_VARIABLE);
		case DIGITAL_OUT_PWM_ENABLE:
			return OOF;
		default:
			return OOF_UNKNOWN_CMD;
	}
}
