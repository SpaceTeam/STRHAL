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

int Servo::prcMsg(uint8_t cmd_id, uint8_t variable_id, uint32_t data, uint8_t *ret) {
	switch(cmd_id) {
		case SERVO_REQ_MOVE:
			if(move() < 0)
				return -1;

			ret[0] = (uint8_t) t_pos & 0xFF;
			ret[1] = (uint8_t) (t_pos >> 8) & 0xFF;
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

int Servo::getVar(uint8_t variable_id, uint8_t *data, uint8_t n) const {
	uint32_t tmp;
	switch(variable_id) {
	case SERVO_POSITION:
		tmp = *fdbk_meas;
		data[0] = (uint8_t) tmp & 0xFF;
		data[1] = (uint8_t) (tmp >> 8) & 0xFF;
		return 2;

	case SERVO_TARGET_POSITION:
		data[0] = (uint8_t) t_pos & 0xFF;
		data[1] = (uint8_t) (t_pos >> 8) & 0xFF;
		return 2;

	default:
		return -1;
	}
}

int Servo::setTargetPos(uint32_t pos) {
	if(pos > max_pos || pos < min_pos)
		return -1;

	t_pos = pos;
	return pos;
}

uint32_t Servo::getTargetPos() const {
	return t_pos;
}

uint32_t Servo::getCurrentPos() const {
	return (uint32_t) *fdbk_meas;
}

int Servo::move() {
	if(LID_TIM_PWM_SetDuty(&pwm_ch, t_pos) != t_pos)
		return -1;

	return 1;
}
