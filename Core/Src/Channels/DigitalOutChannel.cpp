#include <Channels/DigitalOutChannel.h>

DigitalOutChannel::DigitalOutChannel(uint8_t id, const STRHAL_ADC_Channel_t &adcChannel, const STRHAL_GPIO_t &cntrlPin, STRHAL_ADC_InType_t adcInType, uint32_t refreshDivider) :
		AbstractChannel(CHANNEL_TYPE_DIGITAL_OUT, id, refreshDivider), adcChannel(adcChannel), cntrlPin(cntrlPin), adcInType(adcInType), hasFeedback(true)
{
}

DigitalOutChannel::DigitalOutChannel(uint8_t id, const STRHAL_GPIO_t &cntrlPin, uint32_t refreshDivider) :
		AbstractChannel(CHANNEL_TYPE_DIGITAL_OUT, id, refreshDivider), cntrlPin(cntrlPin)
{
}

int DigitalOutChannel::init()
{
	if (hasFeedback)
	{
		adcMeasurement = STRHAL_ADC_SubscribeChannel(&adcChannel, adcInType);
		STRHAL_GPIO_SingleInit(&cntrlPin, STRHAL_GPIO_TYPE_OPP);

		if (adcMeasurement == nullptr)
			return -1;
	}

	return 0;
}

int DigitalOutChannel::exec()
{
	return 0;
}

int DigitalOutChannel::reset()
{
	return 0;
}

int DigitalOutChannel::processMessage(uint8_t commandId, uint8_t *returnData, uint8_t &n)
{
	switch (commandId)
	{
		default:
			return AbstractChannel::processMessage(commandId, returnData, n);
	}
}

int DigitalOutChannel::getSensorData(uint8_t *data, uint8_t &n)
{
	uint16_t *out = (uint16_t*) (data + n);
	*out = (hasFeedback) ? (*adcMeasurement << 4) : 0; // shift to 16bit full scale, if no feedback is present return 0

	n += DIGITAL_OUT_DATA_N_BYTES;
	return 0;
}

int DigitalOutChannel::setVariable(uint8_t variableId, int32_t data)
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

int DigitalOutChannel::getVariable(uint8_t variableId, int32_t &data) const
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

uint32_t DigitalOutChannel::getState() const
{
	return (STRHAL_GPIO_ReadOutput(&cntrlPin) == STRHAL_GPIO_VALUE_L) ? 0UL : 1UL;
}

int DigitalOutChannel::setState(uint32_t state)
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

uint16_t DigitalOutChannel::getMeas() const
{
	return *adcMeasurement;
}
