#include <Channels/Data32Channel.h>
#include <cstring>
#include <cstdio>

Data32Channel::Data32Channel(uint8_t id, int32_t *dataPtr, uint32_t refreshDivider) :
		AbstractChannel(CHANNEL_TYPE_DATA32, id, refreshDivider), dataPtr(dataPtr)
{
}

int Data32Channel::init()
{
	if (dataPtr == nullptr)
		return -1;

	return 0;
}

int Data32Channel::exec()
{
	return 0;
}

int Data32Channel::reset()
{
	return 0;
}

int Data32Channel::processMessage(uint8_t commandId, uint8_t *returnData, uint8_t &n)
{
	switch (commandId)
	{
		default:
			return AbstractChannel::processMessage(commandId, returnData, n);
	}
}

int Data32Channel::getSensorData(uint8_t *data, uint8_t &n)
{
	int32_t *out = (int32_t*) (data + n);
	*out = *dataPtr;

	n += DATA32_DATA_N_BYTES;
	return 0;
}

int Data32Channel::setVariable(uint8_t variableId, int32_t data)
{
	switch (variableId)
	{
		case DATA32_REFRESH_DIVIDER:
			refreshDivider = data;
			refreshCounter = 0;
			return 0;
		default:
			return -1;
	}
}

int Data32Channel::getVariable(uint8_t variableId, int32_t &data) const
{
	switch (variableId)
	{
		case DATA32_REFRESH_DIVIDER:
			data = (int32_t) refreshDivider;
			return 0;
		default:
			return -1;
	}
}

int32_t Data32Channel::getMeasurement() const
{
	return *dataPtr;
}
