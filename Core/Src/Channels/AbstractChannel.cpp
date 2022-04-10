#include "../Inc/Channels/AbstractChannel.h"

#include <cstring>

AbstractChannel::AbstractChannel(CHANNEL_TYPE t, uint8_t channel_id, uint32_t refresh_divider)
	: refresh_divider(refresh_divider), refresh_counter(0), ch_type(t), ch_id(channel_id), ch_status(CHANNEL_STATUS_NOICE) {

}

CHANNEL_TYPE AbstractChannel::getChannelType() const {
	return ch_type;
}

CHANNEL_STATUS AbstractChannel::getChannelStatus() const {
	return ch_status;
}

uint8_t AbstractChannel::getChanneSTRHAL() const {
	return ch_id;
}

bool AbstractChannel::IsChannelType(CHANNEL_TYPE t) const {
	return t == ch_type;
}

bool AbstractChannel::IsChanneSTRHAL(uint8_t channel_id) const {
	return channel_id == ch_id;
}

bool AbstractChannel::IsRefreshed() {
	if(refresh_divider == 0)
		return false;
	refresh_counter++;
	if(refresh_counter != refresh_divider)
		return false;

	refresh_counter = 0;
	return true;
}

int AbstractChannel::prcMsg(uint8_t cmd_id, uint8_t *ret_data, uint8_t &ret_n) {
	SetMsg_t * set_msg =
	{ 0 };
	set_msg = (SetMsg_t *) ret_data;
	int32_t temp = 0;
	int ret = 0;
	switch(cmd_id) {
		case COMMON_REQ_GET_VARIABLE:
			ret = getVar(set_msg->variable_id, temp);
			set_msg->value = temp;
			ret_n+=sizeof(SetMsg_t);
			return ret;

		case COMMON_REQ_RESET_SETTINGS:
			return reset();

		case COMMON_REQ_SET_VARIABLE:
			if(setVar(set_msg->variable_id, set_msg->value) == -1)
				return -1;
			ret = getVar(set_msg->variable_id, temp);
			set_msg->value = temp;
			ret_n+=sizeof(SetMsg_t);
			return ret;

		case COMMON_REQ_STATUS:
			return getChannelStatus();

		default:
			return -1;
	}
}

AbstractChannel::~AbstractChannel() {}
