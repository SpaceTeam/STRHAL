#include "pneumatic_valve_channel.h"
#include "main.h"
#include "channel_util.h"
#include "channels.h"
#include "ui.h"

#include "serial.h"
#include <string.h>
#include <stdio.h>

Result_t PneumaticValve_ResetSettings(Channel_t *channel);
Result_t PneumaticValve_Status(Channel_t *channel);
Result_t PneumaticValve_SetVariable(Channel_t *channel, SetMsg_t *set_msg);
Result_t PneumaticValve_GetVariable(Channel_t *channel, GetMsg_t *get_msg, PNEUMATIC_VALVE_CMDs response_cmd);

static uint32_t readonly_var = 0;
uint32_t* PneumaticValve_VariableSelection(PneumaticValve_Channel_t *pneumatic_valve, uint8_t var_id, uint8_t ch_id)
{
	readonly_var = 0;
	switch (var_id)
	{
		case PNEUMATIC_VALVE_POSITION:
			readonly_var = *node.channels[pneumatic_valve->pos_channel_id].channel.adc16.analog_in;
			return &readonly_var;
		case PNEUMATIC_VALVE_TARGET_POSITION:
			return &pneumatic_valve->target_position;
		case PNEUMATIC_VALVE_P_PARAM:
			return &pneumatic_valve->p_param;
		case PNEUMATIC_VALVE_I_PARAM:
			return &pneumatic_valve->i_param;
		case PNEUMATIC_VALVE_D_PARAM:
			return &pneumatic_valve->d_param;
		case PNEUMATIC_VALVE_ON_CHANNEL_ID:
			return &pneumatic_valve->on_channel_id;
		case PNEUMATIC_VALVE_OFF_CHANNEL_ID:
			return &pneumatic_valve->off_channel_id;
		case PNEUMATIC_VALVE_POS_CHANNEL_ID:
			return &pneumatic_valve->pos_channel_id;
		case PNEUMATIC_VALVE_POS_REFRESH_DIVIDER:
			return &pneumatic_valve->refresh_divider;
		default:
			return NULL;
	}
}

Result_t PneumaticValve_ResetSettings(Channel_t *channel)
{

	return OOF_NOT_IMPLEMENTED;
}

Result_t PneumaticValve_Status(Channel_t *channel)
{

	return OOF_NOT_IMPLEMENTED;
}

Result_t PneumaticValve_SetVariable(Channel_t *channel, SetMsg_t *set_msg)
{
	uint32_t *var = PneumaticValve_VariableSelection(&channel->channel.pneumatic_valve, set_msg->variable_id, channel->id);
	if (var == NULL)
		return OOF;
	*var = set_msg->value;
	return PneumaticValve_GetVariable(channel, (GetMsg_t*) set_msg, PNEUMATIC_VALVE_RES_SET_VARIABLE);
}

Result_t PneumaticValve_GetVariable(Channel_t *channel, GetMsg_t *get_msg, PNEUMATIC_VALVE_CMDs response_cmd)
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
	uint32_t *var = PneumaticValve_VariableSelection(&channel->channel.pneumatic_valve, get_msg->variable_id, channel->id);
	if (var == NULL)
		return OOF;
	set_msg->variable_id = get_msg->variable_id;
	set_msg->value = *var;

#ifdef DEBUG_DATA

	char serial_str[20] =
	{ 0 };

	//sprintf(serial_str,"%d: %d, ",channel->id, *var);
	sprintf(serial_str, "%d,", *var);
	Serial_PutString(serial_str);
#endif
	return Ui_SendCanMessage( MAIN_CAN_BUS, message_id, &data, sizeof(SetMsg_t));
}
Result_t PneumaticValve_Move(Channel_t *channel, PneumaticValveMoveMsg_t *move_msg)
{


	return OOF_NOT_IMPLEMENTED;
}
Result_t PneumaticValve_ProcessMessage(uint8_t ch_id, uint8_t cmd_id, uint8_t *data, uint32_t length)
{
	if (node.channels[ch_id].type != CHANNEL_TYPE_DIGITAL_OUT)
		return OOF_WRONG_CHANNEL_TYPE;
	Channel_t *channel = &node.channels[ch_id];

	switch (cmd_id)
	{
		case PNEUMATIC_VALVE_REQ_RESET_SETTINGS:
			return PneumaticValve_ResetSettings(channel);
		case PNEUMATIC_VALVE_REQ_STATUS:
			return PneumaticValve_Status(channel);
		case PNEUMATIC_VALVE_REQ_SET_VARIABLE:
			return PneumaticValve_SetVariable(channel, (SetMsg_t*) data);
		case PNEUMATIC_VALVE_REQ_GET_VARIABLE:
			return PneumaticValve_GetVariable(channel, (GetMsg_t*) data, DIGITAL_OUT_RES_GET_VARIABLE);
		case PNEUMATIC_VALVE_REQ_MOVE:
			return PneumaticValve_Move(channel, (PneumaticValveMoveMsg_t*) data);
		default:
			return OOF_UNKNOWN_CMD;
	}
}

static Result_t PneumaticValve_GetRawData(uint8_t channel_id, uint16_t *data)
{
	*data = (uint16_t) *node.channels[channel_id].channel.digital_out.analog_in;
	//TODO @ANDI if (No new data)  return OOF_NO_NEW_DATA;
	return NOICE;
}

Result_t PneumaticValve_GetData(uint8_t ch_id, uint8_t *data, uint32_t *length)
{
	uint16_t *out = (uint16_t*) (data + *length);
	uint16_t new_data;
	Result_t result = PneumaticValve_GetRawData(ch_id, &new_data);
	if (result != NOICE)
		return result;
	*out = new_data;
#ifdef DEBUG_DATA
	Serial_PutInt(new_data);
	Serial_PutString(", ");
#endif
	(*length) += DIGITAL_OUT_DATA_N_BYTES;
	return NOICE;
}

