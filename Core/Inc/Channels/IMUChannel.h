#ifndef IMUCHANNEL_H
#define IMUCHANNEL_H

#include "AbstractChannel.h"
#include <W25Qxx_Flash.h>
#include <can_houbolt/channels/imu_channel_def.h>
#include <STRHAL.h>

struct IMUData {
	struct {
		int16_t x,y,z;
	} accel,alpha;
	int16_t temp;
};

enum class IMUMeasurement : uint8_t {
	X_ACCEL = 0,
	Y_ACCEL,
	Z_ACCEL,
	X_GYRO,
	Y_GYRO,
	Z_GYRO,
};

enum class IMUAddr : uint8_t {
	GYRO_CONFIG = 0x1B,
	ACCEL_CONFIG = 0x1C,
	ACCEL_CONFIG_2 = 0x1D,
	USER_CTRL = 0x6A,
	PWR_MGMT_1 = 0x6B,
	PWR_MGMT_2 = 0x6C,
	I2C_IF = 0x70,
	ACCEL_XOUT_H = 0x3B,
	TEMP_OUT_H = 0x41,
	GYRO_XOUT_H = 0x43,
	WHO_AM_I = 0x75,
};

class IMUChannel : public AbstractChannel {
	public:
		IMUChannel(uint8_t id, const STRHAL_SPI_Id_t &spiId, const STRHAL_SPI_Config_t &spiConf, uint32_t refreshDivider);

		IMUChannel(const IMUChannel &other) = delete;
		IMUChannel& operator=(const IMUChannel &other) = delete;
		IMUChannel(const IMUChannel &&other) = delete;
		IMUChannel& operator=(const IMUChannel &&other) = delete;

		~IMUChannel();

		int init() override;
		int reset() override;
		int exec() override;

		int getSensorData(uint8_t *data, uint8_t &n) override;
		int getSensorDataSingle(uint16_t * data, IMUMeasurement measurementType);

		bool IsMeasurementAvailable() const;
		bool getMeasurement(IMUData &x);

		uint8_t whoAmI() const;

		int processMessage(uint8_t commandId, uint8_t *returnData, uint8_t &n) override;

		static constexpr uint32_t BUF_DATA_SIZE = 256 / sizeof(IMUData);
		static constexpr uint8_t READ_BIT = 0x80;

		static constexpr uint64_t EXEC_SAMPLE_TICKS = 2;

	protected:
		int setVariable(uint8_t variableId, int32_t data) override;
		int getVariable(uint8_t variableId, int32_t &data) const override;

	private:
		bool readReg(const IMUAddr &address, uint8_t *reg, uint8_t n = 0);
		bool writeReg(const IMUAddr &address, uint8_t reg, uint16_t delay = 0);
		STRHAL_SPI_Id_t spiId;
		STRHAL_SPI_Config_t spiConf;

		IMUData measData[BUF_DATA_SIZE] = {0};
		uint32_t measDataTail, measDataNum;
		uint64_t timeLastSample = 0;
};

#endif /*IMUCHANNEL_H*/
