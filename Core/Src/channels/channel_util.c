#include "channel_util.h"
#include "main.h"
void ChannelUtil_DefaultMessageId(Can_MessageId_t *msg_id)
{
	msg_id->info.special_cmd = STANDARD_SPECIAL_CMD;
	//msg_id->info.direction = NODE2MASTER_DIRECTION;
	msg_id->info.direction = MASTER2NODE_DIRECTION; //TODO REMOVE
	msg_id->info.node_id = NODE_ID;
	msg_id->info.priority = STANDARD_PRIORITY;
}
