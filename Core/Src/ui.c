#include "ui.h"
#include "main.h"
#include "serial.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Result_t Ui_processCanMessage(uint32_t message_id, uint8_t *data, uint32_t length)
{
	Result_t result = OOF;
	uint8_t buffer = ((data[0] >> 6) & 0x3);
	uint8_t channel = (data[0] & 0x3F);
	Serial_PrintString("CAN NOICE\n");

	char send_buffer[1024] =
	{ 0 };
	sprintf(send_buffer + strlen(send_buffer), "message id :  %ld, 0x%03lx\n", message_id, message_id);

	for (uint32_t c = 0; c < length; c++)
		sprintf(send_buffer + strlen(send_buffer), "%ld :  0x%02x\n", c, data[c]);

	Serial_PrintString(send_buffer);

	if (buffer == 0)
	{
		uint8_t cmd_id = data[1];

		/*if (channel < CHANNEL_TYPE_LAST && channel_type_array[channel].cmds != 0)
		{
			const can_function *channel_cmds_array = channel_type_array[channel].cmds;
			const uint8_t last_index = channel_type_array[channel].last_index;
			if (cmd_id < last_index && channel_cmds_array[cmd_id] != NULL)
				result = channel_cmds_array[cmd_id]((uint32_t*) &data[2]);
		}*/
		//TODO else ERROR
	}
	else
	{
		uint32_t timestamp = (uint32_t) data[1];
		uint8_t cmd_id = data[5];

		timestamp = cmd_id;					//Just here to get rid of warnings
		cmd_id = timestamp;					//Just here to get rid of warnings
	}

return result;
}
