#include "../Inc/Channels/BaroChannel.h"

#include <cstring>
#include <cstdio>

BaroChannel::BaroChannel(uint8_t id, const STRHAL_SPI_Id_t &spiId, const STRHAL_SPI_Config_t &spiConf, uint32_t refreshDivider) :
	AbstractChannel(CHANNEL_TYPE_ADC24, id, refreshDivider),
	spiId(spiId),
	spiConf(spiConf) {
	measDataTail = measDataNum = 0;
}

int BaroChannel::init() {
	if(STRHAL_SPI_Master_Init(spiId, &spiConf) < 0)
		return -1;

	STRHAL_SPI_Master_Run(spiId);

	// Set Power-Down mode - reset CTRL_1 register
	if(!writeReg(BaroAddr::CTRL_REG1, 0x0, 100))
		return -1;

	if(!writeReg(BaroAddr::CTRL_REG1, 0x44, 50)
			 ||	!writeReg(BaroAddr::CTRL_REG2, 0x48, 50)
			 || !writeReg(BaroAddr::CTRL_REG3, 0x00, 50)
			 || !writeReg(BaroAddr::CTRL_REG4, 0x00, 50)
			 || !writeReg(BaroAddr::FIFO_CTRL, 0x20, 50)
			 || !writeReg(BaroAddr::RES_CONF, 0x0A, 50)
			)
		return -1;

	uint8_t ctrlReg1Value;
	if(!readReg(BaroAddr::CTRL_REG1, &ctrlReg1Value, 1))
		return -1;

	ctrlReg1Value |= 0x80;

	// Power up
	if(!writeReg(BaroAddr::CTRL_REG1, ctrlReg1Value, 100))
		return -1;

	if(whoAmI() != 0xBD)
		return -1;

	return 0;
}

int BaroChannel::exec() {
	char buf[64];

	uint64_t time = STRHAL_Systick_GetTick();
	if((time - timeLastSample) < EXEC_SAMPLE_TICKS)
		return 0;

	timeLastSample = time;

	uint8_t i = (measDataTail + measDataNum) % BUF_DATA_SIZE;

	uint8_t tmp[3];
	uint32_t tmpPressure;

	if(!readReg(BaroAddr::PRESS_OUT_XL, &tmp[0], 3))
		return -1;

	tmpPressure = tmp[2] << 16 | tmp[1] << 8 | tmp[0];

	// convert 24bit 2s complement to 32bit 2s complement - not necessary if sent to LL Server in 24bit
	//if(tmpPressure & 0x00800000)
		//tmpPressure |= 0xFF000000;

	measData[i] = (int32_t) tmpPressure;

	if(!readReg(BaroAddr::TEMP_OUT_L, &tmp[0], 2))
		return -1;

	//measData[i].temp = tmp[0] << 8 | tmp[1]; discarding temperature measurement

	if(!readReg(BaroAddr::WHO_AM_I, &tmp[0], 1))
		return -1;

	measDataNum++;
	measDataNum %= BUF_DATA_SIZE;

	sprintf(buf,"%li - %d\n",(int32_t) tmpPressure, tmp[0]);
	STRHAL_UART_Write(buf, strlen(buf));

	return 0;
}

uint8_t BaroChannel::whoAmI() const {
	uint8_t command = static_cast<uint8_t>(BaroAddr::WHO_AM_I) | READ_BIT;

	uint8_t imuId;
	STRHAL_SPI_Master_Transceive(spiId, &command, 1, 1, &imuId, 1, 100);

	return imuId;
}

int BaroChannel::reset() {
	measDataTail = measDataNum = 0;

	return 0;
}

bool BaroChannel::IsMeasurementAvailable() const {
	return measDataNum > 0;
}

bool BaroChannel::getMeasurement(int32_t &x) {
	if(measDataNum == 0)
		return false;

	x = measData[measDataTail++];
	// convert 24bit 2s complement to 32bit 2s complement
	if(x & 0x00800000)
		x |= 0xFF000000;

	measDataTail %= BUF_DATA_SIZE;
	measDataNum--;
	return true;
}

int BaroChannel::getSensorData(uint8_t *data, uint8_t &n) {
	if(measDataNum > 0) {
		uint32_t *out = (uint32_t *) (data+n);
		out[0] = (uint8_t) (measData[measDataTail] >> 0) & 0xFF;
		out[1] = (uint8_t) (measData[measDataTail] >> 8) & 0xFF;
		out[2] = (uint8_t) (measData[measDataTail++] >> 16) & 0xFF;
		measDataTail %= BUF_DATA_SIZE;
		measDataNum--;

		n += ADC24_DATA_N_BYTES;
		return 0;
	}

	return -1;
}

int BaroChannel::processMessage(uint8_t commandId, uint8_t *returnData, uint8_t &n) {
	return AbstractChannel::processMessage(commandId, returnData, n);
}

int BaroChannel::setVariable(uint8_t variableId, int32_t data) {
	switch(variableId) {
		case ADC24_REFRESH_DIVIDER:
			refreshDivider = data;
			refreshCounter = 0;
			return 0;
		default:
			return -1;
	}
}

int BaroChannel::getVariable(uint8_t variableId, int32_t &data) const {
	switch(variableId) {
		case ADC24_REFRESH_DIVIDER:
			data = (int32_t) refreshDivider;
			return 0;
		default:
			return -1;
	}
}

bool BaroChannel::writeReg(const BaroAddr &address, uint8_t reg, uint16_t delay) {
	uint8_t cmd[2];

	cmd[0] = static_cast<uint8_t>(address);
	cmd[1] = reg;

	if(STRHAL_SPI_Master_Transceive(spiId, cmd, 2, 2, nullptr, 0, 100) != 0)
		return false;

	LL_mDelay(delay);
	return true;
}

bool BaroChannel::readReg(const BaroAddr &address, uint8_t *reg, uint8_t n) {
	uint8_t cmd;
	cmd = READ_BIT | static_cast<uint8_t>(address);

	if(n > 1)
		cmd |= 0x40; // set MS bit in address to increment

	return STRHAL_SPI_Master_Transceive(spiId, &cmd, 1, 1, reg, n, 100) == ((int32_t) n);
}

BaroChannel::~BaroChannel() {

}
