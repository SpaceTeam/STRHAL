#include "../Inc/Channels/GenericChannel.h"
#include <cstring>
#include <cstdio>
#include <git_version.h>

GenericChannel* GenericChannel::gcPtr = nullptr; // necessary for static callbacks
bool GenericChannel::loraActive = false;

GenericChannel::GenericChannel(uint32_t nodeId, uint32_t firmwareVersion, uint32_t refreshDivider) :
		AbstractChannel(CHANNEL_TYPE_NODE_GENERIC, GENERIC_CHANNEL_ID, refreshDivider), nodeId(nodeId), firmwareVersion(GIT_COMMIT_HASH_VALUE), flash(W25Qxx_Flash::instance())
{
	gcPtr = this;
}

uint32_t GenericChannel::getNodeId() const
{
	return nodeId;
}

int GenericChannel::init()
{
	for (AbstractModule *module : modules)
	{
		if (module == nullptr)
			continue;
		if (module->init() != 0)
			return -1;
	}

	for (AbstractChannel *channel : channels)
	{
		if (channel == nullptr)
			continue;
		if (channel->init() != 0)
		{
			return -1;
		}
	}

	return 0;
}

int GenericChannel::exec()
{
	for (AbstractModule *module : modules)
	{
		if (module == nullptr)
			continue;
		if (module->exec() != 0)
			return -1;
	}

	for (AbstractChannel *channel : channels)
	{
		if (channel == nullptr)
			continue;
		if (channel->exec() != 0)
			return -1;
	}
	return 0;
}

int GenericChannel::reset()
{
	(void) flash.reset();
	return 0;
}

int GenericChannel::processMessage(uint8_t commandId, uint8_t *returnData, uint8_t &n)
{
	switch (commandId)
	{
		case GENERIC_REQ_NODE_INFO:
			return this->getNodeInfo(returnData, n);
		case GENERIC_REQ_DATA:
			return this->getSensorData(returnData, n);
		case GENERIC_REQ_RESET_ALL_SETTINGS:
			(void) flash.configReset();
			for (AbstractChannel *channel : channels)
			{
				if (channel == nullptr)
					continue;

				channel->reset(); // TODO implement good reset for every channel
			}
			return 0;
		case GENERIC_REQ_FLASH_STATUS:
			(void) flash.setState(FlashState::CLEARING);
			return this->getFlashClearInfo(returnData, n);
		default:
			return AbstractChannel::processMessage(commandId, returnData, n);
	}
}

int GenericChannel::processMessage(uint8_t commandId, uint8_t *returnData, uint8_t &n, uint8_t channelId)
{
	for (AbstractChannel *channel : channels)
	{
		if (channel->IsChannelId(channelId))
		{
			if (channel->processMessage(commandId, returnData, n) != 0)
				return -1;
			return 0;
		}

	}
	return -1;
}

int GenericChannel::setVariable(uint8_t variableId, int32_t data)
{
	switch (variableId)
	{
		case GENERIC_REFRESH_DIVIDER:
			refreshDivider = data;
			refreshCounter = 0;
			return 0;
		case GENERIC_LOGGING_ENABLED:
			loggingEnabled = data;
			if (loggingEnabled == 0)
			{
				flash.setState(FlashState::IDLE);
			}
			else
			{
				flash.setState(FlashState::LOGGING);
			}
			return 0;
		default:
			return -1;
	}
}

