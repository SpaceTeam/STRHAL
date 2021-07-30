#include "channel_util.h"
#include "channels.h"
#include "main.h"
void ChannelUtil_DefaultMessageId(Can_MessageId_t *msg_id)
{
	msg_id->info.special_cmd = STANDARD_SPECIAL_CMD;
	msg_id->info.direction = NODE2MASTER_DIRECTION;
	//msg_id->info.direction = MASTER2NODE_DIRECTION; //TODO REMOVE: Just here for debugging
	msg_id->info.node_id = node.node_id;
	msg_id->info.priority = STANDARD_PRIORITY;
}
