#include "STRHAL.h"
#include <can_houbolt/can_cmds.h>
#include <Can.h>
#include <cstring>
#include <cstdio>

Com_Receptor_t Can::standardReceptor = nullptr;
uint32_t Can::_nodeId = 0; // TODO fix this pfusch

Can::Can(uint32_t nodeId) :
		AbstractCom(nodeId)
{
}

Can& Can::instance(uint32_t nodeId)
{
	static Can can(nodeId);

	if (nodeId != 0)
		_nodeId = nodeId;

	return can;
}

int Can::init(Com_Receptor_t receptor, Com_Heartbeat_t heartbeat)
{
	return Can::init(receptor, heartbeat, COMMode::STANDARD_COM_MODE);
}

int Can::init(Com_Receptor_t receptor, Com_Heartbeat_t heartbeat, COMMode mode)
{
	if(nodeId == 0)
		return -1;

	standardReceptor = receptor;

	if (STRHAL_CAN_Instance_Init(STRHAL_FDCAN1) != 0)
		return -1;

	//if (STRHAL_CAN_Instance_Init(STRHAL_FDCAN2) != 0)
	//	return -1;

	if (STRHAL_TIM_Heartbeat_Init(STRHAL_TIM_TIM7, 1600, 1000) != 100)
		return -1;

	if (STRHAL_TIM_Heartbeat_Subscribe(STRHAL_TIM_TIM7, heartbeat) != 0)
		return -1;

	if (mode == COMMode::STANDARD_COM_MODE)
	{
		Can_MessageId_t mask =
		{ 0 };
		mask.info.direction = 0x1;
		mask.info.node_id = 0x3F;
		mask.info.special_cmd = 0x3;

		Can_MessageId_t id =
		{ 0 };
		id.info.direction = MASTER2NODE_DIRECTION;
		id.info.special_cmd = STANDARD_SPECIAL_CMD;
		id.info.node_id = nodeId;
		Can_MessageId_t id2 =
		{ 0 };
		id2.info.direction = MASTER2NODE_DIRECTION;
		id2.info.special_cmd = STANDARD_SPECIAL_CMD;
		id2.info.node_id = 0;

		STRHAL_FDCAN_Filter_t mainFilter[] =
		{
		{ .value_id1 = id.uint32, .mask_id2 = mask.uint32, .type = FDCAN_FILTER_MASK },
		{ .value_id1 = id2.uint32, .mask_id2 = mask.uint32, .type = FDCAN_FILTER_MASK } };

		if (STRHAL_CAN_Subscribe(STRHAL_FDCAN1, STRHAL_FDCAN_RX0, mainFilter, 2, receptor) != 2)
			return -1;
	}
	else if (mode == COMMode::LISTENER_COM_MODE)
	{
		Can_MessageId_t mask =
		{ 0 };
		mask.info.direction = 0x1;
		mask.info.node_id = 0x1F;
		mask.info.special_cmd = 0x3;

		Can_MessageId_t id =
		{ 0 };
		id.info.direction = MASTER2NODE_DIRECTION;
		id.info.special_cmd = STANDARD_SPECIAL_CMD;
		id.info.node_id = nodeId;

		Can_MessageId_t id2 =
		{ 0 };
		id2.info.direction = MASTER2NODE_DIRECTION;
		id2.info.special_cmd = STANDARD_SPECIAL_CMD;
		id2.info.node_id = 0;

		Can_MessageId_t id3 =
		{ 0 };
		id3.info.direction = NODE2MASTER_DIRECTION;
		id3.info.special_cmd = STANDARD_SPECIAL_CMD;
		id3.info.node_id = 6;

		Can_MessageId_t id4 =
		{ 0 };
		id4.info.direction = NODE2MASTER_DIRECTION;
		id4.info.special_cmd = STANDARD_SPECIAL_CMD;
		id4.info.node_id = 7;

		STRHAL_FDCAN_Filter_t mainFilter[] =
		{
		{ .value_id1 = id.uint32, .mask_id2 = mask.uint32, .type = FDCAN_FILTER_MASK },
		{ .value_id1 = id2.uint32, .mask_id2 = mask.uint32, .type = FDCAN_FILTER_MASK },
		{ .value_id1 = id3.uint32, .mask_id2 = mask.uint32, .type = FDCAN_FILTER_MASK },
		{ .value_id1 = id4.uint32, .mask_id2 = mask.uint32, .type = FDCAN_FILTER_MASK } };

		if (STRHAL_CAN_Subscribe(STRHAL_FDCAN1, STRHAL_FDCAN_RX0, mainFilter, 4, receptor) != 4)
			return -1;
	}
	else if (mode == COMMode::BRIDGE_COM_MODE)
	{
		STRHAL_FDCAN_Filter_t mainFilter[] =
		{
		{ .value_id1 = 0x00, .mask_id2 = 0xFFFF, .type = FDCAN_FILTER_RANGE } };

		if (STRHAL_CAN_Subscribe(STRHAL_FDCAN1, STRHAL_FDCAN_RX0, mainFilter, 1, Can::internalReceptor) != 1)
			return -1;

		if (STRHAL_CAN_Subscribe(STRHAL_FDCAN2, STRHAL_FDCAN_RX0, mainFilter, 1, Can::externalReceptor) != 1)
			return -1;
	}

	return 0;
}

int Can::exec()
{
	STRHAL_CAN_Run();
	if (STRHAL_TIM_Heartbeat_StartHeartbeat(STRHAL_TIM_TIM7) != 0)
		return -1;

	return 0;
}

int Can::send(uint32_t id, uint8_t *data, uint8_t n)
{
	Can_MessageId_t msgId =
	{ 0 };
	msgId.info.special_cmd = STANDARD_SPECIAL_CMD;
	msgId.info.direction = NODE2MASTER_DIRECTION;
	if (id == 0)
	{
		msgId.info.node_id = _nodeId;
	}
	else
	{
		msgId.info.node_id = id;
	}
	msgId.info.priority = STANDARD_PRIORITY;

	Can_MessageData_t msgData =
	{ 0 };

	memcpy(msgData.uint8, data, n);

	(void) STRHAL_CAN_Send(STRHAL_FDCAN1, msgId.uint32, msgData.uint8, n);
	return 0;
}

void Can::sendAsMaster(uint8_t receiverNodeId, uint8_t receiverChannelId, uint8_t commandId, uint8_t *data, uint8_t n)
{
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

void Can::bridgeReceptor(STRHAL_FDCAN_Id_t bus_id, uint32_t id, uint8_t *data, uint32_t n)
{
	Can_MessageId_t incoming_id;
	incoming_id.uint32 = id;

	if (incoming_id.info.node_id == _nodeId)
	{
		Can::standardReceptor(id, data, n);
	}
	else if (incoming_id.info.node_id == 0)
	{
		STRHAL_CAN_Send(bus_id, id, data, n);
		Can::standardReceptor(id, data, n);
	}
	else
	{
		STRHAL_CAN_Send(bus_id, id, data, n);
	}
}

void Can::internalReceptor(uint32_t id, uint8_t *data, uint32_t n)
{
	//Can::bridgeReceptor(STRHAL_FDCAN2, id, data, n);
	STRHAL_CAN_Send(STRHAL_FDCAN2, id, data, n);
}

void Can::externalReceptor(uint32_t id, uint8_t *data, uint32_t n)
{
	Can::bridgeReceptor(STRHAL_FDCAN1, id, data, n);
}
