#include "../Inc/Channels/GenericChannel.h"

GenericChannel::GenericChannel(uint32_t node_id, uint32_t fw_version) : AbstractChannel(CHANNEL_TYPE_NODE_GENERIC, GENERIC_CHANNEL_ID), cancom(node_id, *this), node_id(node_id), fw_version(fw_version) {
}

int GenericChannel::init() {
	if(LID_Init(LID_SYSCLK_SRC_EXT, 8000000) != LID_NOICE)
		return -1;

	COMState CANState;

	CANState = cancom.init();
	if(CANState != COMState::SBY)
		return -1;

	for(AbstractChannel *channel : channels) {
		if(channel == nullptr)
			continue;
		if(channel->init() != 0)
			return -1;
	}

	return 0;
}

int GenericChannel::exec() {
	cancom.exec();
	for(AbstractChannel *channel : channels) {
		if(channel == nullptr)
			continue;
		if(channel->exec() != 0)
			return -1;
	}
	return 0;
}

int GenericChannel::reset() {
	return 0;
}

int GenericChannel::prcMsg(uint8_t cmd_id, uint8_t variable_id, uint32_t data, uint32_t &ret) {
	switch(cmd_id) {
		default:
			return AbstractChannel::prcMsg(cmd_id, variable_id, data, ret);
	}
}

int GenericChannel::prcMsg(uint8_t cmd_id, uint8_t variable_id, uint32_t data, uint32_t &ret, uint8_t ch_id) {
	for(AbstractChannel *channel : channels) {
		if(channel->IsChannelId(ch_id)) {
			if(channel->prcMsg(cmd_id, variable_id, data, ret) != 0)
				return -1;
			return 0;
		}

	}
	return -1;
}

int GenericChannel::setVar(uint8_t variable_id, uint32_t data) {
	switch(variable_id) {
		default:
			return -1;
	}
}

int GenericChannel::getVar(uint8_t variable_id, uint32_t &data) const {
	switch(variable_id) {
		default:
			return -1;
	}
}

int GenericChannel::getSensorData(uint8_t *data, uint8_t &n) {
	for(AbstractChannel *channel : channels) {
		if(channel == nullptr)
			continue;
		if(channel->getSensorData(data, n) == -1)
			return -1;
	}
	return 0;
}

int GenericChannel::addChannel(AbstractChannel &channel) {
	channels[n_sens++] = &channel;
	return 0;
}
