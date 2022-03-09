#include <Channels/ADCChannel.h>

ADCChannel::ADCChannel(uint8_t channel_id, const LID_ADC_Channel_t adc_ch)
	: AbstractChannel(CHANNEL_TYPE_ADC16, channel_id), adc_ch(adc_ch) {
}

int ADCChannel::init() {
	adc_meas = LID_ADC_SubscribeChannel(&adc_ch, LID_ADC_INTYPE_REGULAR);

	if(adc_meas == nullptr)
		return -1;

	return 0;
}

int ADCChannel::exec() {
	return 0;
}

int ADCChannel::reset() {
	return 0;
}

int ADCChannel::prcMsg(uint8_t cmd_id, uint8_t *ret_data, uint8_t &ret_n) {
	switch(cmd_id) {
		default:
			return AbstractChannel::prcMsg(cmd_id, ret_data, ret_n);
	}
}

int ADCChannel::getSensorData(uint8_t *data, uint8_t &n) {
	uint16_t *out = (uint16_t *) (data+n);
	//*out = *adc_meas;
	*out = (uint16_t) LID_Systick_GetTick();
	//*out = 25;

	n += sizeof(uint16_t);
	return 0;
}

int ADCChannel::setVar(uint8_t variable_id, int32_t data) {
	switch(variable_id) {
		default:
			return -1;
	}
}

int ADCChannel::getVar(uint8_t variable_id, int32_t &data) const {
	switch(variable_id) {
		default:
			return -1;
	}
}


uint16_t ADCChannel::getMeas() const {
	return *adc_meas;
}
