#ifndef IMUSINGLECHANNEL_H
#define IMUSINGLECHANNEL_H

#include "./Channels/AbstractChannel.h"
#include "../Modules/ICM20602_IMU.h"
#include <can_houbolt/channels/adc16_channel_def.h>
#include <STRHAL.h>

class IMUChannel: public AbstractChannel
{
	public:
		IMUChannel(uint8_t id, ICM20602_IMU *imu, IMUMeasurement measurementType, uint32_t refreshDivider);
		IMUChannel(const IMUChannel &other) = delete;
		IMUChannel& operator=(const IMUChannel &other) = delete;
		IMUChannel(const IMUChannel &&other) = delete;
		IMUChannel& operator=(const IMUChannel &&other) = delete;

		int init() override;
		int reset() override;
		int exec() override;
		int getSensorData(uint8_t *data, uint8_t &n) override;

		int processMessage(uint8_t commandId, uint8_t *returnData, uint8_t &n) override;

	protected:

		int setVariable(uint8_t variableId, int32_t data) override;
		int getVariable(uint8_t variableId, int32_t &data) const override;

	private:
		ICM20602_IMU *imu;
		IMUMeasurement measurementType;
		uint64_t timeLastSample = 0;

};

#endif /*IMUSINGLECHANNEL_H*/
