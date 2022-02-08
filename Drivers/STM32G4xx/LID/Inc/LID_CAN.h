#ifndef LID_CAN_H
#define LID_CAN_H

#include "LID_CAN_Def.h"
#include "LID.h"
#include "LID_Oof.h"
#include <stm32g4xx.h>
#include <stm32g4xx_ll_bus.h>
#include <stm32g4xx_ll_rcc.h>
#include <stm32g4xx_ll_utils.h>

/***** DEFINES *****/
#define LID_CAN_START_TOT 				16000000
#define LID_CAN_STD_FILTER_NUMBER		0
#define LID_CAN_EXT_FILTER_NUMBER		0

/* ******************************* */
#define CAN_MAX_DATA_SIZE 64
#define CAN_MSG_LENGTH(payload_length) ( sizeof(LID_CAN_MessageDataInfo_t) + sizeof(uint8_t) + payload_length )

typedef enum
{
	MASTER2NODE_DIRECTION, NODE2MASTER_DIRECTION,
} LID_CAN_MessageDirection_t;

typedef enum
{
	URGENT_PRIORITY, HIGH_PRIORITY, STANDARD_PRIORITY, LOW_PRIORITY
} LID_CAN_MessagePriority_t;

typedef enum
{
	ABORT_SPECIAL_CMD, CLOCK_SYNC_SPECIAL_CMD,	// DIR = MASTER2NODE_DIRECTION
	ERROR_SPECIAL_CMD = CLOCK_SYNC_SPECIAL_CMD, // DIR = NODE2MASTER_DIRECTION
	INFO_SPECIAL_CMD,
	STANDARD_SPECIAL_CMD,
} LID_CAN_MessageSpecialCmd_t;

typedef enum
{
	DIRECT_BUFFER, ABSOLUTE_BUFFER, RELATIVE_BUFFER, RESERVED_BUFFER
} LID_CAN_MessageBuffer_t;

typedef union
{
	struct __attribute__((__packed__))
	{
		uint32_t direction :1;		//bit:    0   | CAN_MessageDirection_t
		uint32_t node_id :6;		//bit:  6-1   | Node ID: 0 - 63
		uint32_t special_cmd :2;	//bit:  8-7   | CAN_MessageSpecialCmd_t
		uint32_t priority :2;		//bit: 10-9   | CAN_MessagePriority_t
	} info;
	uint32_t word;
} LID_CAN_MessageId_t;

typedef struct __attribute__((__packed__))
{
	uint32_t channel_id :6;			//bit:  5-0   | Channel ID: 0 - 63
	LID_CAN_MessageBuffer_t buffer :2;	//bit:  7-6   | CAN_MessageBuffer_t
} LID_CAN_MessageDataInfo_t;

typedef union
{
	struct __attribute__((__packed__))
	{
		LID_CAN_MessageDataInfo_t info;
		uint8_t cmd_id;
		uint8_t data[(CAN_MAX_DATA_SIZE - CAN_MSG_LENGTH(0))];
	} bit;
	uint8_t byte[CAN_MAX_DATA_SIZE];
} LID_CAN_MessageData_t;

typedef enum {
	LID_FDCAN1,
	LID_FDCAN2,

	LID_N_FDCAN
} LID_FDCAN_Id_t;

LID_Oof_t LID_CAN_Init();
int LID_CAN_Instance_Init(LID_FDCAN_Id_t fdcan_id);
void LID_CAN_Run();
int LID_CAN_Send(LID_FDCAN_Id_t fdcan_id, LID_CAN_MessageId_t message_id, LID_CAN_MessageData_t *data, uint32_t length);
uint8_t LID_CAN_Receive(LID_FDCAN_Id_t fdcan_id, LID_CAN_MessageId_t *id, LID_CAN_MessageData_t *can_data, uint32_t *length);

#endif
