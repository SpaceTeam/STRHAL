#ifndef ROCKETCHANNEL_H
#define ROCKETCHANNEL_H

#include "./Channels/AbstractChannel.h"
#include "./Channels/ADCChannel.h"
#include "./Channels/ServoChannel.h"
#include "./Channels/PyroChannel.h"
#include <can_houbolt/channels/rocket_channel_def.h>
#include <STRHAL.h>
#include <STRHAL_UART.h>
#include <cstring>
#include <cstdio>

enum class IgnitionSequence : int
{
	INIT = 0,
	IGNITION0_ON,
	IGNITION1_ON,
	T_0,
	VALVES_SLOWLY_OPEN,
	VALVES_FULLY_OPEN,
	IGNITION_OFF
};

class RocketChannel: public AbstractChannel
{
	public:
		RocketChannel(uint8_t id, const ADCChannel &oxPressureChannel, const ADCChannel &fuelPressureChannel, const ADCChannel &chamberPressureChannel, ServoChannel &oxServoChannel, ServoChannel &fuelServoChannel, PyroChannel &igniter0Channel, PyroChannel &igniter1Channel, uint32_t refreshDivider);

		RocketChannel(const RocketChannel &other) = delete;
		RocketChannel& operator=(const RocketChannel &other) = delete;
		RocketChannel(const RocketChannel &&other) = delete;
		RocketChannel& operator=(const RocketChannel &&other) = delete;

		int init() override;
		int reset() override;
		int exec() override;
		int getSensorData(uint8_t *data, uint8_t &n) override;

		int processMessage(uint8_t commandId, uint8_t *returnData, uint8_t &n) override;

		static constexpr uint16_t EXEC_SAMPLE_TICKS = 1;
		static constexpr uint16_t CHAMBER_PRESSURE_LOW_COUNT_MAX = 100;
		static constexpr uint16_t CHAMBER_PRESSURE_GOOD_COUNT_MIN = 50;
		static constexpr uint16_t AUTO_CHECK_BAD_COUNT_MAX = 10;

	protected:

		int setVariable(uint8_t variableId, int32_t data) override;
		int getVariable(uint8_t variableId, int32_t &data) const override;

	private:
		ROCKET_STATE currentStateLogic(uint64_t time);
		void nextStateLogic(ROCKET_STATE nextState, uint64_t time);

		ROCKET_STATE autoCheck(uint64_t time);
		ROCKET_STATE ignitionSequence(uint64_t time);
		ROCKET_STATE holddown(uint64_t time);
		ROCKET_STATE poweredAscent(uint64_t time);
		ROCKET_STATE depress(uint64_t time);
		ROCKET_STATE abort(uint64_t time);

		void setRocketState(uint8_t *data, uint8_t &n);
		void getRocketState(uint8_t *data, uint8_t &n);

		const ADCChannel &oxPressureChannel;
		const ADCChannel &fuelPressureChannel;
		const ADCChannel &chamberPressureChannel;
		ServoChannel &oxServoChannel;
		ServoChannel &fuelServoChannel;
		PyroChannel &igniter0Channel;
		PyroChannel &igniter1Channel;
		ROCKET_STATE state;
		IgnitionSequence ignitionState;

		uint16_t chamberPressureMin = 0;
		uint16_t chamberPressureLowCounter = 0;
		uint16_t chamberPressureGoodCounter = 0;
		uint16_t autoCheckBadCounter = 0;
		uint16_t fuelPressureMin = 0;
		uint16_t oxPressureMin = 0;
		uint16_t holdDownTimeout = 0;

		uint64_t timeLastSample = 0;
		uint64_t timeLastTransition = 0;
		ROCKET_STATE internalNextState = PAD_IDLE;
		ROCKET_STATE externalNextState = PAD_IDLE;

		Can& can;
};

#endif /*ADCCHANNEL_H*/
