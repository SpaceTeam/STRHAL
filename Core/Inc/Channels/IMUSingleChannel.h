#ifndef IMUSINGLECHANNEL_H
#define IMUSINGLECHANNEL_H

#include "./Channels/AbstractChannel.h"
#include "./Channels/IMUChannel.h"
#include <can_houbolt/channels/adc16_channel_def.h>
#include <STRHAL.h>

class IMUSingleChannel : public AbstractChannel {
	public:
		IMUSingleChannel(uint8_t id, IMUMeasurement measurementType, IMUChannel &imuChannel, uint32_t refreshDivider);
		IMUSingleChannel(const IMUSingleChannel &other) = delete;
		IMUSingleChannel& operator=(const IMUSingleChannel &other) = delete;
		IMUSingleChannel(const IMUSingleChannel &&other) = delete;
		IMUSingleChannel& operator=(const IMUSingleChannel &&other) = delete;

		int init() override;
		int reset() override;
		int exec() override;
		int getSensorData(uint8_t *data, uint8_t &n) override;

		int processMessage(uint8_t commandId, uint8_t *returnData, uint8_t &n) override;

	protected:

		int setVariable(uint8_t variableId, int32_t data) override;
		int getVariable(uint8_t variableId, int32_t &data) const override;

	private:
		IMUChannel &imuChannel;
		IMUMeasurement measurementType;

};

#endif /*IMUSINGLECHANNEL_H*/
