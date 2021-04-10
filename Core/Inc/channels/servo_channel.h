#ifndef SERVO_CHANNEL_H_
#define SERVO_CHANNEL_H_

#include "servo_channel_def.h"

typedef struct
{
	uint_least16_t position_fb;
	uint_least16_t position_set;
	//other servo stuff
}Servo_Channel_t;


#endif