#ifndef ICM20602_IMU_H
#define ICM20602_IMU_H

#include <STRHAL.h>
#include "./Modules/AbstractModule.h"

struct IMUData
{
		struct
		{
				int16_t x, y, z;
		} accel, alpha;
		int16_t temp;
};

enum class IMUMeasurement : uint8_t
{
	X_ACCEL = 0,
	Y_ACCEL,
	Z_ACCEL,
	X_GYRO,
	Y_GYRO,
	Z_GYRO,
};

enum class IMUAddr : uint8_t
{
	SMPLRT_DIV = 0x19,
	GYRO_CONFIG = 0x1B,
	ACCEL_CONFIG = 0x1C,
	ACCEL_CONFIG_2 = 0x1D,
	USER_CTRL = 0x6A,
	PWR_MGMT_1 = 0x6B,
	PWR_MGMT_2 = 0x6C,
	I2C_IF = 0x70,
	INT_PIN_CFG = 0x37,
	INT_ENABLE = 0x38,
	ACCEL_XOUT_H = 0x3B,
	TEMP_OUT_H = 0x41,
	GYRO_XOUT_H = 0x43,
	WHO_AM_I = 0x75,
};

class ICM20602_IMU: public AbstractModule
{
	public:
		ICM20602_IMU(const STRHAL_SPI_Id_t &spiId, const STRHAL_SPI_Config_t &spiConf, const STRHAL_GPIO_t &dataReadyPin);

		int init() override;
		int exec() override;
		int reset() override;

		bool dataReady();
		int read();
		bool measurementReady();
		void getMeasurement(uint16_t &measurement, IMUMeasurement measurementType);
		uint8_t whoAmI() const;

		static constexpr uint32_t BUF_DATA_SIZE = 16;
		static constexpr uint8_t READ_BIT = 0x80;

		static constexpr uint64_t EXEC_SAMPLE_TICKS = 5;
	private:
		bool readReg(const IMUAddr &address, uint8_t *reg, uint8_t n = 0);
		bool writeReg(const IMUAddr &address, uint8_t reg, uint16_t delay = 0);

		STRHAL_SPI_Id_t spiId;
		STRHAL_SPI_Config_t spiConf;
		const STRHAL_GPIO_t dataReadyPin;

		IMUData measData[BUF_DATA_SIZE] =
		{ 0 };
		uint32_t measDataTail = 0;
		uint32_t measDataNum = 0;

		uint64_t timeLastSample = 0;

};

#endif /*ICM20602_IMU_H*/
