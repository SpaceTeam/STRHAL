#ifndef GENERIC_CMDS_H__
#define GENERIC_CMDS_H__

#include "cmds.h"
#include "adc16_channel.h"
#include "adc16_single_channel.h"
#include "adc24_channel.h"
#include "digital_out_channel.h"
#include "servo_channel.h"



typedef union
{
	Adc16_t adc16;
	Adc16_single_t adc16_single;
	Adc24_t adc24;
	Digital_out_t digital_out;
	Servo_t servo;
}Channel_t;


typedef struct
{
	const char * firmware_version;
	uint_least32_t channel_number;
	uint_least8_t channel_type[32];
	Channel_t* channel[32];
}Node_t;

extern Node_t node;




typedef enum
{
	GENERIC_RESET_ALL_SETTINGS,
	GENERIC_SYNC_CLOCK,
	GENERIC_NODE_INFO,
	GENERIC_NODE_STATUS,
	GENERIC_CHANNEL_STATUS,			// parameter is channel mask returns all flags of channels in mask
	GENERIC_SEND_DATA,
	GENERIC_SET_SPEAKER,
	GENERIC_SET_REFRESHRATE,
	GENERIC_ENABLE_UART_DEBUGGING,

	GENERIC_TOTAL_CMDS
} GENERIC_CMDs;



typedef enum
{
	ERROR_FLAG_CAN_COMM1,
	ERROR_FLAG_CAN_COMM2,
	ERROR_FLAG_PWR_BUS1,
	ERROR_FLAG_PWR_BUS2,
	ERROR_FLAG_OVERCURRENT_THLD1,
	ERROR_FLAG_RESERVED
} ERROR_FLAG_INDEX;

typedef struct
{

	uint_least32_t firmware_version;
	uint_least32_t channel_number;
	uint_least8_t channel_type[32];
}NodeInfoMsg_t;

typedef struct
{
	uint_least32_t node_error_flags;
	uint_least32_t channels_errors;
}NodeStatusMsg_t;

typedef struct
{
	uint_least16_t tone_frequency;
	uint_least16_t on_time;
	uint_least16_t off_time;
	uint_least8_t count;
}SetSpeakerMsg_t;






typedef Result_t (*const getData_function)(void * channel, uint8_t *data, uint8_t *length); //ToDo Fix Void * to Channel_t * safely
typedef uint32_t (*const getStatus_function)(void * channel); //ToDo Fix Void * to Channel_t * safely


typedef struct
{
 const can_function *cmds;
 const uint8_t last_index;
 getData_function getData;
 getStatus_function getStatus;
}ChannelCmds;

extern const ChannelCmds channel_type_array[];

Result_t SyncClock(uint32_t *data);
Result_t SendNodeInfo(uint32_t *data);
Result_t SendNodeStatus(uint32_t *data);
Result_t SendData(uint32_t *data);
Result_t SetSpeaker(uint32_t *data);


#endif
