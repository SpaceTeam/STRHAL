#ifndef PRESSURECONTROLCHANNEL_H
#define PRESSURECONTROLCHANNEL_H

#include "./Channels/AbstractChannel.h"
#include "./Channels/DigitalOutChannel.h"
#include "./Channels/ADCChannel.h"
#include <can_houbolt/channels/pneumatic_valve_channel_def.h>
#include <LID.h>

class PressureControlChannel : public AbstractChannel {
	public:
		PressureControlChannel(uint8_t channel_id, const ADCChannel &press_ch, DigitalOutChannel &solenoid_ch, uint32_t refresh_divider);

		PressureControlChannel(const PressureControlChannel &other) = delete;
		PressureControlChannel& operator=(const PressureControlChannel &other) = delete;
		PressureControlChannel(const PressureControlChannel &&other) = delete;
		PressureControlChannel& operator=(const PressureControlChannel &&other) = delete;

		int init() override;
		int reset() override;
		int exec() override;
		int getSensorData(uint8_t *data, uint8_t &n) override;

		int prcMsg(uint8_t cmd_id, uint8_t *ret_data, uint8_t &ret_n) override;

		static constexpr uint16_t EXEC_SAMPLE_TICKS = 100;

	protected:

		int setVar(uint8_t variable_id, int32_t data) override;
		int getVar(uint8_t variable_id, int32_t &data) const override;

	private:
		uint16_t enabled;
		uint16_t position;
		uint16_t target_position;
		uint16_t threshold;
		uint16_t hysteresis;
		const ADCChannel &press_ch;
		DigitalOutChannel &solenoid_ch;

};

#endif /*PRESSURECONTROLCHANNEL_H*/
