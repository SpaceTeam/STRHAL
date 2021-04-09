#include "generic_channel.h"
#include "main.h"
#include "channel_util.h"
#include "ui.h"

uint32_t node_id = 12;
Generic_Channel_t generic_channel =
{ 0 };

uint32_t* Generic_VariableSelection(uint8_t var_id)
{
	switch (var_id)
	{
	case GENERIC_BUS1_VOLTAGE:
		return &generic_channel.bus1_voltage;
	case GENERIC_BUS2_VOLTAGE:
		return &generic_channel.bus2_voltage;
	case GENERIC_PWR_VOLTAGE:
		return &generic_channel.power_voltage;
	case GENERIC_PWR_CURRENT:
		return &generic_channel.power_current;
	case GENERIC_REFRESH_DIVIDER:
		return &generic_channel.refresh_divider;
	case GENERIC_REFRESH_RATE:
		return &generic_channel.refresh_rate;
	default:
		return NULL;
	}
}


Result_t Generic_ResetAllSettings();
Result_t Generic_Status();
Result_t Generic_Data();
Result_t Generic_SetVariable(SetMsg_t *set_msg);
Result_t Generic_GetVariable(GetMsg_t *get_msg);
Result_t Generic_SyncClock();
Result_t Channels_SendNodeInfo(void);
Result_t Generic_NodeInfo();
Result_t Generic_NodeStatus();
Result_t Generic_Speaker();
Result_t Generic_EnableUartDebugging();

Result_t Generic_SetVariable(SetMsg_t *set_msg)
{
	uint32_t *var = Generic_VariableSelection(set_msg->variable_id);
	if (var == NULL)
		return OOF;
	*var = set_msg->value;
	return Generic_GetVariable((GetMsg_t*) set_msg);
}
Result_t Generic_GetVariable(GetMsg_t *get_msg)
{
	Can_MessageId_t message_id =
	{ 0 };
	ChannelUtil_DefaultMessageId(&message_id);

	Can_MessageData_t data =
	{ 0 };

	uint32_t length = 2;
	data.bit.cmd_id = GENERIC_SET_VARIABLE;
	data.bit.info.channel_id = GENERIC_NODE_CHANNEL_ID;
	data.bit.info.buffer = DIRECT_BUFFER;

	SetMsg_t *set_msg = (SetMsg_t *)&data.bit.data;
	uint32_t *var = Generic_VariableSelection(get_msg->variable_id);
	if (var == NULL)
		return OOF;
	set_msg->variable_id = get_msg->variable_id;
	set_msg->value = *var;
	length += 4;
	return Ui_SendCanMessage(0, message_id, &data, length);
}

Result_t Generic_NodeInfo(void)
{
	Can_MessageId_t message_id =
	{ 0 };
	ChannelUtil_DefaultMessageId(&message_id);
	Can_MessageData_t data =
	{ 0 };
	data.bit.info.channel_id = GENERIC_NODE_CHANNEL_ID;
	data.bit.info.buffer = DIRECT_BUFFER;
	data.bit.cmd_id = GENERIC_NODE_INFO;
	uint32_t length = 2;

	NodeInfoMsg_t *info = (NodeInfoMsg_t*) &data.bit.data;

	info->channel_mask = 0x00000001;			//TODO Get from node info
	length += 4;
	info->firmware_version = 0xDEADBEEF;
	length += 4;
	info->channel_type[0] = CHANNEL_TYPE_ADC24;
	length++;

	return Ui_SendCanMessage(0, message_id, &data, length);

}

Result_t Generic_ProcessMessage(uint8_t channel, uint8_t cmd_id, uint8_t *data, uint32_t length)
{

	switch (cmd_id)
	{
	case GENERIC_RESET_ALL_SETTINGS:
		return Generic_ResetAllSettings();
	case GENERIC_STATUS:
		return Generic_Status();
	case GENERIC_DATA:
		return Generic_Data();
	case GENERIC_SET_VARIABLE:
		return Generic_SetVariable((SetMsg_t*) data);
	case GENERIC_GET_VARIABLE:
		return Generic_GetVariable((GetMsg_t*) data);
	case GENERIC_SYNC_CLOCK:
		return Generic_SyncClock();
	case GENERIC_NODE_INFO:
		return Generic_NodeInfo();
	case GENERIC_NODE_STATUS:
		return Generic_NodeStatus();
	case GENERIC_SPEAKER:
		return Generic_Speaker();
	case GENERIC_ENABLE_UART_DEBUGGING:
		return Generic_EnableUartDebugging();
	default:
		return OOF_UNKNOWN_CMD;

	}
}

