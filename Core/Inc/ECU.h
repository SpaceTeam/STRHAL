#ifndef ECU_H
#define ECU_H

#include <Channels/ADCChannel.h>
#include <Channels/ServoChannel.h>
#include <Channels/GenericChannel.h>
#include "CANCOM.h"
#include <LID.h>

class ECU : public GenericChannel {
	public:
		ECU(uint32_t node_id, uint32_t fw_version);
		ECU(const ECU &other) = delete;
		ECU& operator=(const ECU &other) = delete;

		int init() override;
		//int reset() override;
		int exec() override;

		static constexpr uint8_t ECU_CHANNEL_N = 9;

	private:
		CANCOM *cancom;
		COMState CANState;

		ADCChannel press0Ch, press1Ch, press2Ch, press3Ch, press4Ch, press5Ch;
		ADCChannel temp0Ch, temp1Ch, temp2Ch;
};

#endif /*ECU_H*/
