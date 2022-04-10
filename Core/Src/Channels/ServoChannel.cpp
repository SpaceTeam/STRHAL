#include "../Inc/Channels/ServoChannel.h"

#include <STRHAL.h>
#include <cstring>
#include <cstdio>

constexpr ServoRefPos ServoChannel::com0Ref;
constexpr ServoRefPos ServoChannel::pwm0Ref;
constexpr ServoRefPos ServoChannel::adc0Ref;

ServoChannel::ServoChannel(uint8_t channel_id, uint8_t servo_id, const STRHAL_TIM_TimerId_t &pwm_timer, const STRHAL_TIM_ChanneSTRHAL_t &control, const STRHAL_ADC_Channel_t &feedbackChannel, const STRHAL_ADC_Channel_t &currentChannel, const STRHAL_GPIO_t &led_o, uint32_t refresh_divider) :
	AbstractChannel(CHANNEL_TYPE_SERVO, channel_id, refresh_divider),
	servo_id(servo_id),
	pwm_tim(pwm_timer), ctrl_chid(control),
	fdbkCh(feedbackChannel), currCh(currentChannel),
	led_o(led_o), servo_state(ServoState::IDLE), reqCalib(false) {

}

int ServoChannel::init() {
	STRHAL_GPIO_SingleInit(&led_o, STRHAL_GPIO_TYPE_OPP);

	if(STRHAL_TIM_PWM_Init(pwm_tim, PWM_PSC, PWM_RES) < 0)
		return -1;

	if(STRHAL_TIM_PWM_AddChannel(&pwm_ch, ctrl_chid, STRHAL_TIM_PWM_CHANNELTYPE_SO) < 0)
		return -1;

	fdbk_meas = STRHAL_ADC_SubscribeChannel(&fdbkCh, STRHAL_ADC_INTYPE_REGULAR);
	curr_meas = STRHAL_ADC_SubscribeChannel(&currCh, STRHAL_ADC_INTYPE_REGULAR);

	flash = W25Qxx_Flash::instance(0x1F);

	// Load and assign config
	if(!flash->readConfig())
		return -1;

	// Read config values starting from the servos config register start address
	uint32_t configAddrStart = SERVOCONFIG_OFFSET+servo_id*SERVOCONFIG_N_EACH;
	adc_ref.start = flash->readConfigReg(configAddrStart);
	adc_ref.end = flash->readConfigReg(configAddrStart+1);
	pwm_ref.start = flash->readConfigReg(configAddrStart+2);
	pwm_ref.end = flash->readConfigReg(configAddrStart+3);

	if(fdbk_meas == nullptr || curr_meas == nullptr || flash == nullptr)
		return -1;

	servo_state = ServoState::READY;
	return 0;
}

int ServoChannel::exec() {
	uint64_t t = STRHAL_Systick_GetTick();
	if((t - t_last_sample) < EXEC_SAMPLE_TICKS)
		return 0;

	t_last_sample = t;

	fdbk_pos_last = fdbk_pos;
	fdbk_pos = tPosToCanonic(*fdbk_meas, adc_ref);

	if(targ_pos != targ_pos_last) {
		STRHAL_TIM_PWM_SetDuty(&pwm_ch, tPosFromCanonic(targ_pos, pwm_ref));
		STRHAL_TIM_PWM_Enable(&pwm_ch, true);
		STRHAL_GPIO_Write(&led_o, STRHAL_GPIO_VALUE_H);
		targ_pos_last = targ_pos;
		targ_hit_cnt = 0;
		t_last_cmd = t;
		servo_state = ServoState::MOVIN;

		if(reqCalib) {
			servo_state = ServoState::CALIB;
			//reqCalib = false;
		}
	}

	switch(servo_state) {
		case ServoState::IDLE:
		case ServoState::READY:
			STRHAL_TIM_PWM_Enable(&pwm_ch, false);
			STRHAL_GPIO_Write(&led_o, STRHAL_GPIO_VALUE_L);
			break;

		case ServoState::MOVIN:
			if(distPos(targ_pos, fdbk_pos) < POS_DEV) {
				targ_hit_cnt++;
			}

			if(targ_hit_cnt >= TARG_HIT_MIN || t - t_last_cmd > 800) {
				servo_state = ServoState::IDLE;
			}
			break;

		case ServoState::CALIB: //TODO make config load/save more efficient
			if(distPos(fdbk_pos, fdbk_pos_last) < POS_DEV) {
				targ_hit_cnt++;
			} else {
				targ_hit_cnt = 0;
			}

			if(targ_hit_cnt >= CALIB_HIT_MIN) {
				uint32_t configAddrStart = SERVOCONFIG_OFFSET+servo_id*SERVOCONFIG_N_EACH;

				if(targ_pos == 0) {
					adc_ref.start = *fdbk_meas;
					Config regs[2] = { static_cast<Config>(configAddrStart), static_cast<Config>(configAddrStart+2) };
					uint32_t vals[2] = { (uint32_t) adc_ref.start, (uint32_t) pwm_ref.start };
					flash->writeConfigRegs(regs,vals,2);
				} else {
					adc_ref.end = *fdbk_meas;
					Config regs[2] = { static_cast<Config>(configAddrStart+1), static_cast<Config>(configAddrStart+3) };
					uint32_t vals[2] = { (uint32_t) adc_ref.end, (uint32_t) pwm_ref.end };
					flash->writeConfigRegs(regs,vals,2);
				}
				servo_state = ServoState::IDLE;
				reqCalib = false;
			}
			break;
		default:
			return -1;
	}
	return 0;
}

