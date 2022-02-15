#include "../Inc/AbstractChannel.h"

#include <cstring>

AbstractChannel::AbstractChannel(CHANNEL_TYPE t, uint8_t channel_id)
	: ch_type(t), ch_id(channel_id), ch_status(CHANNEL_STATUS_NOICE){

}

CHANNEL_TYPE AbstractChannel::getChannelType() const {
	return ch_type;
}

CHANNEL_STATUS AbstractChannel::getChannelStatus() const {
	return ch_status;
}

bool AbstractChannel::IsChannelType(CHANNEL_TYPE t) const {
	return t == ch_type;
}

bool AbstractChannel::IsChannelId(uint8_t channel_id) const {
	return channel_id == ch_id;
}


int AbstractChannel::prcMsg(uint8_t cmd_id, uint8_t variable_id, uint32_t data, uint8_t *ret) {
	switch(cmd_id) {
		case COMMON_REQ_GET_VARIABLE:
			return getVar(variable_id, ret, 0xFF);
			break;
		case COMMON_REQ_RESET_SETTINGS:
			return reset();
			break;

		case COMMON_REQ_SET_VARIABLE:
			return setVar(variable_id, data);
			break;

		case COMMON_REQ_STATUS:
			return getChannelStatus();
			break;

		default:
			return -1;
	}
}

AbstractChannel::~AbstractChannel() {}
