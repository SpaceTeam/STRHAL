#include <Channels/IMUChannel.h>

#include <cstdio>
#include <cstring>

IMUChannel::IMUChannel(uint8_t id, ICM20602_IMU *imu, IMUMeasurement measurementType, uint32_t refreshDivider) :
		AbstractChannel(CHANNEL_TYPE_ADC16, id, refreshDivider), imu(imu), measurementType(measurementType)
{
}

int IMUChannel::init()
{
	return 0;
}

int IMUChannel::exec()
{
	return 0;
}

int IMUChannel::reset()
{
	return imu->reset();
}

int IMUChannel::processMessage(uint8_t commandId, uint8_t *returnData, uint8_t &n)
{
	switch (commandId)
	{
		default:
			return AbstractChannel::processMessage(commandId, returnData, n);
	}
}

int IMUChannel::getSensorData(uint8_t *data, uint8_t &n)
{
	if (imu->measurementReady())
	{
		uint16_t *out = (uint16_t*) (data + n);
		uint16_t measurement = 0;
		imu->getMeasurement(measurement, measurementType);
		*out = measurement;
		n += ADC16_DATA_N_BYTES;
		return 0;
	}
	return -1;
}

int IMUChannel::setVariable(uint8_t variableId, int32_t data)
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

int IMUChannel::getVariable(uint8_t variableId, int32_t &data) const
{
	switch (variableId)
	{
		case ADC16_REFRESH_DIVIDER:
			data = (int32_t) refreshDivider;
			return 0;
		case ADC16_MEASUREMENT:
			//data = (int32_t) *adcMeasurement;
			return 0;
		default:
			return -1;
	}
}
