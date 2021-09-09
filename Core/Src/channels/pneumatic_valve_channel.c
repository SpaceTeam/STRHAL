#include "pneumatic_valve_channel.h"
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

Result_t PneumaticValve_InitChannel(PneumaticValve_Channel_t *pneumatic_valve, uint32_t on_channel_id, uint32_t off_channel_id, uint32_t pos_channel_id)
{
	if (node.node_id == 5) //TODO STORE IN FLASH
	{
		pneumatic_valve->enable = 1;
		pneumatic_valve->on_channel_id = on_channel_id;
		pneumatic_valve->off_channel_id = off_channel_id;
		pneumatic_valve->pos_channel_id = pos_channel_id;
		pneumatic_valve->target_position = 40000;
		pneumatic_valve->refresh_divider = 0;
		pneumatic_valve->threshold = PNEUMATIC_VALVE_DEFAULT_THRESHOLD; //TODO LOAD CONFIG
		pneumatic_valve->hysteresis = PNEUMATIC_VALVE_DEFAULT_HYSTERESIS; //TODO LOAD CONFIG

		if (node.channels[on_channel_id].type != CHANNEL_TYPE_DIGITAL_OUT)
			return OOF;
		if (node.channels[off_channel_id].type != CHANNEL_TYPE_DIGITAL_OUT)
			return OOF;
		if (node.channels[pos_channel_id].type != CHANNEL_TYPE_ADC16)
			return OOF;
	}
	return NOICE;
}

static uint32_t readonly_var = 0;
uint32_t* PneumaticValve_VariableSelection(PneumaticValve_Channel_t *pneumatic_valve, uint8_t var_id, uint8_t ch_id)
{
	readonly_var = 0;
	switch (var_id)
	{
		case PNEUMATIC_VALVE_ENABLE:
			return &pneumatic_valve->enable;
		case PNEUMATIC_VALVE_POSITION:
			readonly_var = *node.channels[pneumatic_valve->pos_channel_id].channel.adc16.analog_in;
			return &readonly_var;
		case PNEUMATIC_VALVE_TARGET_POSITION:
			return &pneumatic_valve->target_position;
		case PNEUMATIC_VALVE_THRESHOLD:
			return &pneumatic_valve->threshold;
		case PNEUMATIC_VALVE_HYSTERISIS:
			return &pneumatic_valve->hysteresis;
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
	Serial_PrintString("ResetSettings");
	return OOF_NOT_IMPLEMENTED;
}

Result_t PneumaticValve_Status(Channel_t *channel)
{
	Serial_PrintString("Status");
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
Result_t PneumaticValve_ProcessMessage(uint8_t ch_id, uint8_t cmd_id, uint8_t *data, uint32_t length)
{
	if (node.channels[ch_id].type != CHANNEL_TYPE_PNEUMATIC_VALVE)
		return OOF_WRONG_CHANNEL_TYPE;
	Channel_t *channel = &node.channels[ch_id];
	Serial_PrintString("PneumaticValve ProcessMessage");
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
		default:
			return OOF_UNKNOWN_CMD;
	}
}

static Result_t PneumaticValve_GetRawData(uint8_t channel_id, uint16_t *data)
{
	*data = (uint16_t) node.channels[channel_id].channel.pneumatic_valve.target_position;
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
	(*length) += PNEUMATIC_VALVE_DATA_N_BYTES;
	return NOICE;
}

Result_t PneumaticValve_Update(PneumaticValve_Channel_t *valve)
{
	if (valve->enable)
	{
		uint16_t position = 0;
		Result_t result = Adc16_GetRawData(valve->pos_channel_id, &position);
		if (result != NOICE)
			return result;

		int32_t error = valve->target_position - position;

		int32_t upper_threshold = (valve->threshold + DigitalOut_GetState(valve->off_channel_id) * valve->hysteresis);
		int32_t lower_threshold = -(valve->threshold + DigitalOut_GetState(valve->on_channel_id) * valve->hysteresis);

		DigitalOut_SetState(&node.channels[valve->off_channel_id].channel.digital_out, (error > upper_threshold));
		DigitalOut_SetState(&node.channels[valve->on_channel_id].channel.digital_out, (error < lower_threshold));
	}
	return NOICE;
}

