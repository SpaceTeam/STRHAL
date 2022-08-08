#include <Channels/PyroChannel.h>

PyroChannel::PyroChannel(uint8_t id, const STRHAL_ADC_Channel_t &adcChannel, const STRHAL_GPIO_t &cntrlPin, const DigitalInChannel &continuityChannel, uint32_t refreshDivider) :
		AbstractChannel(CHANNEL_TYPE_DIGITAL_OUT, id, refreshDivider), adcChannel(adcChannel), cntrlPin(cntrlPin), continuityChannel(continuityChannel)
{
}

int PyroChannel::init()
{
	adcMeasurement = STRHAL_ADC_SubscribeChannel(&adcChannel, STRHAL_ADC_INTYPE_REGULAR);
	STRHAL_GPIO_SingleInit(&cntrlPin, STRHAL_GPIO_TYPE_OPP);

	if (adcMeasurement == nullptr)
		return -1;

	return 0;
}

int PyroChannel::exec()
{
	return 0;
}

int PyroChannel::reset()
{
	return 0;
}

int PyroChannel::processMessage(uint8_t commandId, uint8_t *returnData, uint8_t &n)
{
	switch (commandId)
	{
		default:
			return AbstractChannel::processMessage(commandId, returnData, n);
	}
}

int PyroChannel::getSensorData(uint8_t *data, uint8_t &n)
{
	uint16_t *out = (uint16_t*) (data + n);
	*out = *adcMeasurement << 4; // shift it to 16bit full scale;

	n += DIGITAL_OUT_DATA_N_BYTES;
	return 0;
}

int PyroChannel::setVariable(uint8_t variableId, int32_t data)
{
	switch (variableId)
	{
		case DIGITAL_OUT_STATE:
			if (setState(data) != 0)
				return -1;
			return 0;
		case DIGITAL_OUT_DUTY_CYCLE:
			dutyCycle = data;
			return 0;
		case DIGITAL_OUT_FREQUENCY:
			frequency = data;
			return 0;
		case DIGITAL_OUT_SENSOR_REFRESH_DIVIDER:
			refreshDivider = data;
			refreshCounter = 0;
			return 0;
		default:
			return -1;
	}
}

int PyroChannel::getVariable(uint8_t variableId, int32_t &data) const
{
	switch (variableId)
	{
		case DIGITAL_OUT_STATE:
			data = getState();
			return 0;
		case DIGITAL_OUT_DUTY_CYCLE:
			data = dutyCycle;
			return 0;
		case DIGITAL_OUT_FREQUENCY:
			data = frequency;
			return 0;
		case DIGITAL_OUT_SENSOR_REFRESH_DIVIDER:
			data = (int32_t) refreshDivider;
			return 0;
		default:
			return -1;
	}
}

uint32_t PyroChannel::getState() const
{
	return (STRHAL_GPIO_ReadOutput(&cntrlPin) == STRHAL_GPIO_VALUE_L) ? 0UL : 1UL;
}

int PyroChannel::setState(uint32_t state)
{
	if (state == 0)
	{
		STRHAL_GPIO_Write(&cntrlPin, STRHAL_GPIO_VALUE_L);
	}
	else
	{
		STRHAL_GPIO_Write(&cntrlPin, STRHAL_GPIO_VALUE_H);
	}
	return 0;
}

uint16_t PyroChannel::getMeasurement() const
{
	return *adcMeasurement;
}

uint16_t PyroChannel::getContinuity() const
{
	return continuityChannel.getMeasurement();
}
