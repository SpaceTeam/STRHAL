#include "ui.h"
#include "main.h"
#include "channels.h"
#include "can.h"
#include "serial.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

inline Result_t Ui_SendCanMessage(uint32_t can_index, Can_MessageId_t message_id, Can_MessageData_t *data, uint32_t length)
{
	return Can_sendMessage(can_index, message_id.uint32, data->uint8, CAN_MSG_LENGTH(length));
}

Result_t Ui_ProcessCanMessage(Can_MessageId_t message_id, Can_MessageData_t *data, uint32_t length)
{
	uint8_t buffer = data->bit.info.buffer;
	uint8_t channel = data->bit.info.channel_id;
	uint8_t cmd_id = data->bit.cmd_id;

	Serial_PrintString("CAN NOICE\n");
	Serial_PutString("buffer: ");
	Serial_PrintInt(buffer);
	Serial_PutString("channel: ");
	Serial_PrintInt(channel);
	Serial_PutString("cmd_id: ");
	Serial_PrintInt(cmd_id);

	if (channel == GENERIC_CHANNEL_ID)
		return Generic_ProcessMessage(channel, cmd_id, data->bit.data.uint8, length);
	if (channel >= MAX_CHANNELS)
		return OOF;

	switch (node.channels[channel].type)
	//TODO GET CHANNEL TYPE
	{
		case CHANNEL_TYPE_NODE_SPECIFIC:
			break;
		case CHANNEL_TYPE_ADC16:
			break;
		case CHANNEL_TYPE_ADC24:
			return Adc24_ProcessMessage(channel, cmd_id, data->bit.data.uint8, length);
		case CHANNEL_TYPE_COMPUTED32:
			break;
		case CHANNEL_TYPE_DIGITAL_OUT:
			break;
		case CHANNEL_TYPE_SERVO:
			break;
		case CHANNEL_TYPE_NODE_GENERIC:
		case CHANNEL_TYPE_LAST:
		case CHANNEL_TYPE_UNKNOWN:
		default:
			return OOF_UNKNOWN_CMD;
	}
	return OOF;
}
