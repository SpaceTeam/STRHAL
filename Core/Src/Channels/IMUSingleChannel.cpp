#include <Channels/IMUSingleChannel.h>

IMUSingleChannel::IMUSingleChannel(uint8_t id, IMUMeasurement measurementType, IMUChannel &imuChannel, uint32_t refreshDivider)
	: AbstractChannel(CHANNEL_TYPE_ADC16, id, refreshDivider), imuChannel(imuChannel), measurementType(measurementType) {
}

int IMUSingleChannel::init() {
	return 0;
}

int IMUSingleChannel::exec() {
	return 0;
}

int IMUSingleChannel::reset() {
	return 0;
}

int IMUSingleChannel::processMessage(uint8_t commandId, uint8_t *returnData, uint8_t &n) {
	switch(commandId) {
		default:
			return AbstractChannel::processMessage(commandId, returnData, n);
	}
}

int IMUSingleChannel::getSensorData(uint8_t *data, uint8_t &n) {
	uint16_t *out = (uint16_t *) (data+n);
	int result = imuChannel.getSensorDataSingle(out, measurementType);
	if(result == 0) {
		n += ADC16_DATA_N_BYTES;
		return 0;
	}
	return -1;
}

int IMUSingleChannel::setVariable(uint8_t variableId, int32_t data) {
	switch(variableId) {
		case ADC16_REFRESH_DIVIDER:
			refreshDivider = data;
			refreshCounter = 0;
			return 0;
		default:
			return -1;
	}
}

int IMUSingleChannel::getVariable(uint8_t variableId, int32_t &data) const {
	switch(variableId) {
		case ADC16_REFRESH_DIVIDER:
			data = (int32_t) refreshDivider;
			return 0;
		case ADC16_MEASUREMENT:
			//data = (int32_t) *adcMeasurement;
			return 0;
		default:
			return -1;
	}
}
