#include "servo_channel.h"
#include "main.h"

#include "pneumatic_valve_channel.h"
#include "channel_util.h"
#include "channels.h"
#include "ui.h"
#include "blmb_settings.h"

#include "serial.h"
#include <string.h>
#include <stdio.h>

#include "foc/tmc4671/TMC4671.h"

Result_t Servo_ResetSettings(Channel_t *channel);
Result_t Servo_SaveSettings(Channel_t *channel);
Result_t Servo_Status(Channel_t *channel);
Result_t Servo_SetVariable(Channel_t *channel, SetMsg_t *set_msg);
Result_t Servo_GetVariable(Channel_t *channel, GetMsg_t *get_msg, PNEUMATIC_VALVE_CMDs response_cmd);

Result_t Servo_InitChannel(Servo_Channel_t *servo) // TODO Servo: maybe remove because unnecessary
{
	if (node.channels[BLMB_SENSOR_CHANNEL].type != CHANNEL_TYPE_ADC16)
		return OOF;

	if (node.channels[BLMB_TORQUE_CHANNEL].type != CHANNEL_TYPE_ADC16)
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
		case SERVO_POSITION_STARTPOINT:
			return &servo->startpoint;
		case SERVO_POSITION_ENDPOINT:
			return &servo->endpoint;
		case SERVO_PWM_ENABLED:
			return &servo->pwm_in_enabled;
		case SERVO_SENSOR_REFRESH_DIVIDER:
			return &servo->refresh_divider;
		case SERVO_PRESSURE_CONTROL_ENABLED:
			return &servo->pressure_control_enabled;
		case SERVO_POS_P_PARAM:
			return &servo->pos_p_param;
		case SERVO_POS_I_PARAM:
			return &servo->pos_i_param;
		case SERVO_VEL_P_PARAM:
			return &servo->vel_p_param;
		case SERVO_VEL_I_PARAM:
			return &servo->vel_i_param;
		case SERVO_TORQ_P_PARAM:
			return &servo->torq_p_param;
		case SERVO_TORQ_I_PARAM:
			return &servo->torq_i_param;
		default:
			return NULL;
	}
}

Result_t Servo_ResetSettings(Channel_t *channel)
{
	Serial_PrintString("ResetSettings");
	BlmbSettings_StoreDefault();
	return ChannelUtil_Ack(channel, SERVO_RES_RESET_SETTINGS);
}
Result_t Servo_SaveSettings(Channel_t *channel) // TODO Servo: not all variables are settings?!
{
	Serial_PrintString("SaveSettings");
	BLMB_Settings_t settings =
	{ 0 };
	Servo_Channel_t *servo = &channel->channel.servo;
	settings.startpoint = servo->startpoint;
	settings.endpoint = servo->endpoint;
	settings.max_accel = servo->max_accel;
	settings.max_speed = servo->max_speed;
	settings.max_torque = servo->max_torque;
	settings.p_param = servo->p_param;
	settings.i_param = servo->i_param;
	settings.d_param = servo->d_param;
	settings.pwm_in_enabled = servo->pwm_in_enabled;
	settings.pressure_control_enabled = servo->pressure_control_enabled;
	settings.pos_p_param = servo->pos_p_param;
	settings.pos_i_param = servo->pos_i_param;
	settings.vel_p_param = servo->vel_p_param;
	settings.vel_i_param = servo->vel_i_param;
	settings.torq_p_param = servo->torq_p_param;
	settings.torq_i_param = servo->torq_i_param;
	BlmbSettings_Store(&settings);
	return ChannelUtil_Ack(channel, SERVO_RES_SAVE_SETTINGS);
}
uint16_t Servo_CalculatePercentagePosition(Servo_Channel_t *servo, uint16_t input)
{
	int32_t position = (int64_t) input * ((int32_t) servo->endpoint - (int32_t) servo->startpoint) / UINT16_MAX;
	position += (int32_t) servo->startpoint;
	position = (position < 0x0000) ? 0x0000 : position;
	position = (position > 0xFFFF) ? 0xFFFF : position;
	return position;
}
Result_t Servo_SetPosition(Servo_Channel_t *servo, uint32_t input)
{
	int32_t position = Servo_CalculatePercentagePosition(servo, input);

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
		Servo_Update(&channel->channel.servo, set_msg->variable_id);
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
		case SERVO_REQ_SAVE_SETTINGS:
			return Servo_SaveSettings(channel);
		default:
			return OOF_UNKNOWN_CMD;
	}
}

Result_t Servo_GetRawData(uint8_t channel_id, uint16_t *data)
{
	Servo_Channel_t *servo = &node.channels[channel_id].channel.servo;
	int64_t value = (int16_t) servo->position << 2;

	value -= (int32_t) servo->startpoint;
	int32_t position = (int64_t) value * UINT16_MAX / ((int32_t) servo->endpoint - (int32_t) servo->startpoint);
	position = (position < 0) ? 0 : position;
	position = (position > UINT16_MAX) ? UINT16_MAX : position;
	if(data != NULL)
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

Result_t Servo_Update(Servo_Channel_t *servo, uint8_t var_id)
{
	switch (var_id)
	{
		case SERVO_MAX_SPEED:
			tmc4671_writeInt(TMC4671_PID_VELOCITY_LIMIT, servo->max_speed);
			break;
		case SERVO_MAX_ACCEL:
			tmc4671_writeInt(TMC4671_PID_ACCELERATION_LIMIT, servo->max_accel);
			break;
		case SERVO_MAX_TORQUE:
			tmc4671_writeInt(TMC4671_PID_TORQUE_FLUX_LIMITS, servo->max_torque);
			break;
		case SERVO_POS_P_PARAM:
			tmc4671_setPositionPI(servo->pos_p_param, servo->pos_i_param);
			break;
		case SERVO_POS_I_PARAM:
			tmc4671_setPositionPI(servo->pos_p_param, servo->pos_i_param);
			break;
		case SERVO_VEL_P_PARAM:
			tmc4671_setVelocityPI(servo->vel_p_param, servo->vel_i_param);
			break;
		case SERVO_VEL_I_PARAM:
			tmc4671_setVelocityPI(servo->vel_p_param, servo->vel_i_param);
			break;
		case SERVO_TORQ_P_PARAM:
			tmc4671_setTorqueFluxPI(servo->torq_p_param, servo->torq_i_param);
			break;
		case SERVO_TORQ_I_PARAM:
			tmc4671_setTorqueFluxPI(servo->torq_p_param, servo->torq_i_param);
			break;
		default:
			break;
	}
	return NOICE;
}
