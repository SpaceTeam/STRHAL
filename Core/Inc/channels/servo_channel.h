#ifndef SERVO_CHANNEL_H_
#define SERVO_CHANNEL_H_

#include "servo_channel_def.h"
#include "main.h"

typedef struct
{
	uint32_t position;
	uint32_t position_percentage;
	uint32_t target_percentage;
	uint32_t target_position;
	uint32_t target_pressure;
	uint32_t max_speed;
	uint32_t max_accel;
	uint32_t max_torque;
	uint32_t p_param;
	uint32_t i_param;
	uint32_t d_param;
	uint32_t sensor_channel_id;
	uint32_t startpoint;
	uint32_t endpoint;
	uint32_t pwm_in_enabled;
	uint32_t refresh_divider;
//other servo stuff
} Servo_Channel_t;

Result_t Servo_InitChannel(Servo_Channel_t *servo);
Result_t Servo_SetPosition(Servo_Channel_t *servo, uint32_t input);
Result_t Servo_SetRelativePosition(Servo_Channel_t *servo, int16_t distance);
Result_t Servo_ProcessMessage(uint8_t ch_id, uint8_t cmd_id, uint8_t *data, uint32_t length);
Result_t Servo_GetData(uint8_t ch_id, uint8_t *data, uint32_t *length);
Result_t Servo_Update(Servo_Channel_t *servo);
uint32_t* Servo_VariableSelection(Servo_Channel_t *servo, uint8_t var_id, uint8_t ch_id);

#endif
