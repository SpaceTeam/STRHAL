#include <Channels/ADCChannel.h>
#include <cstring>
#include <cstdio>

ADCChannel::ADCChannel(uint8_t id, const STRHAL_ADC_Channel_t adcChanel, uint32_t refreshDivider) :
		AbstractChannel(CHANNEL_TYPE_ADC16, id, refreshDivider), adcChannel(adcChanel)
{
}

ADCChannel::ADCChannel(uint8_t id, STRHAL_ADC_Data_t *adcPtr, uint32_t refreshDivider) :
		AbstractChannel(CHANNEL_TYPE_ADC16, id, refreshDivider), adcMeasurement(adcPtr)
{
}

int ADCChannel::init()
{
	if (adcMeasurement == nullptr)
		adcMeasurement = STRHAL_ADC_SubscribeChannel(&adcChannel, STRHAL_ADC_INTYPE_REGULAR);

	if (adcMeasurement == nullptr)
		return -1;

	return 0;
}

int ADCChannel::exec()
{
	return 0;
}

int ADCChannel::reset()
{
	return 0;
}

int ADCChannel::processMessage(uint8_t commandId, uint8_t *returnData, uint8_t &n)
{
	switch (commandId)
	{
		default:
			return AbstractChannel::processMessage(commandId, returnData, n);
	}
}

int ADCChannel::getSensorData(uint8_t *data, uint8_t &n)
{
	uint16_t *out = (uint16_t*) (data + n);
	*out = *adcMeasurement << 4; // shift it to 16bit full scale

	/*char buf[32];
	 sprintf(buf,"%d\n",*adcMeasurement);
	 STRHAL_UART_Debug_Write_Blocking(buf, strlen(buf), 100);*/
	n += ADC16_DATA_N_BYTES;
	return 0;
}

int ADCChannel::setVariable(uint8_t variableId, int32_t data)
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

int ADCChannel::getVariable(uint8_t variableId, int32_t &data) const
{
	switch (variableId)
	{
		case ADC16_REFRESH_DIVIDER:
			data = (int32_t) refreshDivider;
			return 0;
		case ADC16_MEASUREMENT:
			data = (int32_t) *adcMeasurement;
			return 0;
		default:
			return -1;
	}
}

uint16_t ADCChannel::getMeasurement() const
{
	return *adcMeasurement;
}
