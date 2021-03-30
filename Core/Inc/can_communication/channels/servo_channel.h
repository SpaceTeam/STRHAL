#ifndef SERVO_CHANNEL_H__
#define SERVO_CHANNEL_H__

#include "cmds.h"

typedef enum
{
	SERVO_RESET_SETTINGS,
	SERVO_SETPOS,
	SERVO_SETSPEED,
	SERVO_STATUS,
	SERVO_GET_SETPOINT,
	SERVO_SET_SENSORCHANNEL,
	SERVO_SET_PID_PARAMETERS,


	SERVO_TOTAL_CMDS
} SERVO_CMDs;

extern const can_function servo_array[];


typedef struct
{
	uint8_t channel_index; //ToDo: Probably unnecessary
	uint_least16_t position_fb;
	uint_least16_t position_set;
	//other servo stuff
}Servo_t;

Result_t Servo_getData(void *channel, uint8_t *array, uint8_t *length);
uint32_t Servo_getStatus(void *channel);










#endif
