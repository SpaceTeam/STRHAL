#include "../Inc/Channels/ServoChannel.h"

constexpr ServoRefPos ServoChannel::com0Ref;
constexpr ServoRefPos ServoChannel::pwm0Ref;
constexpr ServoRefPos ServoChannel::adc0Ref;

ServoChannel::ServoChannel(uint8_t channel_id, const LID_TIM_TimerId_t &pwm_timer, const LID_TIM_ChannelId_t &control, const LID_ADC_Channel_t &feedbackChannel, const LID_ADC_Channel_t &currentChannel, const LID_GPIO_t &led_o) :
	AbstractChannel(CHANNEL_TYPE_SERVO, channel_id),
	pwm_tim(pwm_timer), ctrl_chid(control),
	fdbkCh(feedbackChannel), currCh(currentChannel),
	led_o(led_o), servo_state(ServoState::IDLE) {

}

int ServoChannel::init() {
	LID_GPIO_SingleInit(&led_o, LID_GPIO_TYPE_OPP);

	if(LID_TIM_PWM_Init(pwm_tim, PWM_PSC, PWM_RES) < 0)
		return -1;

	if(LID_TIM_PWM_AddChannel(&pwm_ch, ctrl_chid, LID_TIM_PWM_CHANNELTYPE_SO) < 0)
		return -1;

	fdbk_meas = LID_ADC_SubscribeChannel(&fdbkCh, LID_ADC_INTYPE_REGULAR);
	curr_meas = LID_ADC_SubscribeChannel(&currCh, LID_ADC_INTYPE_REGULAR);

	if(fdbk_meas == nullptr || curr_meas == nullptr)
		return -1;

	servo_state = ServoState::READY;
	return 0;
}

int ServoChannel::exec() {
	static uint16_t targ_pos_last = targ_pos;
	static uint16_t fdbk_pos_last = 0;

	static uint8_t targ_hit_cnt = 0;
	static uint64_t t_last_sample;

	uint64_t t = LID_Systick_GetTick();
	if((t - t_last_sample) < EXEC_SAMPLE_TICKS)
		return 0;

	t_last_sample = t;

	fdbk_pos_last = fdbk_pos;
	fdbk_pos = tPosToCanonic(*fdbk_meas, adc_ref);

	if(targ_pos != targ_pos_last) {
		LID_TIM_PWM_SetDuty(&pwm_ch, tPosFromCanonic(targ_pos, pwm_ref));
		LID_TIM_PWM_Enable(&pwm_ch, true);
		targ_pos_last = targ_pos;
		targ_hit_cnt = 0;
		servo_state = ServoState::MOVIN;

		if(reqCalib) {
			servo_state = ServoState::CALIB;
			reqCalib = false;
		}
	}

	switch(servo_state) {
		case ServoState::IDLE:
		case ServoState::READY:
			LID_TIM_PWM_Enable(&pwm_ch, false);
			break;

		case ServoState::MOVIN: //TODO: maybe moving timeout necessary
			if(distPos(targ_pos, fdbk_pos) < POS_DEV) {
				targ_hit_cnt++;
			}

			if(targ_hit_cnt >= TARG_HIT_MIN) {
				servo_state = ServoState::IDLE;
			}
			break;

		case ServoState::CALIB: //TODO: maybe calibration timeout necessary
			if(distPos(fdbk_pos, fdbk_pos_last) < POS_DEV) {
				targ_hit_cnt++;
			} else {
				targ_hit_cnt = 0;
			}

			if(targ_hit_cnt >= TARG_HIT_MIN) {
				if(targ_pos == 0) {
					adc_ref.start = fdbk_pos;
				} else {
					adc_ref.end = fdbk_pos;
				}
				servo_state = ServoState::IDLE;
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
		default:
			return AbstractChannel::prcMsg(cmd_id, ret_data, ret_n);
	}
}

int ServoChannel::getSensorData(uint8_t *data, uint8_t &n) {
	uint16_t *out = (uint16_t *) (data+n);
	*out = getPos();

	n += sizeof(uint16_t);
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
	if(frame.end == frame.start || pos > frame.end)
		return UINT16_MAX;

	else if(frame.end < frame.start || pos < frame.start)
		return 0;

	return (pos-frame.start) * (UINT16_MAX / (frame.end - frame.start));
}

uint16_t ServoChannel::tPosFromCanonic(uint16_t pos, const ServoRefPos &frame) {
	if(frame.end == frame.start)
		return frame.end;

	else if(frame.end < frame.start)
		return 0;

	return pos / (UINT16_MAX / (frame.end - frame.start));
}



uint16_t ServoChannel::distPos(uint16_t pos1, uint16_t pos2) {
	return pos1 < pos2 ? pos2 - pos1 : pos1 - pos2;
}