int ServoChannel::reset() {
	return 0;
}

int ServoChannel::prcMsg(uint8_t cmd_id, uint8_t *ret_data, uint8_t &ret_n) {
	switch(cmd_id) {
		case SERVO_REQ_RESET_SETTINGS: {
			uint32_t vals[4] = { (uint32_t) adc0Ref.start, (uint32_t) adc0Ref.end, (uint32_t) pwm0Ref.start, (uint32_t) pwm0Ref.end };
			flash->writeConfigRegsFromAddr(SERVOCONFIG_OFFSET+servo_id*SERVOCONFIG_N_EACH, vals, 4);
			adc_ref = adc0Ref;
			pwm_ref = pwm0Ref;
			return 0;
		}
		default:
			return AbstractChannel::prcMsg(cmd_id, ret_data, ret_n);
	}
}

int ServoChannel::getSensorData(uint8_t *data, uint8_t &n) {
	uint32_t *out = (uint32_t *) (data+n);
	*out = (uint32_t) getPos();

	n += SERVO_DATA_N_BYTES;
	return 0;
}


int ServoChannel::setVar(uint8_t variable_id, int32_t data) {
	uint16_t pos_data;
	switch(variable_id) {
		case SERVO_TARGET_POSITION:
			targ_pos = (uint16_t) (data & 0xFFFF);
			return 0;

		case SERVO_POSITION_STARTPOINT:
			pos_data = (uint16_t) (data & 0xFFFF);
			pwm_ref.start = tPosFromCanonic(pos_data, pwm0Ref);
			targ_pos = 0;
			reqCalib = true;

			return 0;

		case SERVO_POSITION_ENDPOINT:
			pos_data = (uint16_t) (data & 0xFFFF);
			pwm_ref.end = tPosFromCanonic(pos_data, pwm0Ref);
			targ_pos = UINT16_MAX;
			reqCalib = true;

			return 0;

		case SERVO_POSITION:
			return -2;

		case SERVO_SENSOR_REFRESH_DIVIDER:
			refresh_divider = data;
			refresh_counter = 0;
			return 0;

		default:
			return -1;
	}
}

int ServoChannel::getVar(uint8_t variable_id, int32_t &data) const {
	switch(variable_id) {
	case SERVO_POSITION:
		data = fdbk_pos;
		return 0;

	case SERVO_TARGET_POSITION:
		data = targ_pos;
		return 0;

	case SERVO_POSITION_STARTPOINT:
		data = tPosToCanonic(pwm_ref.start, pwm0Ref);
		return 0;

	case SERVO_POSITION_ENDPOINT:
		data = tPosToCanonic(pwm_ref.end, pwm0Ref);
		return 0;

	case SERVO_SENSOR_REFRESH_DIVIDER:
		data = (int32_t) refresh_divider;
		return 0;

	default:
		return -1;
	}
}

void ServoChannel::setTargetPos(uint16_t pos) {
	targ_pos = pos;
}

uint16_t ServoChannel::getTargetPos() const {
	return targ_pos;
}

uint16_t ServoChannel::getPos() const {
	return tPosToCanonic(*fdbk_meas, adc_ref);
}

uint16_t ServoChannel::getFeedbackMeas () const {
	return *fdbk_meas;
}

uint16_t ServoChannel::getCurrentMeas () const {
	return *curr_meas;
}

uint16_t ServoChannel::tPosToCanonic(uint16_t pos, const ServoRefPos &frame) {
	if(frame.end == frame.start) {
		return UINT16_MAX;
	} else if(frame.end < frame.start) { // reversed servo
		// check if out of bounds
		if(pos <= frame.end) {
			return UINT16_MAX;
		} else if (pos >= frame.start) {
			return 0;
		}
		return UINT16_MAX -((pos-frame.end) * (UINT16_MAX / (frame.start - frame.end)));
	}

	// check if out of bounds
	if(pos <= frame.start) {
		return 0;
	} else if (pos >= frame.end) {
		return UINT16_MAX;
	}

	return (pos-frame.start) * (UINT16_MAX / (frame.end - frame.start));
}

uint16_t ServoChannel::tPosFromCanonic(uint16_t pos, const ServoRefPos &frame) {
	if(frame.end == frame.start) {
		return frame.end;
	} else if(frame.end < frame.start) { // reversed servo
		uint16_t reversed_pos = UINT16_MAX - pos;
		return (reversed_pos / (UINT16_MAX / (frame.start - frame.end))) + frame.end;
	}

	return (pos / (UINT16_MAX / (frame.end - frame.start))) + frame.start;
}



uint16_t ServoChannel::distPos(uint16_t pos1, uint16_t pos2) {
	return pos1 < pos2 ? pos2 - pos1 : pos1 - pos2;
}
