#include "../Inc/CANCOM.h"
#include "../Inc/Channels/AbstractChannel.h"
#include "../Inc/Channels/GenericChannel.h"
#include "LID.h"
#include <can_houbolt/can_cmds.h>
#include <cstring>

GenericChannel *CANCOM::generic_ch = nullptr;
uint32_t CANCOM::_node_id = 0;

CANCOM::CANCOM(uint32_t node_id, GenericChannel & generic_ch) : AbstractCOM(node_id) {
	CANCOM::generic_ch = &generic_ch;
	_node_id = node_id;
}

COMState CANCOM::init() {
	state = COMState::SBY;

	if(LID_CAN_Instance_Init(LID_FDCAN1) != 0)
		return state = COMState::ERR;

	if(LID_CAN_Instance_Init(LID_FDCAN2) != 0)
		return state = COMState::ERR;

	if(LID_TIM_Burner_Init(LID_TIM_TIM6, 3200, 1000) != 50)
		return state = COMState::ERR;

	if(LID_TIM_Burner_Subscribe(LID_TIM_TIM6, CANCOM::burner) != 0)
		return state = COMState::ERR;

	uint32_t mask = 0x1 | (0x1F << 1) | (0x3 << 7);
	uint32_t id = MASTER2NODE_DIRECTION | (node_id << 1) | (STANDARD_SPECIAL_CMD << 7); // Filter for msgs only meant for this node
	uint32_t id2 = MASTER2NODE_DIRECTION | (0 << 1) | (STANDARD_SPECIAL_CMD << 7); // Filter for generic msgs

	LID_FDCAN_Filter_t mainFilter[] = {
		{.value = id, .mask = mask},
		{.value = id2, .mask = mask}
	};

	if(LID_CAN_Subscribe(LID_FDCAN1, LID_FDCAN_RX0, mainFilter, 2, CANCOM::mainReceptor) != 2)
		return state = COMState::ERR;

	return state;
}

COMState CANCOM::exec() {
	state = COMState::RUN;
	LID_CAN_Run();
	if(LID_TIM_Burner_Start2Burn(LID_TIM_TIM6) != 0)
			return state = COMState::ERR;

	return state;
}

void CANCOM::mainReceptor(uint32_t id, uint8_t *data, uint32_t n) {
	Can_MessageId_t msg_id =
	{ 0 };
	Can_MessageData_t *msg_data =
	{ 0 };
	SetMsg_t *set_msg =
	{ 0 };
	msg_id.uint32 = id;
	msg_data = (Can_MessageData_t *) data;
	memcpy(set_msg, &msg_data->bit.data.uint8[0], 5);
	uint8_t cmd_id = msg_data->bit.cmd_id;
	uint8_t ch_id = msg_data->bit.info.channel_id;
	uint8_t var_id = set_msg->variable_id;
	uint32_t ch_val = set_msg->value;
	uint32_t ret = 0;

	if(ch_id == GENERIC_CHANNEL_ID) {
		if(CANCOM::generic_ch->prcMsg(cmd_id, var_id, ch_val, ret) == 0)
			return;
	} else {
		if(CANCOM::generic_ch->prcMsg(cmd_id, var_id, ch_val, ret, ch_id) == 0)
			return;
	}

	if(cmd_id < COMMON_TOTAL_CMDS) {
		msg_id.info.direction = NODE2MASTER_DIRECTION;
		msg_data->bit.cmd_id++;
		set_msg->value = ret;
		memcpy(&msg_data->bit.data.uint8[0], set_msg, 5);
		LID_CAN_Send(LID_FDCAN1, msg_id.uint32, msg_data->uint8, n);
	}
}

void CANCOM::burner() {
	Can_MessageId_t msg_id =
	{ 0 };
	msg_id.info.special_cmd = STANDARD_SPECIAL_CMD;
	msg_id.info.direction = NODE2MASTER_DIRECTION;
	msg_id.info.node_id = _node_id;
	msg_id.info.priority = STANDARD_PRIORITY;

	Can_MessageData_t msg_data =
	{ 0 };
	msg_data.bit.cmd_id = GENERIC_RES_DATA;
	msg_data.bit.info.channel_id = GENERIC_CHANNEL_ID;
	msg_data.bit.info.buffer = DIRECT_BUFFER;

	DataMsg_t *data = (DataMsg_t*) &msg_data.bit.data;
	uint8_t n = 0;
	if(CANCOM::generic_ch->getSensorData(data->uint8, n) != 0)
		return;
	//TODO: Channel Mask in DataMsg_t
	LID_CAN_Send(LID_FDCAN1, msg_id.uint32, msg_data.uint8, n);
}
