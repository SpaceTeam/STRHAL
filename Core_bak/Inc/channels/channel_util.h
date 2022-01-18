#ifndef CHANNEL_UTIL_H_
#define CHANNEL_UTIL_H_


#include "can_cmds.h"
#include "channels.h"
#include "main.h"

void ChannelUtil_DefaultMessageId(Can_MessageId_t *msg_id);
Result_t ChannelUtil_GetVariable(Channel_t *channel, GetMsg_t *get_msg, uint8_t response_cmd);
Result_t ChannelUtil_Ack(Channel_t *channel, uint8_t response_cmd);

#endif
