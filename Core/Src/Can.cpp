#include "../Inc/Channels/AbstractChannel.h"
#include "../Inc/Channels/GenericChannel.h"
#include "STRHAL.h"
#include <can_houbolt/can_cmds.h>
#include <Can.h>
#include <cstring>
#include <cstdio>

Can *Can::cancom = nullptr;
GenericChannel *Can::genericChannel = nullptr;

Can::Can(GenericChannel *genericChannel)
	: AbstractCom(genericChannel->getNodeId()) {
	Can::genericChannel = genericChannel;
}

Can *Can::instance(GenericChannel *genericChannel) {
	if(Can::cancom == nullptr && genericChannel != nullptr) {
		Can::cancom = new Can(genericChannel);
	}

	return Can::cancom;
}

COMState Can::init() {
	return Can::init(COMMode::STANDARD_COM_MODE);
}

COMState Can::init(COMMode mode) {
	state = COMState::SBY;

	if(STRHAL_CAN_Instance_Init(STRHAL_FDCAN1) != 0)
		return state = COMState::ERR;

	if(STRHAL_CAN_Instance_Init(STRHAL_FDCAN2) != 0)
		return state = COMState::ERR;

	if(STRHAL_TIM_Heartbeat_Init(STRHAL_TIM_TIM7, 160, 100) != 10000) //TODO: create Interface for burn interval calculation based on maximal bus bandwidth
		return state = COMState::ERR;

	if(STRHAL_TIM_Heartbeat_Subscribe(STRHAL_TIM_TIM7, Can::heartbeat) != 0)
		return state = COMState::ERR;

	if(mode == COMMode::STANDARD_COM_MODE) {
		Can_MessageId_t mask =
		{ 0 };
		mask.info.direction = 0x1;
		mask.info.node_id = 0x1F;
		mask.info.special_cmd = 0x3;

		Can_MessageId_t id =
		{ 0 };
		id.info.direction = MASTER2NODE_DIRECTION;
		id.info.special_cmd = STANDARD_SPECIAL_CMD;
		id.info.node_id = Can::genericChannel->getNodeId();
		Can_MessageId_t id2 =
		{ 0 };
		id2.info.direction = MASTER2NODE_DIRECTION;
		id2.info.special_cmd = STANDARD_SPECIAL_CMD;
		id2.info.node_id = 0;

		STRHAL_FDCAN_Filter_t mainFilter[] = {
			{ .value_id1 = id.uint32, .mask_id2 = mask.uint32, .type = FDCAN_FILTER_MASK },
			{ .value_id1 = id2.uint32, .mask_id2 = mask.uint32, .type = FDCAN_FILTER_MASK }
		};

		if (STRHAL_CAN_Subscribe(STRHAL_FDCAN1, STRHAL_FDCAN_RX0, mainFilter, 2, Can::standardReceptor) != 2)
			return state = COMState::ERR;
	} else if (mode == COMMode::BRIDGE_COM_MODE) {
		STRHAL_FDCAN_Filter_t mainFilter[] = {
			{ .value_id1 = 0x00, .mask_id2 = 0xFFFF, .type = FDCAN_FILTER_RANGE }
		};

		if (STRHAL_CAN_Subscribe(STRHAL_FDCAN1, STRHAL_FDCAN_RX0, mainFilter, 1, Can::internalReceptor) != 2)
			return state = COMState::ERR;

		if (STRHAL_CAN_Subscribe(STRHAL_FDCAN2, STRHAL_FDCAN_RX0, mainFilter, 1, Can::externalReceptor) != 2)
			return state = COMState::ERR;
	}

	return state;
}

COMState Can::exec() {
	state = COMState::RUN;
	STRHAL_CAN_Run();
	if(STRHAL_TIM_Heartbeat_StartHeartbeat(STRHAL_TIM_TIM7) != 0)
		return state = COMState::ERR;

	return state;
}

void Can::sendAsMaster(uint8_t receiverNodeId, uint8_t receiverChannelId, uint8_t commandId, uint8_t *data, uint8_t n) {
	Can_MessageId_t msgId =
	{ 0 };
	msgId.info.special_cmd = STANDARD_SPECIAL_CMD;
	msgId.info.direction = MASTER2NODE_DIRECTION;
	msgId.info.node_id = receiverNodeId;
	msgId.info.priority = STANDARD_PRIORITY;

	Can_MessageData_t msgData =
	{ 0 };
	msgData.bit.cmd_id = commandId;
	msgData.bit.info.channel_id = receiverChannelId;
	msgData.bit.info.buffer = DIRECT_BUFFER;

	memcpy(msgData.bit.data.uint8, data, n);

	(void) STRHAL_CAN_Send(STRHAL_FDCAN1, msgId.uint32, msgData.uint8, n);
}

void Can::standardReceptor(uint32_t id, uint8_t *data, uint32_t n) {
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
	msgId.info.node_id = Can::genericChannel->getNodeId();
	msgId.info.special_cmd = STANDARD_SPECIAL_CMD;
	msgId.info.priority = STANDARD_PRIORITY;
	msgData.bit.cmd_id = commandId + 1;

	(void) STRHAL_CAN_Send(STRHAL_FDCAN1, msgId.uint32, msgData.uint8, CAN_MSG_LENGTH(ret_n));
}

void Can::heartbeat() {
	Can_MessageId_t msgId =
	{ 0 };
	msgId.info.special_cmd = STANDARD_SPECIAL_CMD;
	msgId.info.direction = NODE2MASTER_DIRECTION;
	msgId.info.node_id = Can::genericChannel->getNodeId();
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

	(void) STRHAL_CAN_Send(STRHAL_FDCAN1, msgId.uint32, msgData.uint8, n);
}

void Can::bridgeReceptor(STRHAL_FDCAN_Id_t bus_id, uint32_t id, uint8_t *data, uint32_t n) {
	Can_MessageId_t incoming_id;
	incoming_id.uint32 = id;

	if(incoming_id.info.node_id == cancom->nodeId)
		Can::standardReceptor(id, data, n);
	else
		STRHAL_CAN_Send(bus_id, id, data, n);
}

void Can::internalReceptor(uint32_t id, uint8_t *data, uint32_t n) {
	Can::bridgeReceptor(STRHAL_FDCAN2, id, data, n);
}

void Can::externalReceptor(uint32_t id, uint8_t *data, uint32_t n) {
	Can::bridgeReceptor(STRHAL_FDCAN1, id, data, n);
}

Can::~Can() {}