int GenericChannel::getVariable(uint8_t variableId, int32_t &data) const
{
	switch (variableId)
	{
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

int GenericChannel::flashClear(uint8_t *data, uint8_t &n)
{
	FlashStatusMsg_t *dataMsg = (FlashStatusMsg_t*) data;

	dataMsg->status = INITIATED;
	n = sizeof(FlashStatusMsg_t);
	return 0;
}

int GenericChannel::getSensorData(uint8_t *data, uint8_t &n)
{
	if (!IsRefreshed())
		return -1;

	DataMsg_t *dataMsg = (DataMsg_t*) data;
	dataMsg->channel_mask = 0;
	for (AbstractChannel *channel : channels)
	{
		if (channel == nullptr || !channel->IsRefreshed())
			continue;
		if (channel->getSensorData(&dataMsg->uint8[0], n) == -1)
			continue;
		dataMsg->channel_mask |= 1 << channel->getChannelId();
	}
	n += 1 * sizeof(uint32_t);

	if (loggingEnabled && !flash.lock)
		flash.addLog(data, n);
	return 0;
}

int GenericChannel::getFlashClearInfo(uint8_t *data, uint8_t &n)
{

	FlashStatusMsg_t *info = (FlashStatusMsg_t*) data;

	FlashState flashState = flash.getState();
	if (flashState == FlashState::IDLE || flashState == FlashState::CLEARING)
	{ //TODO actually check if clearing has initiated
		info->status = INITIATED;
	}
	else if (flashState == FlashState::READY)
	{
		info->status = COMPLETED;
	}
	else
	{
		info->status = INITIATED;
	}

	n = sizeof(FlashStatusMsg_t);
	return 0;
}

int GenericChannel::getNodeInfo(uint8_t *data, uint8_t &n)
{

	NodeInfoMsg_t *info = (NodeInfoMsg_t*) data;

	info->firmware_version = firmwareVersion;

	info->channel_mask = 0x00000000;
	uint32_t length = 0;
	uint8_t i = 0;
	for (AbstractChannel *channel : channels)
	{
		if (channel == nullptr)
			continue;

		info->channel_type[i] = channel->getChannelType();
		info->channel_mask |= 1 << channel->getChannelId();
		length++;
		i++;
	}
	n = length + 2 * sizeof(uint32_t);
	return 0;
}

void GenericChannel::registerChannel(AbstractChannel *channel)
{
	if (channel->getChannelId() < MAX_CHANNELS)
		channels[channel->getChannelId()] = channel;
}

void GenericChannel::registerChannels(AbstractChannel **channels, uint8_t n)
{
	for (uint8_t i = 0; i < (n > MAX_CHANNELS ? MAX_CHANNELS : n); i++)
	{
		registerChannel(channels[i]);
	}
}

void GenericChannel::registerModule(AbstractModule *module)
{
	if (moduleIndex < MAX_MODULES)
	{
		modules[moduleIndex] = module;
		moduleIndex++;
	}
}

void GenericChannel::printLog()
{
	char kartoffel_buffer[64];
	uint8_t readData[256];
	for (int i = LOGGING_BASE >> 8; i < 8192 * 16; i++)
	{
		if (flash.read(i << 8, readData, 252) != 252)
		{
			sprintf(kartoffel_buffer, "UNABLE TO READ PAGE!\n");
			STRHAL_UART_Debug_Write_Blocking(kartoffel_buffer, strlen(kartoffel_buffer), 100);
			i--;
			continue;
		}
		STRHAL_UART_Debug_Write_Blocking((char*) readData, 256, 100);
		LL_mDelay(2);
	}
}

void GenericChannel::detectReadoutMode()
{
	STRHAL_UART_Listen(STRHAL_UART_DEBUG);
	char read[16];
	int32_t n = STRHAL_UART_Read(STRHAL_UART_DEBUG, read, 2);
	if (n > 0)
	{
		printLog();
	}
}

void GenericChannel::setLoraActive(bool enable) {
	loraActive = enable;
}

void GenericChannel::receptorLora(uint32_t id, uint8_t *data, uint32_t n)
{
	Can_MessageId_t msgId =
	{ 0 };
	Can_MessageData_t msgData =
	{ 0 };

	msgId.uint32 = id;
	memcpy(msgData.uint8, data, 64); //TODO only copy n bytes
	uint8_t commandId = msgData.bit.cmd_id;
	uint8_t channelId = msgData.bit.info.channel_id;
	uint8_t ret_n = 0;

	if (channelId == 6)
	{ // ECU
		if (loraActive)
		{
			Radio::msgArray[Radio::ECU_START_ADDR] = 1;
			memcpy(&Radio::msgArray[Radio::ECU_START_ADDR + 1], msgData.bit.data.uint8, Radio::ECU_MSG_SIZE - 1);
		}
		return;
	}
	else if (channelId == 7)
	{ // PMU
		if (loraActive)
		{
			Radio::msgArray[Radio::ECU_START_ADDR] = 1;
			memcpy(&Radio::msgArray[Radio::PMU_START_ADDR + 1], msgData.bit.data.uint8, Radio::PMU_MSG_SIZE - 1);
		}
		return;
	}

	if (channelId == GENERIC_CHANNEL_ID)
	{
		if (gcPtr->processMessage(commandId, msgData.bit.data.uint8, ret_n) != 0)
			return;
	}
	else
	{
		if (gcPtr->processMessage(commandId, msgData.bit.data.uint8, ret_n, channelId) != 0)
			return;
	}

	msgId.info.direction = NODE2MASTER_DIRECTION;
	msgId.info.node_id = gcPtr->getNodeId();
	msgId.info.special_cmd = STANDARD_SPECIAL_CMD;
	msgId.info.priority = STANDARD_PRIORITY;
	msgData.bit.cmd_id = commandId + 1;

	(void) STRHAL_CAN_Send(STRHAL_FDCAN1, msgId.uint32, msgData.uint8, CAN_MSG_LENGTH(ret_n));
}

void GenericChannel::receptor(uint32_t id, uint8_t *data, uint32_t n)
{
	Can_MessageId_t msgId =
	{ 0 };
	Can_MessageData_t msgData =
	{ 0 };

	msgId.uint32 = id;
	memcpy(msgData.uint8, data, 64); //TODO only copy n bytes
	uint8_t commandId = msgData.bit.cmd_id;
	uint8_t channelId = msgData.bit.info.channel_id;
	uint8_t ret_n = 0;

	if (channelId == GENERIC_CHANNEL_ID)
	{
		if (gcPtr->processMessage(commandId, msgData.bit.data.uint8, ret_n) != 0)
			return;
	}
	else
	{
		if (gcPtr->processMessage(commandId, msgData.bit.data.uint8, ret_n, channelId) != 0)
			return;
	}

	msgId.info.direction = NODE2MASTER_DIRECTION;
	msgId.info.node_id = gcPtr->getNodeId();
	msgId.info.special_cmd = STANDARD_SPECIAL_CMD;
	msgId.info.priority = STANDARD_PRIORITY;
	msgData.bit.cmd_id = commandId + 1;

#ifdef UART_DEBUG
	uint8_t msgBuf[66] =
	{ 0 };
	msgBuf[0] = 0x3A;
	msgBuf[1] = gcPtr->getNodeId();
	memcpy(&msgBuf[2], msgData.uint8, CAN_MSG_LENGTH(ret_n));
	msgBuf[CAN_MSG_LENGTH(ret_n) + 2] = 0x0A;
	STRHAL_UART_Debug_Write_DMA((char *) msgBuf, CAN_MSG_LENGTH(ret_n) + 3);
#endif

	(void) STRHAL_CAN_Send(STRHAL_FDCAN1, msgId.uint32, msgData.uint8, CAN_MSG_LENGTH(ret_n));
}

void GenericChannel::heartbeatCan()
{
	Can_MessageId_t msgId =
	{ 0 };
	msgId.info.special_cmd = STANDARD_SPECIAL_CMD;
	msgId.info.direction = NODE2MASTER_DIRECTION;
	msgId.info.node_id = gcPtr->getNodeId();
	msgId.info.priority = STANDARD_PRIORITY;

	Can_MessageData_t msgData =
	{ 0 };
	msgData.bit.cmd_id = GENERIC_RES_DATA;
	msgData.bit.info.channel_id = GENERIC_CHANNEL_ID;
	msgData.bit.info.buffer = DIRECT_BUFFER;

	uint8_t n = 0;
	if (gcPtr->getSensorData(&msgData.bit.data.uint8[0], n) != 0)
	{ // Sensor Data collection failed, or Refresh Divider not yet met
		return;
	}

	if (loraActive && n >= 23)
	{
		Radio::msgArray[Radio::RCU_START_ADDR] = 1;
		memcpy(&Radio::msgArray[Radio::RCU_START_ADDR + 1], msgData.bit.data.uint8, n - 4);
	}

#ifdef UART_DEBUG
	uint8_t msgBuf[66] =
	{ 0 };
	msgBuf[0] = 0x3A;
	msgBuf[1] = gcPtr->getNodeId();
	memcpy(&msgBuf[2], msgData.uint8, CAN_MSG_LENGTH(n));
	msgBuf[CAN_MSG_LENGTH(n) + 2] = 0x0A;
	STRHAL_UART_Debug_Write_DMA((char *) msgBuf, CAN_MSG_LENGTH(n) + 3);
#endif

	(void) STRHAL_CAN_Send(STRHAL_FDCAN1, msgId.uint32, msgData.uint8, n);
}

void GenericChannel::heartbeatLora()
{
	//sprintf((char *)radioArray,"hello world");
	//radio->send(0, radioArray, MSG_SIZE);
	//char *test = "hello world";
	//radio->send(0, (uint8_t *) test, MSG_SIZE);
}
