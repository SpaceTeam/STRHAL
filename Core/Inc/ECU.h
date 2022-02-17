#ifndef ECU_H
#define ECU_H

#include <Channels/PressureChannel.h>
#include <Channels/TempChannel.h>
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

	private:

};

#endif /*ECU_H*/
