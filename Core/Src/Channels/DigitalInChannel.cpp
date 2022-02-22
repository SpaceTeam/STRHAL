#include <Channels/DigitalInChannel.h>

DigitalInChannel::DigitalInChannel(uint8_t channel_id, const LID_GPIO_t &digin_pin)
	: AbstractChannel(CHANNEL_TYPE_ADC16, channel_id), digin_pin(digin_pin) {
}

int DigitalInChannel::init() {
	LID_GPIO_SingleInit(&digin_pin,LID_GPIO_TYPE_IHZ);

	return 0;
}

int DigitalInChannel::exec() {
	return 0;
}

int DigitalInChannel::reset() {
	return 0;
}

int DigitalInChannel::prcMsg(uint8_t cmd_id, uint8_t *ret_data, uint8_t &ret_n) {
	switch(cmd_id) {
		default:
			return AbstractChannel::prcMsg(cmd_id, ret_data, ret_n);
	}
}

int DigitalInChannel::getSensorData(uint8_t *data, uint8_t &n) {
	uint16_t *out = (uint16_t *) (data+n);
	*out = (LID_GPIO_Read(&digin_pin) == LID_GPIO_VALUE_L) ? 0UL : 1UL;

	n += sizeof(uint16_t);
	return 0;
}

int DigitalInChannel::setVar(uint8_t variable_id, int32_t data) {
	switch(variable_id) {
		default:
			return -1;
	}
}

int DigitalInChannel::getVar(uint8_t variable_id, int32_t *data) const {
	switch(variable_id) {
		default:
			return -1;
	}
}

uint16_t DigitalInChannel::getMeas() const {
	return (LID_GPIO_Read(&digin_pin) == LID_GPIO_VALUE_L) ? 0UL : 1UL;;
}
