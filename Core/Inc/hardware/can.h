#ifndef CAN_H_
#define CAN_H_

#include <stdint.h>
#include "can_def.h"
#include "main.h"
#include "cmds.h"

typedef struct
{
	FDCAN_GlobalTypeDef *can;
	Can_Message_RAM *can_ram;
} Can_Handle;

Result_t Can_Init(uint8_t node_id);
void Can_checkFifo(uint32_t can_handle_index);
Result_t Can_sendMessage(uint32_t can_handle_index, uint32_t message_id, uint8_t *data, uint32_t length);

#endif
