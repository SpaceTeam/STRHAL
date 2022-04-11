#include "../Inc/Channels/GenericChannel.h"
#include <cstring>
#include <cstdio>

GenericChannel::GenericChannel(uint32_t nodeId, uint32_t firmwareVersion, uint32_t refreshDivider)
	: AbstractChannel(CHANNEL_TYPE_NODE_GENERIC, GENERIC_CHANNEL_ID, refreshDivider), nodeId(nodeId), firmwareVersion(firmwareVersion) {
}

uint32_t GenericChannel::getNodeId() const {
	return nodeId;
}

int GenericChannel::init() {
	for(AbstractChannel *channel : channels) {
		if(channel == nullptr)
			continue;
		if(channel->init() != 0) {
			return -1;
		}
	}

	flash = W25Qxx_Flash::instance(0x1F);

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

int GenericChannel::processMessage(uint8_t commandId, uint8_t *returnData, uint8_t &n) {
	switch(commandId) {
		case GENERIC_REQ_NODE_INFO:
			return this->getNodeInfo(returnData, n);
		case GENERIC_REQ_DATA:
			return this->getSensorData(returnData, n);
		case GENERIC_REQ_RESET_ALL_SETTINGS:
			return ((flash->configReset()) ? 0 : -1);
		default:
			return AbstractChannel::processMessage(commandId, returnData, n);
	}
}

int GenericChannel::processMessage(uint8_t commandId, uint8_t *returnData, uint8_t &n, uint8_t channelId) {
	for(AbstractChannel *channel : channels) {
		if(channel->IsChannelId(channelId)) {
			if(channel->processMessage(commandId, returnData, n) != 0)
				return -1;
			return 0;
		}

	}
	return -1;
}

int GenericChannel::setVariable(uint8_t variableId, int32_t data) {
	switch(variableId) {
		case GENERIC_REFRESH_DIVIDER:
			refreshDivider = data;
			refreshCounter = 0;
			return 0;
		default:
			return -1;
	}
}

int GenericChannel::getVariable(uint8_t variableId, int32_t &data) const {
	switch(variableId) {
		case GENERIC_REFRESH_DIVIDER:
			data = (int32_t) refreshDivider;
			return 0;
		default:
			return -1;
	}
}

int GenericChannel::getSensorData(uint8_t *data, uint8_t &n) {
	if(!IsRefreshed())
		return -1;

	DataMsg_t *dataMsg = (DataMsg_t *) data;
	dataMsg->channel_mask = 0;
	for(AbstractChannel *channel : channels) {
		if(channel == nullptr || !channel->IsRefreshed())
			continue;
		if(channel->getSensorData(&dataMsg->uint8[0], n) == -1)
			return -1;
		dataMsg->channel_mask |= 1 << channel->getChanneSTRHAL();
	}
	n += 1 * sizeof(uint32_t);
	return 0;
}

int GenericChannel::getNodeInfo(uint8_t *data, uint8_t &n) {

	NodeInfoMsg_t *info = (NodeInfoMsg_t *) data;

	info->firmware_version = firmwareVersion;

	info->channel_mask = 0x00000000;
	uint32_t length = 0;
	uint8_t i = 0;
	for(AbstractChannel *channel : channels) {
		if(channel == nullptr)
			continue;

		info->channel_type[i] = channel->getChannelType();
		info->channel_mask |= 1 << channel->getChanneSTRHAL();
		length++;
		i++;
	}
	n = length + 2 * sizeof(uint32_t);
	return 0;
}

void GenericChannel::registerChannel(AbstractChannel *channel) {
	if(channel->getChanneSTRHAL() < MAX_CHANNELS)
		channels[channel->getChanneSTRHAL()] = channel;
}

void GenericChannel::registerChannels(AbstractChannel **channels, uint8_t n) {
	for(uint8_t i = 0; i < (n > MAX_CHANNELS ? MAX_CHANNELS : n); i++) {
		registerChannel(channels[i]);
	}
}
