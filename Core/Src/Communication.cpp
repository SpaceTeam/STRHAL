#include "../Inc/Channels/AbstractChannel.h"
#include "../Inc/Channels/GenericChannel.h"
#include "STRHAL.h"
#include <can_houbolt/can_cmds.h>
#include <Communication.h>
#include <cstring>
#include <cstdio>

Communication *Communication::com = nullptr;
GenericChannel *Communication::genericChannel = nullptr;
LoRa1276F30_Radio *Communication::lora = nullptr;
Can *Communication::can = nullptr;
Radio *Communication::radio = nullptr;
uint8_t Communication::radioArray[MSG_SIZE] = { 0 };
bool Communication::loraActive = false;

Communication::Communication(GenericChannel *genericChannel, LoRa1276F30_Radio *lora) {
	Communication::genericChannel = genericChannel;
	Communication::lora = lora;
	can = Can::instance(genericChannel->getNodeId());
	if(lora != nullptr)
		radio = Radio::instance(genericChannel->getNodeId(), lora);
}

Communication *Communication::instance(GenericChannel *genericChannel, LoRa1276F30_Radio *lora) {
	if(Communication::com == nullptr && genericChannel != nullptr) {
		if(lora != nullptr)
			Communication::com = new Communication(genericChannel, lora);
		else
			Communication::com = new Communication(genericChannel, nullptr);
	}

	return Communication::com;
}

int Communication::init() {
	return init(COMMode::STANDARD_COM_MODE);
}

int Communication::init(COMMode canMode) {
	memset(radioArray,0,MSG_SIZE);
	if(can->init(receptor, heartbeatCan, canMode) != 0)
		return -1;

	if(radio != nullptr) {
		if(radio->init(nullptr, heartbeatLora) != 0)
			return -1;
		loraActive = true;
	}

	return 0;
}

int Communication::exec() {
	if(can->exec() != 0)
		return -1;

	if(radio != nullptr) {
		if(radio->exec() != 0)
			return -1;
	}
	return 0;
}

void Communication::receptorLora(uint32_t id, uint8_t *data, uint32_t n) {
	Can_MessageId_t msgId =
	{ 0 };
	Can_MessageData_t msgData =
	{ 0 };

	msgId.uint32 = id;
	memcpy(msgData.uint8, data, 64); //TODO only copy n bytes
	uint8_t commandId = msgData.bit.cmd_id;
	uint8_t channelId = msgData.bit.info.channel_id;
	uint8_t ret_n = 0;

	if(channelId == 6) {// ECU
		if(loraActive) {
			radioArray[ECU_START_ADDR] = 1;
			memcpy(&radioArray[ECU_START_ADDR+1],msgData.bit.data.uint8,ECU_MSG_SIZE-1);
		}
		return;
	} else if(channelId == 7) {// PMU
		if(loraActive) {
			radioArray[ECU_START_ADDR] = 1;
			memcpy(&radioArray[PMU_START_ADDR+1],msgData.bit.data.uint8,PMU_MSG_SIZE-1);
		}
		return;
	}

	if(channelId == GENERIC_CHANNEL_ID) {
		if(genericChannel->processMessage(commandId, msgData.bit.data.uint8, ret_n) != 0)
			return;
	} else {
		if(genericChannel->processMessage(commandId, msgData.bit.data.uint8, ret_n, channelId) != 0)
			return;
	}

	msgId.info.direction = NODE2MASTER_DIRECTION;
	msgId.info.node_id = Communication::genericChannel->getNodeId();
	msgId.info.special_cmd = STANDARD_SPECIAL_CMD;
	msgId.info.priority = STANDARD_PRIORITY;
	msgData.bit.cmd_id = commandId + 1;

	(void) STRHAL_CAN_Send(STRHAL_FDCAN1, msgId.uint32, msgData.uint8, CAN_MSG_LENGTH(ret_n));
}

void Communication::receptor(uint32_t id, uint8_t *data, uint32_t n) {
	Can_MessageId_t msgId =
	{ 0 };
	Can_MessageData_t msgData =
	{ 0 };

	msgId.uint32 = id;
	memcpy(msgData.uint8, data, 64); //TODO only copy n bytes
	uint8_t commandId = msgData.bit.cmd_id;
	uint8_t channelId = msgData.bit.info.channel_id;
	uint8_t ret_n = 0;

	if(channelId == GENERIC_CHANNEL_ID) {
		if(genericChannel->processMessage(commandId, msgData.bit.data.uint8, ret_n) != 0)
			return;
	} else {
		if(genericChannel->processMessage(commandId, msgData.bit.data.uint8, ret_n, channelId) != 0)
			return;
	}

	msgId.info.direction = NODE2MASTER_DIRECTION;
	msgId.info.node_id = Communication::genericChannel->getNodeId();
	msgId.info.special_cmd = STANDARD_SPECIAL_CMD;
	msgId.info.priority = STANDARD_PRIORITY;
	msgData.bit.cmd_id = commandId + 1;

	(void) STRHAL_CAN_Send(STRHAL_FDCAN1, msgId.uint32, msgData.uint8, CAN_MSG_LENGTH(ret_n));
}

void Communication::heartbeatCan() {
	Can_MessageId_t msgId =
	{ 0 };
	msgId.info.special_cmd = STANDARD_SPECIAL_CMD;
	msgId.info.direction = NODE2MASTER_DIRECTION;
	msgId.info.node_id = Communication::genericChannel->getNodeId();
	msgId.info.priority = STANDARD_PRIORITY;

	Can_MessageData_t msgData =
	{ 0 };
	msgData.bit.cmd_id = GENERIC_RES_DATA;
	msgData.bit.info.channel_id = GENERIC_CHANNEL_ID;
	msgData.bit.info.buffer = DIRECT_BUFFER;

	uint8_t n = 0;
	if(genericChannel->getSensorData(&msgData.bit.data.uint8[0], n) != 0) { // Sensor Data collection failed, or Refresh Divider not yet met
		return;
	}

	if(loraActive) {
		radioArray[RCU_START_ADDR] = 1;
		memcpy(&radioArray[RCU_START_ADDR+1],msgData.bit.data.uint8,RCU_MSG_SIZE-1);
	}

	(void) STRHAL_CAN_Send(STRHAL_FDCAN1, msgId.uint32, msgData.uint8, n);
}

void Communication::heartbeatLora() {
	//sprintf((char *)radioArray,"hello world");
	//radio->send(0, radioArray, MSG_SIZE);
	//char *test = "hello world";
	//radio->send(0, (uint8_t *) test, MSG_SIZE);
}
