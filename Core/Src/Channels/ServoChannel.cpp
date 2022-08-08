#include "../Inc/Channels/ServoChannel.h"

#include <STRHAL.h>
#include <cstring>
#include <cstdio>

constexpr ServoRefPos ServoChannel::com0Ref;
constexpr ServoRefPos ServoChannel::pwm0Ref;
constexpr ServoRefPos ServoChannel::adc0Ref;

ServoChannel::ServoChannel(uint8_t id, uint8_t servoId, const STRHAL_TIM_TimerId_t &pwmTimer, const STRHAL_TIM_ChannelId_t &control, const STRHAL_ADC_Channel_t &feedbackChannel, const STRHAL_ADC_Channel_t &currentChannel, const STRHAL_GPIO_t &led, uint32_t refreshDivider) :
		AbstractChannel(CHANNEL_TYPE_SERVO, id, refreshDivider), servoId(servoId), pwmTimer(pwmTimer), ctrlChannelId(control), feedbackChannel(feedbackChannel), currentChannel(currentChannel), led(led), servoState(ServoState::IDLE), reqCalib(false)
{

}

int ServoChannel::init()
{
	STRHAL_GPIO_SingleInit(&led, STRHAL_GPIO_TYPE_OPP);

	if (STRHAL_TIM_PWM_Init(pwmTimer, PWM_PSC, PWM_RES) < 0)
		return -1;

	if (STRHAL_TIM_PWM_AddChannel(&pwmChannel, ctrlChannelId, STRHAL_TIM_PWM_CHANNELTYPE_SO) < 0)
		return -1;

	feedbackMeasurement = STRHAL_ADC_SubscribeChannel(&feedbackChannel, STRHAL_ADC_INTYPE_REGULAR);
	currentMeasurement = STRHAL_ADC_SubscribeChannel(&currentChannel, STRHAL_ADC_INTYPE_REGULAR);

	flash = W25Qxx_Flash::instance(0x1F);

	// Load and assign config
	if (!flash->readConfig())
		return -1;

	// Read config values starting from the servos config register start address
	uint32_t configAddrStart = SERVOCONFIG_OFFSET + servoId * SERVOCONFIG_N_EACH;
	adcRef.start = flash->readConfigReg(configAddrStart);
	adcRef.end = flash->readConfigReg(configAddrStart + 1);
	pwmRef.start = flash->readConfigReg(configAddrStart + 2);
	pwmRef.end = flash->readConfigReg(configAddrStart + 3);

	if (feedbackMeasurement == nullptr || currentMeasurement == nullptr || flash == nullptr)
		return -1;

	servoState = ServoState::READY;
	return 0;
}

int ServoChannel::exec()
{
	uint64_t time = STRHAL_Systick_GetTick();
	if ((time - timeLastSample) < EXEC_SAMPLE_TICKS)
		return 0;

	timeLastSample = time;

	feedbackPositionLast = feedbackPosition;
	feedbackPosition = tPosToCanonic(*feedbackMeasurement, adcRef);
	if (step != 0)
	{
		if (finalPosition != targetPosition)
		{
			targetPosition += step;
			if (step > 0)
				targetPosition = (targetPosition > finalPosition) ? finalPosition : targetPosition;
			else
				targetPosition = (targetPosition < finalPosition) ? finalPosition : targetPosition;
		}
		else
		{
			step = 0;
		}
	}
	if (targetPosition != targetPositionLast)
	{

		STRHAL_TIM_PWM_SetDuty(&pwmChannel, tPosFromCanonic(targetPosition, pwmRef));
		STRHAL_TIM_PWM_Enable(&pwmChannel, true);
		STRHAL_GPIO_Write(&led, STRHAL_GPIO_VALUE_H);
		targetPositionLast = targetPosition;
		targetHitCount = 0;
		timeLastCommand = time;
		servoState = ServoState::MOVIN;

		if (reqCalib)
		{
			servoState = ServoState::CALIB;
			//reqCalib = false;
		}
	}

	switch (servoState)
	{
		case ServoState::IDLE:
		case ServoState::READY:
			STRHAL_TIM_PWM_Enable(&pwmChannel, false);
			STRHAL_GPIO_Write(&led, STRHAL_GPIO_VALUE_L);
			break;

		case ServoState::MOVIN:
			if (distPos(targetPosition, feedbackPosition) < POS_DEV)
			{
				targetHitCount++;
			}

			if (targetHitCount >= TARG_HIT_MIN || time - timeLastCommand > 800)
			{
				servoState = ServoState::IDLE;
			}
			break;

		case ServoState::CALIB: //TODO make config load/save more efficient
			if (distPos(feedbackPosition, feedbackPositionLast) < POS_DEV)
			{
				targetHitCount++;
			}
			else
			{
				targetHitCount = 0;
			}

			if (targetHitCount >= CALIB_HIT_MIN)
			{
				uint32_t configAddrStart = SERVOCONFIG_OFFSET + servoId * SERVOCONFIG_N_EACH;

				if (targetPosition == 0)
				{
					adcRef.start = *feedbackMeasurement;
					Config regs[2] =
					{ static_cast<Config>(configAddrStart), static_cast<Config>(configAddrStart + 2) };
					uint32_t vals[2] =
					{ (uint32_t) adcRef.start, (uint32_t) pwmRef.start };
					flash->writeConfigRegs(regs, vals, 2);
				}
				else
				{
					adcRef.end = *feedbackMeasurement;
					Config regs[2] =
					{ static_cast<Config>(configAddrStart + 1), static_cast<Config>(configAddrStart + 3) };
					uint32_t vals[2] =
					{ (uint32_t) adcRef.end, (uint32_t) pwmRef.end };
					flash->writeConfigRegs(regs, vals, 2);
				}
				servoState = ServoState::IDLE;
				reqCalib = false;
			}
			break;
		default:
			return -1;
	}
	return 0;
}

