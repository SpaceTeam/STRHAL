#include "../Inc/Channels/GenericChannel.h"
#include <cstring>
#include <cstdio>
#include <git_version.h>

GenericChannel::GenericChannel(uint32_t nodeId, uint32_t firmwareVersion, uint32_t refreshDivider)
	: AbstractChannel(CHANNEL_TYPE_NODE_GENERIC, GENERIC_CHANNEL_ID, refreshDivider), nodeId(nodeId), firmwareVersion(GIT_COMMIT_HASH_VALUE) {
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
	(void) flash->reset();
	return 0;
}

int GenericChannel::processMessage(uint8_t commandId, uint8_t *returnData, uint8_t &n) {
	switch(commandId) {
		case GENERIC_REQ_NODE_INFO:
			return this->getNodeInfo(returnData, n);
		case GENERIC_REQ_DATA:
			return this->getSensorData(returnData, n);
		case GENERIC_REQ_RESET_ALL_SETTINGS:
			(void) flash->configReset();
			for(AbstractChannel *channel : channels) {
				if(channel == nullptr)
					continue;

				channel->reset(); // TODO implement good reset for every channel
			}
			return 0;
		case GENERIC_REQ_FLASH_STATUS:
			(void) flash->setState(FlashState::CLEARING);
			return this->getFlashClearInfo(returnData, n);
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
		case GENERIC_LOGGING_ENABLED:
			loggingEnabled = data;
			if(loggingEnabled == 0) {
				flash->setState(FlashState::IDLE);
			} else {
				flash->setState(FlashState::LOGGING);
			}
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
		case GENERIC_LOGGING_ENABLED:
			data = (int32_t) loggingEnabled;
			return 0;
		default:
			return -1;
	}
}

int GenericChannel::flashClear(uint8_t *data, uint8_t &n) {
	FlashStatusMsg_t *dataMsg = (FlashStatusMsg_t *) data;

	dataMsg->status = INITIATED;
	n = sizeof(FlashStatusMsg_t);
	return 0;
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
			continue;
		dataMsg->channel_mask |= 1 << channel->getChannelId();
	}
	n += 1 * sizeof(uint32_t);

	if(loggingEnabled && !flash->lock)
		flash->addLog(data, n);
	return 0;
}

int GenericChannel::getFlashClearInfo(uint8_t *data, uint8_t &n) {

	FlashStatusMsg_t *info = (FlashStatusMsg_t *) data;

	FlashState flashState = flash->getState();
	if(flashState == FlashState::IDLE || flashState == FlashState::CLEARING) { //TODO actually check if clearing has initiated
		info->status = INITIATED;
	} else if(flashState == FlashState::READY) {
		info->status = COMPLETED;
	} else {
		info->status = INITIATED;
	}

	n = sizeof(FlashStatusMsg_t);
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
		info->channel_mask |= 1 << channel->getChannelId();
		length++;
		i++;
	}
	n = length + 2 * sizeof(uint32_t);
	return 0;
}

void GenericChannel::registerChannel(AbstractChannel *channel) {
	if(channel->getChannelId() < MAX_CHANNELS)
		channels[channel->getChannelId()] = channel;
}

void GenericChannel::registerChannels(AbstractChannel **channels, uint8_t n) {
	for(uint8_t i = 0; i < (n > MAX_CHANNELS ? MAX_CHANNELS : n); i++) {
		registerChannel(channels[i]);
	}
}

void GenericChannel::printLog() {
	char kartoffel_buffer[64];
	uint8_t readData[256];
	for(int i = LOGGING_BASE >> 8; i < 8192*16; i++) {
		if(flash->read(i << 8, readData, 252) != 252) {
			sprintf(kartoffel_buffer, "UNABLE TO READ PAGE!\n");
			STRHAL_UART_Debug_Write_Blocking(kartoffel_buffer, strlen(kartoffel_buffer), 100);
			i--;
			continue;
		}
		STRHAL_UART_Debug_Write_Blocking((char *) readData, 256, 100);
		LL_mDelay(2);
	}
}


void GenericChannel::detectReadoutMode() {
	STRHAL_UART_Listen(STRHAL_UART_DEBUG);
	char read[16];
	int32_t n = STRHAL_UART_Read(STRHAL_UART_DEBUG, read, 2);
	if(n > 0) {
		printLog();
	}
}
