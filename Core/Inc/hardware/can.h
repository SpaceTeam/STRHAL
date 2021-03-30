#ifndef CAN_H_
#define CAN_H_

#include <stdint.h>

#include "generic_cmds.h"

#include "fdcan.h"

void Can_init(FDCAN_GlobalTypeDef *caninstance, uint32_t nodeid);
HAL_StatusTypeDef Can_addFilter(FDCAN_HandleTypeDef *handle, uint32_t filter_index, uint32_t mask, uint32_t filter_id, uint32_t target_location);

Result_t Can_processStandardMessage(uint32_t message_id, uint8_t *data, uint32_t length);
Result_t Can_sendMessage(FDCAN_GlobalTypeDef *caninstance, uint32_t message_id, uint8_t *data, uint32_t length);
void Can_checkFifo(FDCAN_GlobalTypeDef *caninstance);


#endif
