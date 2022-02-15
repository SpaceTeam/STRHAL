#include "../Inc/Servo.h"

Servo::Servo(CHANNEL_TYPE t, uint8_t channel_id, const LID_TIM_TimerId_t &pwm_timer, const LID_TIM_ChannelId_t &control, const LID_ADC_Channel_t &feedback, const LID_ADC_Channel_t &current, const LID_GPIO_t &led_out)
	: AbstractChannel(t, channel_id),
	  pwm_tim(pwm_timer), ctrl_chid(control), fdbk_ch(feedback), curr_ch(current), led_o(led_out) {


}
int Servo::init() {
	LID_GPIO_SingleInit(&led_o, LID_GPIO_TYPE_OPP);

	if(LID_TIM_PWM_Init(pwm_tim, SystemCoreClock / SERVO_PWM_FREQ, 100) < 0)
		return -1;

	if(LID_TIM_PWM_AddChannel(&pwm_ch, ctrl_chid, LID_TIM_PWM_CHANNELTYPE_SO) < 0)
		return -1;

	fdbk_meas = LID_ADC_SubscribeChannel(&fdbk_ch, LID_ADC_INTYPE_REGULAR);
	curr_meas = LID_ADC_SubscribeChannel(&curr_ch, LID_ADC_INTYPE_REGULAR);

	if(fdbk_meas == nullptr || curr_meas == nullptr)
		return -1;

	return 0;
}

int Servo::exec() {
	return 0;
}

int Servo::reset() {
	return 0;
}

int Servo::prcMsg(uint8_t cmd_id, uint8_t variable_id, uint32_t data, uint32_t &ret) {
	switch(cmd_id) {
		case SERVO_REQ_MOVE:
			if(move() < 0)
				return -1;

			ret = t_pos;
			return 2;

		default:
			return AbstractChannel::prcMsg(cmd_id, variable_id, data, ret);
	}
}


int Servo::setVar(uint8_t variable_id, uint32_t data) {
	switch(variable_id) {
	case SERVO_TARGET_POSITION:
		if(setTargetPos(data) < 0)
			return -1;

	default:
		return -1;
	}
}

int Servo::getVar(uint8_t variable_id, uint32_t &data) const {
	switch(variable_id) {
	case SERVO_POSITION:
		data = *fdbk_meas;
		return sizeof(uint16_t);

	case SERVO_TARGET_POSITION:
		data = t_pos;
		return sizeof(uint16_t);

	default:
		return -1;
	}
}

int Servo::setTargetPos(uint16_t pos) {
	if(pos > max_pos || pos < min_pos)
		return -1;

	t_pos = pos;
	return pos;
}

uint16_t Servo::getTargetPos() const {
	return t_pos;
}

uint16_t Servo::getPos() const {
	return (uint32_t) *fdbk_meas;
}

int Servo::move() {
	int32_t tmp = LID_TIM_PWM_SetDuty(&pwm_ch, t_pos);
	if(tmp< 0 || (uint16_t) tmp != t_pos)
		return -1;

	return 1;
}
