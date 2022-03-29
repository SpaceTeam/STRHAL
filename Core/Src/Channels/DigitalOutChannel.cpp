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

int DigitalOutChannel::prcMsg(uint8_t cmd_id, uint8_t *ret_data, uint8_t &ret_n) {
	switch(cmd_id) {
		default:
			return AbstractChannel::prcMsg(cmd_id, ret_data, ret_n);
	}
}

int DigitalOutChannel::getSensorData(uint8_t *data, uint8_t &n) {
	uint16_t *out = (uint16_t *) (data+n);
	*out = *adc_meas;

	n += DIGITAL_OUT_DATA_N_BYTES;
	return 0;
}

int DigitalOutChannel::setVar(uint8_t variable_id, int32_t data) {
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

int DigitalOutChannel::getVar(uint8_t variable_id, int32_t &data) const {
	switch(variable_id) {
		case DIGITAL_OUT_STATE:
			data = getState();
			return 0;
		case DIGITAL_OUT_DUTY_CYCLE:
			data = duty_cycle;
			return 0;
		case DIGITAL_OUT_FREQUENCY:
			data = frequency;
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
