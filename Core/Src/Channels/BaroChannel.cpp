#include "../Inc/Channels/BaroChannel.h"

#include <cstring>
#include <cstdio>

BaroChannel::BaroChannel(uint8_t id, LPS25HB_Baro *baro, uint32_t refreshDivider) :
		AbstractChannel(CHANNEL_TYPE_ADC24, id, refreshDivider), baro(baro)
{
}

int BaroChannel::init()
{
	return 0;
}

int BaroChannel::exec()
{
	return 0;
}

int BaroChannel::reset()
{
	return baro->reset();
}

int BaroChannel::getSensorData(uint8_t *data, uint8_t &n)
{
	//if (baro.measurementReady())
	//{
		uint8_t *out = data + n;
		int32_t measurement = baro->measurementData;
		//baro.getMeasurement(measurement);
		out[0] = (uint8_t) (measurement >> 0) & 0xFF;
		out[1] = (uint8_t) (measurement >> 8) & 0xFF;
		out[2] = (uint8_t) (measurement >> 16) & 0xFF;

		n += ADC24_DATA_N_BYTES;
		return 0;
	//}

	//return -1;
}

int BaroChannel::processMessage(uint8_t commandId, uint8_t *returnData, uint8_t &n)
{
	return AbstractChannel::processMessage(commandId, returnData, n);
}

int BaroChannel::setVariable(uint8_t variableId, int32_t data)
{
	switch (variableId)
	{
		case ADC24_REFRESH_DIVIDER:
			refreshDivider = data;
			refreshCounter = 0;
			return 0;
		default:
			return -1;
	}
}

int BaroChannel::getVariable(uint8_t variableId, int32_t &data) const
{
	switch (variableId)
	{
		case ADC24_REFRESH_DIVIDER:
			data = (int32_t) refreshDivider;
			return 0;
		default:
			return -1;
	}
}

BaroChannel::~BaroChannel()
{

}
