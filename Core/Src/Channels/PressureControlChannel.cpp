#include <Channels/PressureControlChannel.h>

PressureControlChannel::PressureControlChannel(uint8_t channel_id, const ADCChannel &press_ch, DigitalOutChannel &solenoid_ch)
	: AbstractChannel(CHANNEL_TYPE_PNEUMATIC_VALVE, channel_id), press_ch(press_ch), solenoid_ch(solenoid_ch) {
}

int PressureControlChannel::init() {

	return 0;
}

int PressureControlChannel::exec() {
	static uint64_t t_last_sample;

	uint64_t t = LID_Systick_GetTick();
	if((t - t_last_sample) < EXEC_SAMPLE_TICKS)
		return 0;

	t_last_sample = t;

	uint16_t pressure = press_ch.getMeas();
	if (enabled == 1U && pressure > threshold) { // pressure too high and control is enabled
		threshold = target_position - hysteresis;
		if(solenoid_ch.getState() != 1) {
			if(solenoid_ch.setState(1) != 0) // if not already open -> open
				return -1;
		}
	} else { // pressure below threshold or control disabled
		threshold = target_position;
		if(solenoid_ch.getState() != 0) {
			if(solenoid_ch.setState(0) != 0) // if not already closed -> close
				return -1;
		}
	}
	return 0;
}

int PressureControlChannel::reset() {
	return 0;
}

int PressureControlChannel::prcMsg(uint8_t cmd_id, uint8_t *ret_data, uint8_t &ret_n) {
	switch(cmd_id) {
		default:
			return AbstractChannel::prcMsg(cmd_id, ret_data, ret_n);
	}
}

int PressureControlChannel::getSensorData(uint8_t *data, uint8_t &n) {
	uint16_t *out = (uint16_t *) (data+n);
	*out = (uint16_t) solenoid_ch.getState();

	n += PNEUMATIC_VALVE_DATA_N_BYTES;
	return 0;
}

int PressureControlChannel::setVar(uint8_t variable_id, int32_t data) {
	switch(variable_id) {
		case PNEUMATIC_VALVE_ENABLED:
			enabled = data;
			return 0;
		case PNEUMATIC_VALVE_POSITION:
			position = data;
			return 0;
		case PNEUMATIC_VALVE_TARGET_POSITION:
			target_position = data;
			threshold = data;
			return 0;
		case PNEUMATIC_VALVE_THRESHOLD: //cannot set threshold
			return -1;
		case PNEUMATIC_VALVE_HYSTERESIS:
			hysteresis = data;
			return 0;
		default:
			return -1;
	}
}

int PressureControlChannel::getVar(uint8_t variable_id, int32_t &data) const {
	switch(variable_id) {
		case PNEUMATIC_VALVE_ENABLED:
			data = enabled;
			return 0;
		case PNEUMATIC_VALVE_POSITION:
			data = position;
			return 0;
		case PNEUMATIC_VALVE_TARGET_POSITION:
			data = target_position;
			return 0;
		case PNEUMATIC_VALVE_THRESHOLD:
			data = threshold;
			return 0;
		case PNEUMATIC_VALVE_HYSTERESIS:
			data = hysteresis;
			return 0;
		default:
			return -1;
	}
}
