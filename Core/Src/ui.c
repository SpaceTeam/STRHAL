#include "ui.h"
#include "main.h"
#include "can.h"
#include "serial.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

inline Result_t Ui_SendCanMessage(uint32_t can_index, Can_MessageId_t message_id, Can_MessageData_t *data, uint32_t length)
{
	return Can_sendMessage(can_index, message_id.uint32, data->uint8, length);
}

Result_t Ui_ProcessCanMessage(Can_MessageId_t message_id, Can_MessageData_t *data, uint32_t length)
{
	Result_t result = OOF;
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

	switch (0)
	//TODO GET CHANNEL TYPE
	{
	case CHANNEL_TYPE_NODE_GENERIC:
		break;
	case CHANNEL_TYPE_NODE_SPECIFIC:
		break;
	case CHANNEL_TYPE_ADC16:
		break;
	case CHANNEL_TYPE_ADC24:
		break;
	case CHANNEL_TYPE_COMPUTED32:
		break;
	case CHANNEL_TYPE_DIGITAL_OUT:
		break;
	case CHANNEL_TYPE_SERVO:
		break;
	case CHANNEL_TYPE_UNKNOWN:
	default:

		break;

	}

	/*if (channel < CHANNEL_TYPE_LAST && channel_type_array[channel].cmds != 0)
	 {
	 const can_function *channel_cmds_array = channel_type_array[channel].cmds;
	 const uint8_t last_index = channel_type_array[channel].last_index;
	 if (cmd_id < last_index && channel_cmds_array[cmd_id] != NULL)
	 result = channel_cmds_array[cmd_id]((uint32_t*) &data[2]);
	 }*/
	//TODO else ERROR
	return result;
}
