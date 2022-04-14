#include "../Inc/Channels/IMUChannel.h"

#include <cstring>
#include <cstdio>

IMUChannel::IMUChannel(uint8_t id, const STRHAL_SPI_Id_t &spiId, const STRHAL_SPI_Config_t &spiConf, uint32_t refreshDivider) :
	AbstractChannel(CHANNEL_TYPE_UNKNOWN, id, refreshDivider),
	spiId(spiId),
	spiConf(spiConf) {
	measDataTail = measDataNum = 0;
}

int IMUChannel::init() {
	if(STRHAL_SPI_Master_Init(spiId, &spiConf) < 0)
		return -1;

	STRHAL_SPI_Master_Run(spiId);

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
	uint64_t time = STRHAL_Systick_GetTick();
	if((time - timeLastSample) < EXEC_SAMPLE_TICKS)
		return 0;

	timeLastSample = time;

	uint8_t i = (measDataTail + measDataNum) % BUF_DATA_SIZE;

	uint8_t tmp[6];

	if(!readReg(IMUAddr::ACCEL_XOUT_H, &tmp[0], 6))
		return -1;

	measData[i].accel.x = tmp[0] << 8 | tmp[1];
	measData[i].accel.y = tmp[2] << 8 | tmp[3];
	measData[i].accel.z = tmp[4] << 8 | tmp[5];

	if(!readReg(IMUAddr::TEMP_OUT_H, &tmp[0], 2))
		return -1;

	measData[i].temp = tmp[0] << 8 | tmp[1];

	if(!readReg(IMUAddr::GYRO_XOUT_H, &tmp[0], 6))
		return -1;

	measData[i].alpha.x = tmp[0] << 8 | tmp[1];
	measData[i].alpha.y = tmp[2] << 8 | tmp[3];
	measData[i].alpha.z = tmp[4] << 8 | tmp[5];

	measDataNum++;
	measDataNum %= BUF_DATA_SIZE;

	return 0;
}

uint8_t IMUChannel::whoAmI() const {
	uint8_t command = static_cast<uint8_t>(IMUAddr::WHO_AM_I) | READ_BIT;

	uint8_t imuId;
	STRHAL_SPI_Master_Transceive(spiId, &command, 1, 1, &imuId, 1, 100);

	return imuId;
}

int IMUChannel::reset() {
	measDataTail = measDataNum = 0;

	return 0;
}

bool IMUChannel::IsMeasurementAvailable() const {
	return measDataNum > 0;
}

bool IMUChannel::getMeasurement(IMUData &x) {
	if(measDataNum == 0)
		return false;

	x = measData[measDataTail++];
	measDataTail %= BUF_DATA_SIZE;
	measDataNum--;
	return true;
}

int IMUChannel::getSensorData(uint8_t *data, uint8_t &n) {
	if(measDataNum > 0) {
		std::memcpy(data, &measData[measDataTail++], 12);
		measDataTail %= BUF_DATA_SIZE;
		measDataNum--;
	}

	return 0;
}

int IMUChannel::processMessage(uint8_t commandId, uint8_t *returnData, uint8_t &n) {
	return AbstractChannel::processMessage(commandId, returnData, n);
}

int IMUChannel::setVariable(uint8_t variableId, int32_t data) {
	return -1;
}

int IMUChannel::getVariable(uint8_t variableId, int32_t &data) const {
	return -1;
}

bool IMUChannel::writeReg(const IMUAddr &address, uint8_t reg, uint16_t delay) {
	uint8_t cmd[2];

	cmd[0] = static_cast<uint8_t>(address);
	cmd[1] = reg;

	if(STRHAL_SPI_Master_Transceive(spiId, cmd, 2, 2, nullptr, 0, 100) != 0)
		return false;

	LL_mDelay(delay);
	return true;
}

bool IMUChannel::readReg(const IMUAddr &address, uint8_t *reg, uint8_t n) {
	uint8_t cmd;
	cmd = READ_BIT | static_cast<uint8_t>(address);

	return STRHAL_SPI_Master_Transceive(spiId, &cmd, 1, 1, reg, n, 100) == ((int32_t) n);
}

IMUChannel::~IMUChannel() {

}
