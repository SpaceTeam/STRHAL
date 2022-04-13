#include <Channels/PressureControlChannel.h>

PressureControlChannel::PressureControlChannel(uint8_t id, const ADCChannel &pressureChannel, DigitalOutChannel &solenoidChannel, uint32_t refreshDivider)
	: AbstractChannel(CHANNEL_TYPE_PNEUMATIC_VALVE, id, refreshDivider), pressureChannel(pressureChannel), solenoidChannel(solenoidChannel) {
}

int PressureControlChannel::init() {

	return 0;
}

int PressureControlChannel::exec() {
	uint64_t time = STRHAL_Systick_GetTick();
	if((time - timeLastSample) < EXEC_SAMPLE_TICKS)
		return 0;

	timeLastSample = time;

	uint16_t pressure = pressureChannel.getMeasurement();
	if(enabled == 1) {
		if(pressure > threshold) { // pressure too high
			threshold = targetPosition - hysteresis;
			if(solenoidChannel.getState() != 1) {
				if(solenoidChannel.setState(1) != 0) // if not already open -> open
					return -1;
			}
		} else { // pressure below threshold
			threshold = targetPosition;
			if(solenoidChannel.getState() != 0) {
				if(solenoidChannel.setState(0) != 0) // if not already closed -> close
					return -1;
			}
		}
	}
	return 0;
}

int PressureControlChannel::reset() {
	return 0;
}

int PressureControlChannel::processMessage(uint8_t commandId, uint8_t *returnData, uint8_t &n) {
	switch(commandId) {
		default:
			return AbstractChannel::processMessage(commandId, returnData, n);
	}
}

int PressureControlChannel::getSensorData(uint8_t *data, uint8_t &n) {
	uint16_t *out = (uint16_t *) (data+n);
	*out = (uint16_t) solenoidChannel.getState();

	n += PNEUMATIC_VALVE_DATA_N_BYTES;
	return 0;
}

int PressureControlChannel::setVariable(uint8_t variableId, int32_t data) {
	switch(variableId) {
		case PNEUMATIC_VALVE_ENABLED:
			enabled = data;
			if(data == 0 && solenoidChannel.getState() != 0) {
				if(solenoidChannel.setState(0) != 0) // make sure solenoid is closed after disabling
					return -1;
			}
			return 0;
		case PNEUMATIC_VALVE_POSITION:
			position = data;
			return 0;
		case PNEUMATIC_VALVE_TARGET_POSITION:
			targetPosition = data*4095/UINT16_MAX; // convert from 16 to 12bit scale
			threshold = data*4095/UINT16_MAX;
			return 0;
		case PNEUMATIC_VALVE_THRESHOLD: //cannot set threshold
			return -1;
		case PNEUMATIC_VALVE_HYSTERESIS:
			hysteresis = data*4095/UINT16_MAX;
			return 0;
		case PNEUMATIC_VALVE_POS_REFRESH_DIVIDER:
			refreshDivider = data;
			refreshCounter = 0;
			return 0;
		default:
			return -1;
	}
}

int PressureControlChannel::getVariable(uint8_t variableId, int32_t &data) const {
	switch(variableId) {
		case PNEUMATIC_VALVE_ENABLED:
			data = enabled;
			return 0;
		case PNEUMATIC_VALVE_POSITION:
			data = position;
			return 0;
		case PNEUMATIC_VALVE_TARGET_POSITION:
			data = targetPosition*UINT16_MAX/4095; // convert back to 16bit full scale
			return 0;
		case PNEUMATIC_VALVE_THRESHOLD:
			data = threshold*UINT16_MAX/4095;
			return 0;
		case PNEUMATIC_VALVE_HYSTERESIS:
			data = hysteresis*UINT16_MAX/4095;
			return 0;
		case PNEUMATIC_VALVE_POS_REFRESH_DIVIDER:
			data = (int32_t) refreshDivider;
			return 0;
		default:
			return -1;
	}
}
