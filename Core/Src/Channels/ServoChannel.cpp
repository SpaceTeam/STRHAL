#include "../Inc/Channels/ServoChannel.h"

ServoChannel::ServoChannel(uint8_t channel_id, const LID_TIM_TimerId_t &pwm_timer, const LID_TIM_ChannelId_t &control, const ADCChannel &feedbackChannel, const ADCChannel &currentChannel, const LID_GPIO_t &led_o) :
	AbstractChannel(CHANNEL_TYPE_SERVO, channel_id),
	pwm_tim(pwm_timer), ctrl_chid(control),
	fdbkCh(feedbackChannel), currCh(currentChannel),
	led_o(led_o) {


}
int ServoChannel::init() {
	LID_GPIO_SingleInit(&led_o, LID_GPIO_TYPE_OPP);

	if(LID_TIM_PWM_Init(pwm_tim, SystemCoreClock / SERVO_PWM_FREQ, 100) < 0)
		return -1;

	if(LID_TIM_PWM_AddChannel(&pwm_ch, ctrl_chid, LID_TIM_PWM_CHANNELTYPE_SO) < 0)
		return -1;

	return 0;
}

int ServoChannel::exec() {
	return 0;
}

int ServoChannel::reset() {
	return 0;
}

int ServoChannel::prcMsg(uint8_t cmd_id, uint8_t variable_id, uint32_t data, uint32_t &ret) {
	switch(cmd_id) {
		case SERVO_REQ_MOVE:
			if(move() < 0)
				return -1;

			ret = t_pos;
			return sizeof(uint16_t);

		default:
			return AbstractChannel::prcMsg(cmd_id, variable_id, data, ret);
	}
}

int ServoChannel::getSensorData(uint8_t *data, uint8_t &n) {
	uint16_t *out = (uint16_t *) (data+n);
	*out = getPos();

	n += sizeof(uint16_t);
	return 0;
}


int ServoChannel::setVar(uint8_t variable_id, uint32_t data) {
	switch(variable_id) {
	case SERVO_TARGET_POSITION:
		if(setTargetPos(data) < 0)
			return -1;

		return 0;

	default:
		return -1;
	}
}

int ServoChannel::getVar(uint8_t variable_id, uint32_t &data) const {
	switch(variable_id) {
	case SERVO_POSITION:
		data = c_pos;
		return sizeof(uint16_t);

	case SERVO_TARGET_POSITION:
		data = t_pos;
		return sizeof(uint16_t);

	default:
		return -1;
	}
}

int ServoChannel::setTargetPos(uint16_t pos) {
	if(pos > end_pos || pos < start_pos)
		return -1;

	t_pos = pos;
	return pos;
}

uint16_t ServoChannel::getTargetPos() const {
	return t_pos;
}

uint16_t ServoChannel::getPos() const {
	return (uint16_t) c_pos;
}

int ServoChannel::move() {
	int32_t tmp = LID_TIM_PWM_SetDuty(&pwm_ch, t_pos);
	if(tmp< 0 || (uint16_t) tmp != t_pos)
		return -1;

	return 1;
}
