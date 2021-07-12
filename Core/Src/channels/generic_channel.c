#include "generic_channel.h"
#include "main.h"
#include "channel_util.h"
#include "channels.h"
#include "ui.h"
#include "serial.h"
#include "speaker.h"

static int32_t readonly_var = 0;
int32_t* Generic_VariableSelection(uint8_t var_id)
{
	readonly_var = 0;
	switch (var_id)
	{
		case GENERIC_BUS1_VOLTAGE:
			readonly_var = *node.generic_channel.bus1_voltage;
			return &readonly_var;
		case GENERIC_BUS2_VOLTAGE:
			readonly_var = *node.generic_channel.bus2_voltage;
			return &readonly_var;
		case GENERIC_PWR_VOLTAGE:
			readonly_var = *node.generic_channel.power_voltage;
			return &readonly_var;
		case GENERIC_PWR_CURRENT:
			readonly_var = *node.generic_channel.power_current;
			return &readonly_var;
		case GENERIC_REFRESH_DIVIDER:
			return &node.generic_channel.refresh_divider;
		case GENERIC_REFRESH_RATE:
			return &node.generic_channel.refresh_rate;
		default:
			return NULL;
	}
}

Result_t Generic_ResetAllSettings();
Result_t Generic_Status();

Result_t Generic_SetVariable(SetMsg_t *set_msg);
Result_t Generic_GetVariable(GetMsg_t *get_msg, GENERIC_CMDs response_cmd);
Result_t Generic_SyncClock();
Result_t Generic_NodeStatus();
Result_t Generic_Speaker();
Result_t Generic_EnableUartDebugging();

Result_t Generic_ResetAllSettings()
{

	return OOF_NOT_IMPLEMENTED;
}
Result_t Generic_Status()
{

	return OOF_NOT_IMPLEMENTED;
}

Result_t Generic_GenerateDataPayload(DataMsg_t *data, uint32_t *length)
{

	data->channel_mask = 0;
	for (uint32_t c = 0; c < MAX_CHANNELS; c++)
	{
		switch (node.channels[c].type)
		{
			case CHANNEL_TYPE_NODE_SPECIFIC:
				break;
			case CHANNEL_TYPE_ADC16:
				Adc16_GetData(c, data->uint8, length);
				break;
			case CHANNEL_TYPE_ADC16_SINGLE:
				//TODO @ANDI Add it and make it work
				break;
			case CHANNEL_TYPE_ADC24:
				Adc24_GetData(c, data->uint8, length);
				break;
			case CHANNEL_TYPE_COMPUTED32:
				break;
			case CHANNEL_TYPE_DIGITAL_OUT:
				DigitalOut_GetData(c, data->uint8, length);
				break;
			case CHANNEL_TYPE_SERVO:
				break;
			case CHANNEL_TYPE_NODE_GENERIC:
			case CHANNEL_TYPE_LAST:
			case CHANNEL_TYPE_UNKNOWN:
			default:
				break;
		}
	}
	return NOICE;
}

