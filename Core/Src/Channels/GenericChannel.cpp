#include "../Inc/Channels/GenericChannel.h"
#include <cstring>
#include <cstdio>

GenericChannel::GenericChannel(uint32_t node_id, uint32_t fw_version, uint32_t refresh_divider)
	: AbstractChannel(CHANNEL_TYPE_NODE_GENERIC, GENERIC_CHANNEL_ID), node_id(node_id), fw_version(fw_version), refresh_divider(refresh_divider), refresh_counter(0) {
}

uint32_t GenericChannel::getNodeId() const {
	return node_id;
}

int GenericChannel::init() {
	for(AbstractChannel *channel : channels) {
		if(channel == nullptr)
			continue;
		if(channel->init() != 0) {
			return -1;
		}
	}
	return 0;
}

int GenericChannel::exec() {
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

int GenericChannel::prcMsg(uint8_t cmd_id, uint8_t *ret_data, uint8_t &ret_n) {
	switch(cmd_id) {
		case GENERIC_REQ_NODE_INFO:
			return this->getNodeInfo(ret_data, ret_n);
		case GENERIC_REQ_DATA:
			return this->getSensorData(ret_data, ret_n);
		default:
			return AbstractChannel::prcMsg(cmd_id, ret_data, ret_n);
	}
}

int GenericChannel::prcMsg(uint8_t cmd_id, uint8_t *ret_data, uint8_t &ret_n, uint8_t ch_id) {
	for(AbstractChannel *channel : channels) {
		if(channel->IsChannelId(ch_id)) {
			if(channel->prcMsg(cmd_id, ret_data, ret_n) != 0)
				return -1;
			return 0;
		}

	}
	return -1;
}

int GenericChannel::setVar(uint8_t variable_id, int32_t data) {
	switch(variable_id) {
		case GENERIC_REFRESH_DIVIDER:
			refresh_divider = data;
			refresh_counter = 0;
			return 0;
		default:
			return -1;
	}
}

int GenericChannel::getVar(uint8_t variable_id, int32_t &data) const {
	switch(variable_id) {
		case GENERIC_REFRESH_DIVIDER:
			data = (int32_t) refresh_divider;
			return 0;
		default:
			return -1;
	}
}

int GenericChannel::getSensorData(uint8_t *data, uint8_t &n) {
	if(refresh_divider == 0)
		return -1;
	refresh_counter++;
	if(refresh_counter != refresh_divider)
		return -1;

	refresh_counter = 0;
	DataMsg_t *data_msg = (DataMsg_t *) data;
	data_msg->channel_mask = 0;
	for(AbstractChannel *channel : channels) {
		if(channel == nullptr)
			continue;
		if(channel->getSensorData(&data_msg->uint8[0], n) == -1)
			return -1;
		data_msg->channel_mask |= 1 << channel->getChannelId();
	}
	n += 1 * sizeof(uint32_t);
	return 0;
}

int GenericChannel::getNodeInfo(uint8_t *data, uint8_t &n) {

	NodeInfoMsg_t *info = (NodeInfoMsg_t *) data;

	info->firmware_version = fw_version;

	info->channel_mask = 0x00000000;
	uint32_t length = 0;
	for(AbstractChannel *channel : channels) {
		if(channel == nullptr)
			continue;

		info->channel_type[channel->getChannelId()] = channel->getChannelType();
		info->channel_mask |= 1 << channel->getChannelId();
		length++;
	}
	n = length + 2 * sizeof(uint32_t);
	return 0;
}

void GenericChannel::registerChannel(AbstractChannel *channel) {
	if(n_sens <= MAX_CHANNELS)
		channels[n_sens++] = channel;
}

void GenericChannel::registerChannels(AbstractChannel **channels, uint8_t n) {
	for(uint8_t i = 0; i < (n > MAX_CHANNELS ? MAX_CHANNELS : n); i++) {
		this->channels[i] = channels[i];
		n_sens++;
	}
}
