#ifndef UI_H_
#define UI_H_

#include "main.h"
#include "cmds.h"

Result_t Ui_SendCanMessage(uint32_t can_index, Can_MessageId_t message_id, Can_MessageData_t * data, uint32_t length);
Result_t Ui_ProcessCanMessage(Can_MessageId_t message_id, Can_MessageData_t *data, uint32_t length);

#endif
