#include "adc16_channel.h"
#include "main.h"
#include "channel_util.h"
#include "channels.h"

Result_t Adc16_ProcessMessage(uint8_t ch_id, uint8_t cmd_id, uint8_t *data, uint32_t length)
{

	if (node.channels[ch_id].type != CHANNEL_TYPE_ADC16)
		return OOF_WRONG_CHANNEL_TYPE;
	Channel_t *channel = &node.channels[ch_id];

	switch (cmd_id)
	{
		case ADC16_RESET_SETTINGS:
			return OOF;
		case ADC16_SET_VARIABLE:
			return OOF;
		case ADC16_GET_VARIABLE:
			return OOF;
		case ADC16_CALIBRATE:
			return OOF;
		default:
			return OOF_UNKNOWN_CMD;
	}
}

Result_t Adc16_GetData(uint8_t ch_id, int32_t *data, uint32_t *length)
{
	return OOF;
}
