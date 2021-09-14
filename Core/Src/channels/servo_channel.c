#include "servo_channel.h"
#include "main.h"

#include "pneumatic_valve_channel.h"
#include "channel_util.h"
#include "channels.h"
#include "ui.h"

#include "serial.h"
#include <string.h>
#include <stdio.h>

Result_t Servo_ResetSettings(Channel_t *channel);
Result_t Servo_Status(Channel_t *channel);
Result_t Servo_SetVariable(Channel_t *channel, SetMsg_t *set_msg);
Result_t Servo_GetVariable(Channel_t *channel, GetMsg_t *get_msg, PNEUMATIC_VALVE_CMDs response_cmd);

Result_t Servo_InitChannel(Servo_Channel_t *servo)
{

	servo->position = 0;
	servo->target_percentage = 0;
	servo->target_position = 0;
	servo->target_pressure = 0;
	servo->max_speed = 0;
	servo->max_accel = 0;
	servo->max_torque = 0;
	servo->p_param = 0;
	servo->i_param = 0;
	servo->d_param = 0;
	servo->startpoint = 0;
	servo->endpoint = 0;
	servo->pwm_in_enabled = 0;
	servo->refresh_divider = 0;

	if (node.channels[BLMB_SENSOR_CHANNEL].type != CHANNEL_TYPE_ADC16)
		return OOF;

	return NOICE;
}

static uint32_t readonly_var = 0;
uint32_t* Servo_VariableSelection(Servo_Channel_t *servo, uint8_t var_id, uint8_t ch_id)
{
	readonly_var = 0;
	switch (var_id)
	{
		case SERVO_POSITION:
			readonly_var = servo->position;
			return &readonly_var;
		case SERVO_TARGET_POSITION:
			return &servo->target_position;
		case SERVO_TARGET_PRESSURE:
			return &servo->target_pressure;
		case SERVO_MAX_SPEED:
			return &servo->max_speed;
		case SERVO_MAX_ACCEL:
			return &servo->max_accel;
		case SERVO_MAX_TORQUE:
			return &servo->max_torque;
		case SERVO_P_PARAM:
			return &servo->p_param;
		case SERVO_I_PARAM:
			return &servo->i_param;
		case SERVO_D_PARAM:
			return &servo->d_param;
		case SERVO_SENSOR_CHANNEL_ID:
			return &servo->sensor_channel_id;
		case SERVO_POSITION_STARTPOINT:
			return &servo->startpoint;
		case SERVO_POSITION_ENDPOINT:
			return &servo->endpoint;
		case SERVO_PWM_ENABLED:
			return &servo->pwm_in_enabled;
		case SERVO_SENSOR_REFRESH_DIVIDER:
			return &servo->refresh_divider;
		default:
			return NULL;
	}
}

Result_t Servo_ResetSettings(Channel_t *channel)
{
	Serial_PrintString("ResetSettings");
	return OOF_NOT_IMPLEMENTED;
}
Result_t Servo_SetPosition(Servo_Channel_t *servo, uint32_t input)
{
	int32_t position = (int64_t) input * ((int32_t) servo->endpoint - (int32_t) servo->startpoint) / UINT16_MAX;
	position += (int32_t) servo->startpoint;

	/*int32_t position = (int64_t) input * ((int32_t) servo->endpoint - (int32_t) servo->startpoint) / ((int32_t) UINT16_MAX);
	 position += position >> 1;
	 position += (int32_t) servo->startpoint;
	 position -= (int32_t) (servo->endpoint - servo->startpoint) / 4;
	 */
	servo->target_percentage = input;
	servo->target_position = position;
	return NOICE;
}

Result_t Servo_SetRelativePosition(Servo_Channel_t *servo, int16_t distance)
{
	Serial_PutString("Move: ");
	Serial_PrintInt(servo->target_position);
	servo->target_position += distance;
	return NOICE;
}

Result_t Servo_Status(Channel_t *channel)
{
	Serial_PrintString("Status");
	return OOF_NOT_IMPLEMENTED;
}

Result_t Servo_SetVariable(Channel_t *channel, SetMsg_t *set_msg)
{

	if (set_msg->variable_id == SERVO_TARGET_POSITION)
	{
		Servo_SetPosition(&channel->channel.servo, set_msg->value);
	}
	else
	{
		uint32_t *var = Servo_VariableSelection(&channel->channel.servo, set_msg->variable_id, channel->id);
		if (var == NULL)
			return OOF;
		*var = set_msg->value;
	}
	return Servo_GetVariable(channel, (GetMsg_t*) set_msg, SERVO_RES_SET_VARIABLE);
}

Result_t Servo_GetVariable(Channel_t *channel, GetMsg_t *get_msg, PNEUMATIC_VALVE_CMDs response_cmd)
{
	return ChannelUtil_GetVariable(channel, get_msg, response_cmd);
}

Result_t Servo_Move(Channel_t *channel, ServoMoveMsg_t *move_msg)
{
	Serial_PutString("Move: position: ");
	Serial_PutInt(move_msg->position);

	Serial_PutString(", interval: ");
	Serial_PrintInt(move_msg->interval);
	return OOF_NOT_IMPLEMENTED;
}

Result_t Servo_ProcessMessage(uint8_t ch_id, uint8_t cmd_id, uint8_t *data, uint32_t length)
{
	if (node.channels[ch_id].type != CHANNEL_TYPE_SERVO)
		return OOF_WRONG_CHANNEL_TYPE;
	Channel_t *channel = &node.channels[ch_id];
	Serial_PrintString("Servo ProcessMessage");
	switch (cmd_id)
	{
		case SERVO_REQ_RESET_SETTINGS:
			return Servo_ResetSettings(channel);
		case SERVO_REQ_STATUS:
			return Servo_Status(channel);
		case SERVO_REQ_SET_VARIABLE:
			return Servo_SetVariable(channel, (SetMsg_t*) data);
		case SERVO_REQ_GET_VARIABLE:
			return Servo_GetVariable(channel, (GetMsg_t*) data, SERVO_RES_GET_VARIABLE);
		case SERVO_REQ_MOVE:
			return Servo_Move(channel, (ServoMoveMsg_t*) data);
		default:
			return OOF_UNKNOWN_CMD;
	}
}

static Result_t Servo_GetRawData(uint8_t channel_id, uint16_t *data)
{
	Servo_Channel_t *servo = &node.channels[channel_id].channel.servo;
	int64_t value = (int16_t) servo->position << 2;

	value -= (int32_t) servo->startpoint;
	int32_t position = (int64_t) value * UINT16_MAX / ((int32_t) servo->endpoint - (int32_t) servo->startpoint);
	position = (position < 0) ? 0 : position;
	position = (position > UINT16_MAX) ? UINT16_MAX : position;
	*data = position;
	servo->position_percentage = position;
	//TODO @ANDI if (No new data)  return OOF_NO_NEW_DATA;
	return NOICE;
}

Result_t Servo_GetData(uint8_t ch_id, uint8_t *data, uint32_t *length)
{
	uint16_t *out = (uint16_t*) (data + *length);
	uint16_t new_data;
	Result_t result = Servo_GetRawData(ch_id, &new_data);
	if (result != NOICE)
		return result;
	*out = new_data;
#ifdef DEBUG_DATA
	Serial_PutInt(new_data);
	Serial_PutString(", ");
#endif
	(*length) += SERVO_DATA_N_BYTES;
	return NOICE;
}

Result_t Servo_Update(Servo_Channel_t *servo)
{

	return OOF_NOT_IMPLEMENTED;
}
