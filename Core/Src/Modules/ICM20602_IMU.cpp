#include "../../Inc/Modules/ICM20602_IMU.h"

#include <cstdio>
#include <cstring>

ICM20602_IMU::ICM20602_IMU(const STRHAL_SPI_Id_t &spiId, const STRHAL_SPI_Config_t &spiConf, const STRHAL_GPIO_t &dataReadyPin) :
		spiId(spiId), spiConf(spiConf), dataReadyPin(dataReadyPin)
{
}

int ICM20602_IMU::init()
{
	STRHAL_GPIO_SingleInit(&dataReadyPin, STRHAL_GPIO_TYPE_IHZ);

	if (STRHAL_SPI_Master_Init(spiId, &spiConf) < 0)
		return -1;

	STRHAL_SPI_Master_Run(spiId);

	if (!writeReg(IMUAddr::PWR_MGMT_1, 0x80, 1000) || !writeReg(IMUAddr::PWR_MGMT_1, 0x01, 50) || !writeReg(IMUAddr::PWR_MGMT_2, 0x3F, 50) || !writeReg(IMUAddr::USER_CTRL, 0x00, 50) || !writeReg(IMUAddr::I2C_IF, 0x40, 50) || !writeReg(IMUAddr::GYRO_CONFIG, 0x19, 50)
			|| !writeReg(IMUAddr::ACCEL_CONFIG, 0x18, 50) || !writeReg(IMUAddr::ACCEL_CONFIG_2, 0x04, 50) || !writeReg(IMUAddr::SMPLRT_DIV, 0x09, 50) || !writeReg(IMUAddr::INT_PIN_CFG, 0x30, 50) || !writeReg(IMUAddr::INT_ENABLE, 0x01, 50) || !writeReg(IMUAddr::PWR_MGMT_2, 0x00, 50))
	{
		return -1;
	}

	if (whoAmI() != 0x12)
		return -1;

	return 0;
}

int ICM20602_IMU::exec()
{
	uint64_t time = STRHAL_Systick_GetTick();
	if ((time - timeLastSample) < EXEC_SAMPLE_TICKS)
		return 0;

	timeLastSample = time;
	if (dataReady())
		(void) read();
	return 0;
}

int ICM20602_IMU::reset()
{
	measDataTail = 0;
	measDataNum = 0;
	return 0;
}

bool ICM20602_IMU::writeReg(const IMUAddr &address, uint8_t reg, uint16_t delay)
{
	uint8_t cmd[2];

	cmd[0] = static_cast<uint8_t>(address);
	cmd[1] = reg;

	if (STRHAL_SPI_Master_Transceive(spiId, cmd, 2, 2, nullptr, 0, 100) != 0)
		return false;

	LL_mDelay(delay);
	return true;
}

bool ICM20602_IMU::readReg(const IMUAddr &address, uint8_t *reg, uint8_t n)
{
	uint8_t cmd;
	cmd = READ_BIT | static_cast<uint8_t>(address);

	return STRHAL_SPI_Master_Transceive(spiId, &cmd, 1, 1, reg, n, 100) == ((int32_t) n);
}

uint8_t ICM20602_IMU::whoAmI() const
{
	uint8_t command = static_cast<uint8_t>(IMUAddr::WHO_AM_I) | READ_BIT;

	uint8_t imuId;
	STRHAL_SPI_Master_Transceive(spiId, &command, 1, 1, &imuId, 1, 100);

	return imuId;
}

bool ICM20602_IMU::dataReady()
{
	return (STRHAL_GPIO_Read(&dataReadyPin) == STRHAL_GPIO_VALUE_L) ? false : true;
}

bool ICM20602_IMU::measurementReady()
{
	return (measDataNum == 0) ? false : true;
}

int ICM20602_IMU::read()
{
	uint8_t i = (measDataTail + measDataNum) % BUF_DATA_SIZE;

	uint8_t tmp[6];

	if (!readReg(IMUAddr::ACCEL_XOUT_H, &tmp[0], 6))
		return -1;

	measData[i].accel.x = tmp[0] << 8 | tmp[1];
	measData[i].accel.y = tmp[2] << 8 | tmp[3];
	measData[i].accel.z = tmp[4] << 8 | tmp[5];

	if (!readReg(IMUAddr::TEMP_OUT_H, &tmp[0], 2))
		return -1;

	measData[i].temp = tmp[0] << 8 | tmp[1];

	if (!readReg(IMUAddr::GYRO_XOUT_H, &tmp[0], 6))
		return -1;

	measData[i].alpha.x = tmp[0] << 8 | tmp[1];
	measData[i].alpha.y = tmp[2] << 8 | tmp[3];
	measData[i].alpha.z = tmp[4] << 8 | tmp[5];

	//char buf[64];
	//sprintf(buf,"%d\n",measData[i].accel.z);
	//STRHAL_UART_Debug_Write(buf, strlen(buf));

	measDataNum++;
	measDataNum %= BUF_DATA_SIZE;

	return 0;
}

void ICM20602_IMU::getMeasurement(uint16_t &measurement, IMUMeasurement measurementType)
{
	IMUData allMeasurements = measData[measDataTail];
	switch (measurementType)
	{
		case IMUMeasurement::X_ACCEL:
			measurement = (uint16_t) allMeasurements.accel.x;
			break;
		case IMUMeasurement::Y_ACCEL:
			measurement = (uint16_t) allMeasurements.accel.y;
			break;
		case IMUMeasurement::Z_ACCEL:
			measurement = (uint16_t) allMeasurements.accel.z;
			break;
		case IMUMeasurement::X_GYRO:
			measurement = (uint16_t) allMeasurements.alpha.x;
			break;
		case IMUMeasurement::Y_GYRO:
			measurement = (uint16_t) allMeasurements.alpha.y;
			break;
		case IMUMeasurement::Z_GYRO:
			measurement = (uint16_t) allMeasurements.alpha.z;
			measDataTail++;
			measDataTail %= BUF_DATA_SIZE;
			measDataNum--;
			break;
		default:
			return;
	}
}
