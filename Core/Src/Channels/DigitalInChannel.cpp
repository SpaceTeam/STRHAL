#include <Channels/DigitalInChannel.h>

DigitalInChannel::DigitalInChannel(uint8_t id, const STRHAL_GPIO_t &digitalInPin, uint32_t refreshDivider) :
		AbstractChannel(CHANNEL_TYPE_ADC16, id, refreshDivider), digitalInPin(digitalInPin)
{
}

int DigitalInChannel::init()
{
	STRHAL_GPIO_SingleInit(&digitalInPin, STRHAL_GPIO_TYPE_IHZ);

	return 0;
}

int DigitalInChannel::exec()
{
	return 0;
}

int DigitalInChannel::reset()
{
	return 0;
}

int DigitalInChannel::processMessage(uint8_t commandId, uint8_t *returnData, uint8_t &n)
{
	switch (commandId)
	{
		default:
			return AbstractChannel::processMessage(commandId, returnData, n);
	}
}

int DigitalInChannel::getSensorData(uint8_t *data, uint8_t &n)
{
	uint16_t *out = (uint16_t*) (data + n);
	*out = (STRHAL_GPIO_Read(&digitalInPin) == STRHAL_GPIO_VALUE_L) ? 0UL : 1UL;

	n += ADC16_DATA_N_BYTES;
	return 0;
}

int DigitalInChannel::setVariable(uint8_t variableId, int32_t data)
{
	switch (variableId)
	{
		case ADC16_REFRESH_DIVIDER:
			refreshDivider = data;
			refreshCounter = 0;
			return 0;
		default:
			return -1;
	}
}

int DigitalInChannel::getVariable(uint8_t variableId, int32_t &data) const
{
	switch (variableId)
	{
		case ADC16_REFRESH_DIVIDER:
			data = (int32_t) refreshDivider;
			return 0;
		default:
			return -1;
	}
}

uint16_t DigitalInChannel::getMeasurement() const
{
	return (STRHAL_GPIO_Read(&digitalInPin) == STRHAL_GPIO_VALUE_L) ? 0UL : 1UL;;
}
