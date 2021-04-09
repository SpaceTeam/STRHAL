#ifndef UI_H_
#define UI_H_

#include "main.h"
#include "cmds.h"



typedef Result_t (* can_function)(uint32_t *);

Result_t Ui_processCanMessage(uint32_t message_id, uint8_t *data, uint32_t length);

#endif
