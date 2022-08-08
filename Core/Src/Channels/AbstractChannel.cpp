#include "../Inc/Channels/AbstractChannel.h"

#include <cstring>

AbstractChannel::AbstractChannel(CHANNEL_TYPE type, uint8_t id, uint32_t refreshDivider) :
		refreshDivider(refreshDivider), refreshCounter(0), channelType(type), channelId(id), channelStatus(CHANNEL_STATUS_NOICE)
{

}

CHANNEL_TYPE AbstractChannel::getChannelType() const
{
	return channelType;
}

CHANNEL_STATUS AbstractChannel::getChannelStatus() const
{
	return channelStatus;
}

uint8_t AbstractChannel::getChannelId() const
{
	return channelId;
}

bool AbstractChannel::IsChannelType(CHANNEL_TYPE type) const
{
	return type == channelType;
}

bool AbstractChannel::IsChannelId(uint8_t id) const
{
	return channelId == id;
}

bool AbstractChannel::IsRefreshed()
{
	if (refreshDivider == 0)
		return false;
	refreshCounter++;
	if (refreshCounter != refreshDivider)
		return false;

	refreshCounter = 0;
	return true;
}

int AbstractChannel::processMessage(uint8_t commandId, uint8_t *returnData, uint8_t &n)
{
	SetMsg_t *setMsg;
	setMsg = (SetMsg_t*) returnData;
	int32_t temp = 0;
	int status = 0;
	switch (commandId)
	{
		case COMMON_REQ_GET_VARIABLE:
			status = getVariable(setMsg->variable_id, temp);
			setMsg->value = temp;
			n += sizeof(SetMsg_t);
			return status;

		case COMMON_REQ_RESET_SETTINGS:
			return reset();

		case COMMON_REQ_SET_VARIABLE:
			if (setVariable(setMsg->variable_id, setMsg->value) == -1)
				return -1;
			status = getVariable(setMsg->variable_id, temp);
			setMsg->value = temp;
			n += sizeof(SetMsg_t);
			return status;

		case COMMON_REQ_STATUS:
			return getChannelStatus();

		default:
			return -1;
	}
}

AbstractChannel::~AbstractChannel()
{
}
