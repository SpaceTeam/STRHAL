#ifndef PRESSURECONTROLCHANNEL_H
#define PRESSURECONTROLCHANNEL_H

#include "./Channels/AbstractChannel.h"
#include "./Channels/DigitalOutChannel.h"
#include "./Channels/ADCChannel.h"
#include <can_houbolt/channels/pneumatic_valve_channel_def.h>
#include <STRHAL.h>

class PressureControlChannel : public AbstractChannel {
	public:
		PressureControlChannel(uint8_t id, const ADCChannel &pressureChannel, DigitalOutChannel &solenoidChannel, uint32_t refreshDivider);

		PressureControlChannel(const PressureControlChannel &other) = delete;
		PressureControlChannel& operator=(const PressureControlChannel &other) = delete;
		PressureControlChannel(const PressureControlChannel &&other) = delete;
		PressureControlChannel& operator=(const PressureControlChannel &&other) = delete;

		int init() override;
		int reset() override;
		int exec() override;
		int getSensorData(uint8_t *data, uint8_t &n) override;

		int processMessage(uint8_t commandId, uint8_t *returnData, uint8_t &n) override;

		static constexpr uint16_t EXEC_SAMPLE_TICKS = 100;

	protected:

		int setVariable(uint8_t variableId, int32_t data) override;
		int getVariable(uint8_t variableId, int32_t &data) const override;

	private:
		uint16_t enabled;
		uint16_t position;
		uint16_t targetPosition;
		uint16_t threshold;
		uint16_t hysteresis;
		const ADCChannel &pressureChannel;
		DigitalOutChannel &solenoidChannel;

		uint64_t timeLastSample = 0;

};

#endif /*PRESSURECONTROLCHANNEL_H*/
