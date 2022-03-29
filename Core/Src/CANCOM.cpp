#include "../Inc/CANCOM.h"
#include "../Inc/Channels/AbstractChannel.h"
#include "../Inc/Channels/GenericChannel.h"
#include "LID.h"
#include <can_houbolt/can_cmds.h>
#include <cstring>
#include <cstdio>

CANCOM* CANCOM::cancom = nullptr;
GenericChannel* CANCOM::generic_ch = nullptr;

CANCOM::CANCOM(GenericChannel *g_ch)
	: AbstractCOM(g_ch->getNodeId()) {
	CANCOM::generic_ch = g_ch;
}

CANCOM* CANCOM::instance(GenericChannel *gc) {
	if(CANCOM::cancom == nullptr && gc != nullptr) {
		CANCOM::cancom = new CANCOM(gc);
	}

	return CANCOM::cancom;
}

COMState CANCOM::init() {
	state = COMState::SBY;

	if(LID_CAN_Instance_Init(LID_FDCAN1) != 0)
		return state = COMState::ERR;

	if(LID_CAN_Instance_Init(LID_FDCAN2) != 0)
		return state = COMState::ERR;

	if(LID_TIM_Burner_Init(LID_TIM_TIM7, 160, 100) != 10000) //TODO: create Interface for burn interval calculation based on maximal bus bandwidth
		return state = COMState::ERR;

	if(LID_TIM_Burner_Subscribe(LID_TIM_TIM7, CANCOM::burner) != 0)
		return state = COMState::ERR;

	Can_MessageId_t mask =
	{ 0 };
	mask.info.direction = 0x1;
	mask.info.node_id = 0x1F;
	mask.info.special_cmd = 0x3;

	Can_MessageId_t id =
	{ 0 };
	id.info.direction = MASTER2NODE_DIRECTION;
	id.info.special_cmd = STANDARD_SPECIAL_CMD;
	id.info.node_id = CANCOM::generic_ch->getNodeId();
	Can_MessageId_t id2 =
	{ 0 };
	id2.info.direction = MASTER2NODE_DIRECTION;
	id2.info.special_cmd = STANDARD_SPECIAL_CMD;
	id2.info.node_id = 0;

	LID_FDCAN_Filter_t mainFilter[] = {
		{.value = id.uint32, .mask = mask.uint32},
		{.value = id2.uint32, .mask = mask.uint32}
	};

	if(LID_CAN_Subscribe(LID_FDCAN1, LID_FDCAN_RX0, mainFilter, 2, CANCOM::mainReceptor) != 2)
		return state = COMState::ERR;

	return state;
}

COMState CANCOM::exec() {
	state = COMState::RUN;
	LID_CAN_Run();
	if(LID_TIM_Burner_Start2Burn(LID_TIM_TIM7) != 0)
		return state = COMState::ERR;

	return state;
}

void CANCOM::mainReceptor(uint32_t id, uint8_t *data, uint32_t n) {
	Can_MessageId_t msg_id =
	{ 0 };
	Can_MessageData_t msg_data =
	{ 0 };

	msg_id.uint32 = id;
	memcpy(msg_data.uint8,data,64); //TODO only copy n bytes
	uint8_t cmd_id = msg_data.bit.cmd_id;
	uint8_t ch_id = msg_data.bit.info.channel_id;
	uint8_t ret_n = 0;

	if(ch_id == GENERIC_CHANNEL_ID) {
		if(generic_ch->prcMsg(cmd_id, msg_data.bit.data.uint8, ret_n) != 0)
			return;
	} else {
		if(generic_ch->prcMsg(cmd_id, msg_data.bit.data.uint8, ret_n, ch_id) != 0)
			return;
	}

	msg_id.info.direction = NODE2MASTER_DIRECTION;
	msg_id.info.node_id = CANCOM::generic_ch->getNodeId();
	msg_id.info.special_cmd = STANDARD_SPECIAL_CMD;
	msg_id.info.priority = STANDARD_PRIORITY;
	msg_data.bit.cmd_id = cmd_id + 1;

	(void) LID_CAN_Send(LID_FDCAN1, msg_id.uint32, msg_data.uint8, CAN_MSG_LENGTH(ret_n));
}

void CANCOM::burner() {
	//LID_UART_Write("BURN\n", 5);
	Can_MessageId_t msg_id =
	{ 0 };
	msg_id.info.special_cmd = STANDARD_SPECIAL_CMD;
	msg_id.info.direction = NODE2MASTER_DIRECTION;
	msg_id.info.node_id = CANCOM::generic_ch->getNodeId();
	msg_id.info.priority = STANDARD_PRIORITY;

	Can_MessageData_t msg_data =
	{ 0 };
	msg_data.bit.cmd_id = GENERIC_RES_DATA;
	msg_data.bit.info.channel_id = GENERIC_CHANNEL_ID;
	msg_data.bit.info.buffer = DIRECT_BUFFER;

	uint8_t n = 0;
	if(generic_ch->getSensorData(&msg_data.bit.data.uint8[0], n) != 0) { // Sensor Data collection failed, or Refresh Divider not yet met
		return;
	}

	(void) LID_CAN_Send(LID_FDCAN1, msg_id.uint32, msg_data.uint8, n);
}

CANCOM::~CANCOM() {}
