#include <Channels/PressureChannel.h>

PressureChannel::PressureChannel(uint8_t channel_id, const LID_ADC_Channel_t & adc_ch) : AbstractChannel(CHANNEL_TYPE_ADC16, channel_id), press_ch(adc_ch) {
}

int PressureChannel::init() {
	press_meas = LID_ADC_SubscribeChannel(&press_ch, LID_ADC_INTYPE_REGULAR);

	if(press_meas == nullptr)
		return -1;

	return 0;
}

int PressureChannel::exec() {
	return 0;
}

int PressureChannel::reset() {
	return 0;
}

int PressureChannel::prcMsg(uint8_t cmd_id, uint8_t variable_id, uint32_t data, uint32_t &ret) {
	switch(cmd_id) {
		default:
			return AbstractChannel::prcMsg(cmd_id, variable_id, data, ret);
	}
}

int PressureChannel::getSensorData(uint8_t *data, uint8_t &n) {
	uint16_t *out = (uint16_t *) (data+n);
	*out = *press_meas;

	n += sizeof(uint16_t);
	return 0;
}

int PressureChannel::setVar(uint8_t variable_id, uint32_t data) {
	switch(variable_id) {
		default:
			return -1;
	}
}

int PressureChannel::getVar(uint8_t variable_id, uint32_t &data) const {
	switch(variable_id) {
		default:
			return -1;
	}
}
