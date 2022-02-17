#include <Channels/TempChannel.h>

TempChannel::TempChannel(uint8_t channel_id, const LID_ADC_Channel_t & adc_ch) : AbstractChannel(CHANNEL_TYPE_ADC16, channel_id), temp_ch(adc_ch) {
}

int TempChannel::init() {
	temp_meas = LID_ADC_SubscribeChannel(&temp_ch, LID_ADC_INTYPE_REGULAR);

	if(temp_meas == nullptr)
		return -1;

	return 0;
}

int TempChannel::exec() {
	return 0;
}

int TempChannel::reset() {
	return 0;
}

int TempChannel::prcMsg(uint8_t cmd_id, uint8_t variable_id, uint32_t data, uint32_t &ret) {
	switch(cmd_id) {
		default:
			return AbstractChannel::prcMsg(cmd_id, variable_id, data, ret);
	}
}

int TempChannel::getSensorData(uint8_t *data, uint8_t &n) {
	uint16_t *out = (uint16_t *) (data+n);
	*out = *temp_meas;

	n += sizeof(uint16_t);
	return 0;
}

int TempChannel::setVar(uint8_t variable_id, uint32_t data) {
	switch(variable_id) {
		default:
			return -1;
	}
}

int TempChannel::getVar(uint8_t variable_id, uint32_t &data) const {
	switch(variable_id) {
		default:
			return -1;
	}
}