Result_t Generic_Data(void)
{

	Can_MessageId_t message_id =
	{ 0 };
	ChannelUtil_DefaultMessageId(&message_id);

	Can_MessageData_t data =
	{ 0 };

	data.bit.cmd_id = GENERIC_RES_DATA;
	data.bit.info.channel_id = GENERIC_CHANNEL_ID;
	data.bit.info.buffer = DIRECT_BUFFER;

	DataMsg_t *payload = (DataMsg_t*) &data.bit.data;
	uint32_t length = 0;
	Result_t result = Generic_GenerateDataPayload(payload, &length);
	if (result != NOICE)
		return result;
	return Ui_SendCanMessage(MAIN_CAN_BUS, message_id, &data, length);
}
Result_t Generic_SetVariable(SetMsg_t *set_msg)
{
	int32_t *var = Generic_VariableSelection(set_msg->variable_id);
	if (var == NULL)
		return OOF;
	*var = set_msg->value;
	return Generic_GetVariable((GetMsg_t*) set_msg, GENERIC_RES_SET_VARIABLE);
}
Result_t Generic_GetVariable(GetMsg_t *get_msg, GENERIC_CMDs response_cmd)
{
	Can_MessageId_t message_id =
	{ 0 };
	ChannelUtil_DefaultMessageId(&message_id);

	Can_MessageData_t data =
	{ 0 };

	data.bit.cmd_id = response_cmd;
	data.bit.info.channel_id = GENERIC_CHANNEL_ID;
	data.bit.info.buffer = DIRECT_BUFFER;

	SetMsg_t *set_msg = (SetMsg_t*) data.bit.data.uint8;
	int32_t *var = Generic_VariableSelection(get_msg->variable_id);
	if (var == NULL)
		return OOF;
	set_msg->variable_id = get_msg->variable_id;
	set_msg->value = *var;

	return Ui_SendCanMessage(MAIN_CAN_BUS, message_id, &data, sizeof(SetMsg_t));
}
Result_t Generic_SyncClock()
{
	return OOF_NOT_IMPLEMENTED;
}

Result_t Generic_NodeInfo(void)
{
	Can_MessageId_t message_id =
	{ 0 };
	ChannelUtil_DefaultMessageId(&message_id);
	Can_MessageData_t data =
	{ 0 };
	data.bit.info.channel_id = GENERIC_CHANNEL_ID;
	data.bit.info.buffer = DIRECT_BUFFER;
	data.bit.cmd_id = GENERIC_RES_NODE_INFO;

	NodeInfoMsg_t *info = (NodeInfoMsg_t*) &data.bit.data;

	info->firmware_version = node.firmware_version;

	info->channel_mask = 0x00000000;
	uint32_t length = 0;
	for (uint32_t c = 0; c < MAX_CHANNELS; c++)
	{
		if (node.channels[c].type != CHANNEL_TYPE_UNKNOWN)
		{
			info->channel_mask |= 1 << c;
			info->channel_type[length++] = node.channels[c].type;
		}
	}
	length += 2 * sizeof(uint32_t);
	return Ui_SendCanMessage(MAIN_CAN_BUS, message_id, &data, length);

}

Result_t Generic_ProcessMessage(uint8_t ch_id, uint8_t cmd_id, uint8_t *data, uint32_t length)
{
	if (ch_id != GENERIC_CHANNEL_ID)
		return OOF_WRONG_CHANNEL_TYPE;
	switch (cmd_id)
	{
		case GENERIC_REQ_RESET_ALL_SETTINGS:
			return Generic_ResetAllSettings();
		case GENERIC_REQ_DATA:
			return Generic_Data();
		case GENERIC_REQ_SET_VARIABLE:
			return Generic_SetVariable((SetMsg_t*) data);
		case GENERIC_REQ_GET_VARIABLE:
			return Generic_GetVariable((GetMsg_t*) data, GENERIC_RES_GET_VARIABLE);
		case GENERIC_REQ_SYNC_CLOCK:
			return Generic_SyncClock();
		case GENERIC_REQ_NODE_INFO:
			return Generic_NodeInfo();
		case GENERIC_REQ_NODE_STATUS:
			return Generic_NodeStatus();
		case GENERIC_REQ_SPEAKER:
			return Generic_Speaker((SpeakerMsg_t*) data);
		default:
			return OOF_UNKNOWN_CMD;

	}
}
Result_t Generic_NodeStatus()
{
	return OOF_NOT_IMPLEMENTED;
}
Result_t Generic_Speaker(SpeakerMsg_t *speaker_msg)
{
	Speaker_Set(speaker_msg->tone_frequency, speaker_msg->on_time, speaker_msg->off_time, speaker_msg->count);
	return NOICE;
}
Result_t Generic_EnableUartDebugging()
{
	return OOF_NOT_IMPLEMENTED;
}

