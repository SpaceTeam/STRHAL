#ifndef IMUCHANNEL_H
#define IMUCHANNEL_H

#include "AbstractChannel.h"
#include <W25Qxx_Flash.h>

#include <LID.h>

struct IMUData {
	struct {
		int16_t x,y,z;
	} accel,alpha;
	int16_t temp;
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
		IMUChannel(uint8_t channel_id, const LID_SPI_Id_t &spi_id, const LID_SPI_Config_t &spi_conf);

		IMUChannel(const IMUChannel &other) = delete;
		IMUChannel& operator=(const IMUChannel &other) = delete;
		IMUChannel(const IMUChannel &&other) = delete;
		IMUChannel& operator=(const IMUChannel &&other) = delete;

		~IMUChannel();

		int init() override;
		int reset() override;
		int exec() override;

		int getSensorData(uint8_t *data, uint8_t &n) override;

		bool IsMeasAvailable() const;
		bool getMeas(IMUData &x);

		uint8_t whoAmI() const;

		int prcMsg(uint8_t cmd_id, uint8_t *ret_data, uint8_t &ret_n) override;

		static constexpr uint32_t BUF_DATA_SIZE = 256 / sizeof(IMUData);
		static constexpr uint8_t READ_BIT = 0x80;

		static constexpr uint64_t EXEC_SAMPLE_TICKS = 2;

	protected:
		int setVar(uint8_t variable_id, int32_t data) override;
		int getVar(uint8_t variable_id, int32_t &data) const override;

	private:
		bool readReg(const IMUAddr &addr, uint8_t *reg, uint8_t n = 0);
		bool writeReg(const IMUAddr &addr, uint8_t reg, uint16_t del = 0);
		LID_SPI_Id_t spi_id;
		LID_SPI_Config_t spi_conf;

		IMUData meas_data[BUF_DATA_SIZE] = {0};
		uint32_t meas_data_tail, meas_data_n;
};

#endif /*IMUCHANNEL_H*/