int ServoChannel::reset()
{
	return 0;
}

int ServoChannel::processMessage(uint8_t cmd_id, uint8_t *ret_data, uint8_t &ret_n)
{
	switch (cmd_id)
	{
		case SERVO_REQ_RESET_SETTINGS:
		{
			uint32_t vals[4] =
			{ (uint32_t) adc0Ref.start, (uint32_t) adc0Ref.end, (uint32_t) pwm0Ref.start, (uint32_t) pwm0Ref.end };
			flash->writeConfigRegsFromAddr(SERVOCONFIG_OFFSET + servoId * SERVOCONFIG_N_EACH, vals, 4);
			adcRef = adc0Ref;
			pwmRef = pwm0Ref;
			return 0;
		}
		default:
			return AbstractChannel::processMessage(cmd_id, ret_data, ret_n);
	}
}

int ServoChannel::getSensorData(uint8_t *data, uint8_t &n)
{
	uint32_t *out = (uint32_t*) (data + n);
	*out = (uint32_t) getPos();

	n += SERVO_DATA_N_BYTES;
	return 0;
}

int ServoChannel::setVariable(uint8_t variable_id, int32_t data)
{
	uint16_t pos_data;
	switch (variable_id)
	{
		case SERVO_TARGET_POSITION:
			targetPosition = (uint16_t) (data & 0xFFFF);
			return 0;

		case SERVO_POSITION_STARTPOINT:
			pos_data = (uint16_t) (data & 0xFFFF);
			pwmRef.start = tPosFromCanonic(pos_data, pwm0Ref);
			targetPosition = 0;
			reqCalib = true;

			return 0;

		case SERVO_POSITION_ENDPOINT:
			pos_data = (uint16_t) (data & 0xFFFF);
			pwmRef.end = tPosFromCanonic(pos_data, pwm0Ref);
			targetPosition = UINT16_MAX;
			reqCalib = true;

			return 0;

		case SERVO_POSITION:
			return -2;

		case SERVO_POSITION_RAW:
			return -2;

		case SERVO_SENSOR_REFRESH_DIVIDER:
			refreshDivider = data;
			refreshCounter = 0;
			return 0;

		default:
			return -1;
	}
}

int ServoChannel::getVariable(uint8_t variable_id, int32_t &data) const
{
	switch (variable_id)
	{
		case SERVO_POSITION:
			data = feedbackPosition;
			return 0;

		case SERVO_POSITION_RAW:
			data = *feedbackMeasurement << 4;
			return 0;

		case SERVO_TARGET_POSITION:
			data = targetPosition;
			return 0;

		case SERVO_POSITION_STARTPOINT:
			data = tPosToCanonic(pwmRef.start, pwm0Ref);
			return 0;

		case SERVO_POSITION_ENDPOINT:
			data = tPosToCanonic(pwmRef.end, pwm0Ref);
			return 0;

		case SERVO_SENSOR_REFRESH_DIVIDER:
			data = (int32_t) refreshDivider;
			return 0;

		default:
			return -1;
	}
}

void ServoChannel::setTargetPos(uint16_t pos)
{
	targetPosition = pos;
}

void ServoChannel::moveToPosInInterval(uint16_t position, uint16_t interval)
{
	step = (position - targetPosition) / interval * EXEC_SAMPLE_TICKS;
	finalPosition = position;
}

uint16_t ServoChannel::getTargetPos() const
{
	return targetPosition;
}

uint16_t ServoChannel::getPos() const
{
	return tPosToCanonic(*feedbackMeasurement, adcRef);
}

uint16_t ServoChannel::getFeedbackMeasurement() const
{
	return *feedbackMeasurement;
}

uint16_t ServoChannel::getCurrentMeasurement() const
{
	return *currentMeasurement;
}

uint16_t ServoChannel::tPosToCanonic(uint16_t pos, const ServoRefPos &frame)
{
	if (frame.end == frame.start)
	{
		return UINT16_MAX;
	}
	else if (frame.end < frame.start)
	{ // reversed servo
		// check if out of bounds
		if (pos <= frame.end)
		{
			return UINT16_MAX;
		}
		else if (pos >= frame.start)
		{
			return 0;
		}
		return UINT16_MAX - ((pos - frame.end) * (UINT16_MAX / (frame.start - frame.end)));
	}

	// check if out of bounds
	if (pos <= frame.start)
	{
		return 0;
	}
	else if (pos >= frame.end)
	{
		return UINT16_MAX;
	}

	return (pos - frame.start) * (UINT16_MAX / (frame.end - frame.start));
}

uint16_t ServoChannel::tPosFromCanonic(uint16_t pos, const ServoRefPos &frame)
{
	if (frame.end == frame.start)
	{
		return frame.end;
	}
	else if (frame.end < frame.start)
	{ // reversed servo
		uint16_t reversedPosition = UINT16_MAX - pos;
		return (reversedPosition / (UINT16_MAX / (frame.start - frame.end))) + frame.end;
	}

	return (pos / (UINT16_MAX / (frame.end - frame.start))) + frame.start;
}

uint16_t ServoChannel::distPos(uint16_t pos1, uint16_t pos2)
{
	return pos1 < pos2 ? pos2 - pos1 : pos1 - pos2;
}
