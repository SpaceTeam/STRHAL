#include <Channels/DigitalOutChannel.h>

DigitalOutChannel::DigitalOutChannel(uint8_t channel_id, const LID_ADC_Channel_t & adc_ch, const LID_GPIO_t &cntrl_pin)
	: AbstractChannel(CHANNEL_TYPE_DIGITAL_OUT, channel_id), adc_ch(adc_ch), cntrl_pin(cntrl_pin) {
}

int DigitalOutChannel::init() {
	adc_meas = LID_ADC_SubscribeChannel(&adc_ch, LID_ADC_INTYPE_OPAMP);
	LID_GPIO_SingleInit(&cntrl_pin,LID_GPIO_TYPE_OPP);

	if(adc_meas == nullptr)
		return -1;

	return 0;
}

int DigitalOutChannel::exec() {
	return 0;
}

int DigitalOutChannel::reset() {
	return 0;
}

int DigitalOutChannel::prcMsg(uint8_t cmd_id, uint8_t variable_id, uint32_t data, uint8_t *ret_data, uint8_t &ret_n) {
	switch(cmd_id) {
		default:
			return AbstractChannel::prcMsg(cmd_id, variable_id, data, ret_data, ret_n);
	}
}

int DigitalOutChannel::getSensorData(uint8_t *data, uint8_t &n) {
	uint16_t *out = (uint16_t *) (data+n);
	*out = *adc_meas;

	n += sizeof(uint16_t);
	return 0;
}

int DigitalOutChannel::setVar(uint8_t variable_id, uint32_t data) {
	switch(variable_id) {
		case DIGITAL_OUT_STATE:
			if(setState(data) != 0)
				return -1;
			return 0;
		case DIGITAL_OUT_DUTY_CYCLE:
			duty_cycle = data;
			return 0;
		case DIGITAL_OUT_FREQUENCY:
			frequency = data;
			return 0;
		default:
			return -1;
	}
}

int DigitalOutChannel::getVar(uint8_t variable_id, uint8_t *data) const {
	SetMsg_t * set_msg = (SetMsg_t *) data;
	set_msg->variable_id = variable_id;
	switch(variable_id) {
		case DIGITAL_OUT_STATE:
			set_msg->value = getState();
			return 0;
		case DIGITAL_OUT_DUTY_CYCLE:
			set_msg->value = duty_cycle;
			return 0;
		case DIGITAL_OUT_FREQUENCY:
			set_msg->value = frequency;
			return 0;
		default:
			return -1;
	}
}

uint32_t DigitalOutChannel::getState() const {
	return (LID_GPIO_ReadOutput(&cntrl_pin) == LID_GPIO_VALUE_L) ? 0UL : 1UL;
}

int DigitalOutChannel::setState(uint32_t state) {
	if (state == 0) {
		LID_GPIO_Write(&cntrl_pin, LID_GPIO_VALUE_L);
	} else {
		LID_GPIO_Write(&cntrl_pin, LID_GPIO_VALUE_H);
	}
	return 0;
}


uint16_t DigitalOutChannel::getMeas() const {
	return *adc_meas;
}
