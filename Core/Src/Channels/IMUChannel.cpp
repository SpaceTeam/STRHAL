#include "../Inc/Channels/IMUChannel.h"

#include <cstring>
#include <cstdio>

IMUChannel::IMUChannel(uint8_t channel_id, const LID_SPI_Id_t &spi_id, const LID_SPI_Config_t &spi_conf, uint32_t refresh_divider) :
	AbstractChannel(CHANNEL_TYPE_UNKNOWN, channel_id, refresh_divider),
	spi_id(spi_id),
	spi_conf(spi_conf) {
	meas_data_tail = meas_data_n = 0;
}

int IMUChannel::init() {
	if(LID_SPI_Master_Init(spi_id, &spi_conf) < 0)
		return -1;

	LID_SPI_Master_Run(spi_id);

	if(!writeReg(IMUAddr::PWR_MGMT_1, 0x80, 1000)
			 ||	!writeReg(IMUAddr::PWR_MGMT_1, 0x01, 50)
			 || !writeReg(IMUAddr::PWR_MGMT_2, 0x3F, 50)
			 || !writeReg(IMUAddr::USER_CTRL, 0x00, 50)
			 || !writeReg(IMUAddr::I2C_IF, 0x40, 50)
			 || !writeReg(IMUAddr::GYRO_CONFIG, 0x19, 50)
			 || !writeReg(IMUAddr::ACCEL_CONFIG, 0x18, 50)
			 || !writeReg(IMUAddr::ACCEL_CONFIG_2, 0x04, 50)
			 || !writeReg(IMUAddr::PWR_MGMT_2, 0x00, 50)
			) {
		return -1;
	}

	if(whoAmI() != 0x12)
		return -1;

	return 0;
}

int IMUChannel::exec() {
	static uint64_t t_last_sample = LID_Systick_GetTick();

	uint64_t t = LID_Systick_GetTick();
	if((t - t_last_sample) < EXEC_SAMPLE_TICKS)
		return 0;

	t_last_sample = t;

	uint8_t i = (meas_data_tail + meas_data_n) % BUF_DATA_SIZE;

	uint8_t tmp[6];

	if(!readReg(IMUAddr::ACCEL_XOUT_H, &tmp[0], 6))
		return -1;

	meas_data[i].accel.x = tmp[0] << 8 | tmp[1];
	meas_data[i].accel.y = tmp[2] << 8 | tmp[3];
	meas_data[i].accel.z = tmp[4] << 8 | tmp[5];

	if(!readReg(IMUAddr::TEMP_OUT_H, &tmp[0], 2))
		return -1;

	meas_data[i].temp = tmp[0] << 8 | tmp[1];

	if(!readReg(IMUAddr::GYRO_XOUT_H, &tmp[0], 6))
		return -1;

	meas_data[i].alpha.x = tmp[0] << 8 | tmp[1];
	meas_data[i].alpha.y = tmp[2] << 8 | tmp[3];
	meas_data[i].alpha.z = tmp[4] << 8 | tmp[5];

	/*if(i == BUF_DATA_SIZE-1) {
		meas_data_n = 0;
		(void) flash.writeEnable();
		(void) flash.writeCurrentPage((uint8_t *)meas_data, BUF_DATA_SIZE*sizeof(IMUData));
	} else {
		meas_data_n++;
	}*/
	meas_data_n++;
	meas_data_n %= BUF_DATA_SIZE;

	return 0;
}

uint8_t IMUChannel::whoAmI() const {
	uint8_t cmd = static_cast<uint8_t>(IMUAddr::WHO_AM_I) | READ_BIT;

	uint8_t imu_id;
	LID_SPI_Master_Transceive(spi_id, &cmd, 1, 1, &imu_id, 1, 100);

	return imu_id;
}

int IMUChannel::reset() {
	meas_data_tail = meas_data_n = 0;

	return 0;
}

bool IMUChannel::IsMeasAvailable() const {
	return meas_data_n > 0;
}

bool IMUChannel::getMeas(IMUData &x) {
	if(meas_data_n == 0)
		return false;

	x = meas_data[meas_data_tail++];
	meas_data_tail %= BUF_DATA_SIZE;
	meas_data_n--;
	return true;
}

int IMUChannel::getSensorData(uint8_t *data, uint8_t &n) {
	if(meas_data_n > 0) {
		std::memcpy(data, &meas_data[meas_data_tail++], 12);
		meas_data_tail %= BUF_DATA_SIZE;
		meas_data_n--;
	}

	return 0;
}

int IMUChannel::prcMsg(uint8_t cmd_id, uint8_t *ret_data, uint8_t &ret_n) {
	return AbstractChannel::prcMsg(cmd_id, ret_data, ret_n);
}

int IMUChannel::setVar(uint8_t variable_id, int32_t data) {
	return -1;
}

int IMUChannel::getVar(uint8_t variable_id, int32_t &data) const {
	return -1;
}

bool IMUChannel::writeReg(const IMUAddr &addr, uint8_t reg, uint16_t del) {
	uint8_t cmd[2];

	cmd[0] = static_cast<uint8_t>(addr);
	cmd[1] = reg;

	if(LID_SPI_Master_Transceive(spi_id, cmd, 2, 2, nullptr, 0, 100) != 0)
		return false;

	LL_mDelay(del);
	return true;
}

bool IMUChannel::readReg(const IMUAddr &addr, uint8_t *reg, uint8_t n) {
	uint8_t cmd;
	cmd = READ_BIT | static_cast<uint8_t>(addr);

	return LID_SPI_Master_Transceive(spi_id, &cmd, 1, 1, reg, n, 100) == ((int32_t) n);
}

IMUChannel::~IMUChannel() {

}
