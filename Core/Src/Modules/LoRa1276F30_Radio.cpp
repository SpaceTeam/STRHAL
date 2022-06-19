#include "../../Inc/Modules/LoRa1276F30_Radio.h"
#include <stm32g4xx_ll_exti.h>
#include <stm32g4xx_ll_system.h>

#include <cstring>
#include <cstdio>

LoRa1276F30_Radio::LoRa1276F30_Radio(const STRHAL_SPI_Id_t &spiId, const STRHAL_SPI_Config_t &spiConf, const STRHAL_GPIO_t &dio1, const STRHAL_GPIO_t &dio3) :
	spiId(spiId),
	spiConf(spiConf),
	dio1(dio1),
	dio3(dio3) {
}

int LoRa1276F30_Radio::init() {
	// Uncomment if baro interrupt should be used
	/*STRHAL_GPIO_SingleInit(&dataReadyPin, STRHAL_GPIO_TYPE_IHZ);
	LL_EXTI_InitTypeDef EXTI_InitStruct = {0};
	EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_3;
	EXTI_InitStruct.Line_32_63 = LL_EXTI_LINE_NONE;
	EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
	EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING;
	EXTI_InitStruct.LineCommand = ENABLE;
	if(LL_EXTI_Init(&EXTI_InitStruct) != 0)
		return -1;

	LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTC, LL_SYSCFG_EXTI_LINE3);

	NVIC_SetPriority(EXTI3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 3, 1));
	NVIC_EnableIRQ(EXTI3_IRQn);*/

	if(STRHAL_SPI_Master_Init(spiId, &spiConf) < 0)
		return -1;

	STRHAL_SPI_Master_Run(spiId);

	LL_mDelay(10);

	if(whoAmI() != 0xc8)
		return -1;

	//lora_reset(); TODO solder bridge for reset

	bool ret = true;
	ret &= setState(LoraState::SLEEPING);
	ret &= setFrequency(FREQUENCY);
	ret &= writeReg(LoraAddr::FIFO_TX_BASE_ADDR, 0, 10);
	ret &= writeReg(LoraAddr::FIFO_RX_BASE_ADDR, 0, 10);
	ret &= writeReg(LoraAddr::OCP, 0x0B, 10);
	ret &= writeReg(LoraAddr::LNA, 0x23, 10);
	ret &= writeReg(LoraAddr::ERRATA1, 0x02, 10); //See Errata note
	ret &= writeReg(LoraAddr::ERRATA2, 0x64, 10); //See Errata note
	ret &= setTxPower(TX_POWER);
	ret &= setSpreadingFactor(SPREADING_FACTOR);
	ret &= setCodingRate4(CODING_RATE);
	ret &= setSignalBandwidth(SIGNAL_BANDWIDTH);
	ret &= setPreambleLength(PREAMBLE_LENGTH);
	ret &= setSyncWord(SYNC_WORD);
	ret &= implicitHeaderMode();
	ret &= writeReg(LoraAddr::PAYLOAD_LENGTH, 103, 10);
	ret &= disableCRC();

	ret &= setState(LoraState::RECEIVING);

	if(!ret)
		return -1;

	return 0;
}

int LoRa1276F30_Radio::reset() {
	return 0;
}

uint8_t LoRa1276F30_Radio::whoAmI() {
	uint8_t version = 0;
	(void) readReg(LoraAddr::VERSION, &version, 1);

	return version;
}

uint8_t LoRa1276F30_Radio::getMode() {
	uint8_t mode = 0;
	(void) readReg(LoraAddr::OP_MODE, &mode, 1);
	return mode;
}

LoraState LoRa1276F30_Radio::getState() {
	uint8_t mode = getMode();
	if(mode == (static_cast<uint8_t>(LoraMode::LONG_RANGE_MODE) | static_cast<uint8_t>(LoraMode::TX)))
		return LoraState::SENDING;
	if(mode == (static_cast<uint8_t>(LoraMode::LONG_RANGE_MODE) | static_cast<uint8_t>(LoraMode::RX_CONTINUOUS)) || mode == (static_cast<uint8_t>(LoraMode::LONG_RANGE_MODE) | static_cast<uint8_t>(LoraMode::RX_SINGLE)))
		return LoraState::RECEIVING;
	if(mode == (static_cast<uint8_t>(LoraMode::LONG_RANGE_MODE) | static_cast<uint8_t>(LoraMode::STDBY)))
		return LoraState::IDLE;
	if(mode == (static_cast<uint8_t>(LoraMode::LONG_RANGE_MODE) | static_cast<uint8_t>(LoraMode::SLEEP)))
		return LoraState::SENDING;
	return LoraState::DISCONNECTED;
}

bool LoRa1276F30_Radio::setState(LoraState state) {
	uint8_t mode = getMode();
	bool ret = true;
	switch(state) {
		case LoraState::DISCONNECTED:
			break;
		case LoraState::IDLE:
			ret &= writeReg(LoraAddr::OP_MODE, static_cast<uint8_t>(LoraMode::LONG_RANGE_MODE), 10);
			ret &= writeReg(LoraAddr::OP_MODE, static_cast<uint8_t>(LoraMode::STDBY), 10);
			break;
		case LoraState::RECEIVING:
			if(mode != (static_cast<uint8_t>(LoraMode::LONG_RANGE_MODE) | static_cast<uint8_t>(LoraMode::RX_CONTINUOUS))) {
				ret &= writeReg(LoraAddr::OP_MODE, static_cast<uint8_t>(LoraMode::STDBY), 10);
				ret &= writeReg(LoraAddr::DIO_MAPPING_1, 0, 10);
				//lora_writeRegister(LR_RegHopPeriod, 0xFF);	//No FHSS
				ret &= writeReg(LoraAddr::LR_RegHopPeriod, 0x00, 10);	//No FHSS
				ret &= writeReg(LoraAddr::PAYLOAD_LENGTH, PKT_LENGTH, 10);
				ret &= writeReg(LoraAddr::FIFO_ADDR_PTR, 0, 10);
				ret &= writeReg(LoraAddr::OP_MODE, static_cast<uint8_t>(LoraMode::LONG_RANGE_MODE), 10);
				ret &= writeReg(LoraAddr::OP_MODE, static_cast<uint8_t>(LoraMode::LONG_RANGE_MODE) | static_cast<uint8_t>(LoraMode::RX_CONTINUOUS), 10);
			}
			break;
		case LoraState::SENDING:
			ret &= writeReg(LoraAddr::OP_MODE, static_cast<uint8_t>(LoraMode::LONG_RANGE_MODE) | static_cast<uint8_t>(LoraMode::TX), 10);
			break;
		case LoraState::SLEEPING:
			ret &= writeReg(LoraAddr::OP_MODE, static_cast<uint8_t>(LoraMode::LONG_RANGE_MODE) | static_cast<uint8_t>(LoraMode::SLEEP), 10);
			break;
	}
	return ret;
}

bool LoRa1276F30_Radio::ready() {
	LoraState state = getState();
	return (state != LoraState::SENDING) && (state != LoraState::RECEIVING) && (state != LoraState::DISCONNECTED);
}

int LoRa1276F30_Radio::sendBytes(uint8_t* buffer, uint8_t length) {
	int ret = length;
	LoraState state = getState();
	if((state == LoraState::SENDING) || (state == LoraState::DISCONNECTED))
		return 0;
	setState(LoraState::IDLE);

	bool writeSuccess = true;
	writeSuccess &= writeReg(LoraAddr::IRQ_FLAGS, static_cast<uint8_t>(LoraIRQ::TX_DONE_MASK)); //Clear irq
	writeSuccess &= writeReg(LoraAddr::LR_RegHopPeriod, 0x00);	//No FHSS
	writeSuccess &= writeReg(LoraAddr::DIO_MAPPING_1, 1 << 6);
	writeSuccess &= writeReg(LoraAddr::FIFO_ADDR_PTR, 0);
	writeSuccess &= writeFifo(LoraAddr::FIFO,buffer,length);
	writeSuccess &= setState(LoraState::SENDING);
	if(!writeSuccess)
		return -1;

	uint64_t startTime = STRHAL_Systick_GetTick();
	while ((getReg(LoraAddr::IRQ_FLAGS) & static_cast<uint8_t>(LoraIRQ::TX_DONE_MASK)) == 0)
	{
		if((STRHAL_Systick_GetTick() - startTime) > 200)
		{
			ret = -1;
			break;
		}
	}
	ret &= writeReg(LoraAddr::IRQ_FLAGS, static_cast<uint8_t>(LoraIRQ::TX_DONE_MASK));
	//if (_transmitCompleteClb) _transmitCompleteClb();
	//stat = idle;
	return ret;
}

int16_t LoRa1276F30_Radio::read() {
	if (!available()) {
		return -1;
	}

	_packetIndex++;

	uint8_t reg;
	reg = getReg(LoraAddr::FIFO);

	return reg;
}

uint16_t LoRa1276F30_Radio::readBytes(uint8_t *data, uint16_t length) {
	uint16_t i = 0;
	uint8_t byte;
	while((byte = read()) >= 0)
	{
		if(i == length)
		break;
		data[i] = byte;
		i++;
	}
	return i;
}

uint8_t LoRa1276F30_Radio::available() {
	LoraState state = getState();
	if(state == LoraState::SENDING ||state == LoraState::DISCONNECTED)
		return 0;
	uint8_t reg;
	reg = getReg(LoraAddr::RX_NB_BYTES);
	return (reg - _packetIndex);
}

bool LoRa1276F30_Radio::setTxPower(uint8_t level) {
	if (level < 2) {
		level = 2;
		} else if (level > 17) {
		level = 17;
	}
	return writeRegSafe(LoraAddr::PA_CONFIG, 0x80 | (level - 2), 10);
}

bool LoRa1276F30_Radio::setFrequency(uint32_t frequency) {
	uint64_t frf = ((uint64_t) frequency << 19) / 32000000;
	uint8_t mode = getMode();
	bool ret = true;
	if(mode != (static_cast<uint8_t>(LoraMode::LONG_RANGE_MODE) | static_cast<uint8_t>(LoraMode::SLEEP)))
		setState(LoraState::SLEEPING);

	ret &= writeReg(LoraAddr::FRF_MSB, (uint8_t) (frf >> 16), 10);
	ret &= writeReg(LoraAddr::FRF_MID, (uint8_t) (frf >> 8), 10);
	ret &= writeReg(LoraAddr::FRF_LSB, (uint8_t) (frf >> 0), 10);
	if(mode != (static_cast<uint8_t>(LoraMode::LONG_RANGE_MODE) | static_cast<uint8_t>(LoraMode::SLEEP)))
		ret &= writeReg(LoraAddr::OP_MODE, mode, 10);

	return ret;
}

bool LoRa1276F30_Radio::setSpreadingFactor(uint8_t sf) {
	if (sf < 6) {
		sf = 6;
	} else if (sf > 12) {
		sf = 12;
	}
	uint8_t mode = getMode();
	bool ret = true;
	if(mode != (static_cast<uint8_t>(LoraMode::LONG_RANGE_MODE) | static_cast<uint8_t>(LoraMode::SLEEP)))
		setState(LoraState::SLEEPING);

	if (sf == 6) {
		ret &= writeReg(LoraAddr::DETECTION_OPTIMIZE, 0xc5, 10);
		ret &= writeReg(LoraAddr::DETECTION_THRESHOLD, 0x0c, 10);
	} else {
		ret &= writeReg(LoraAddr::DETECTION_OPTIMIZE, 0xc3, 10);
		ret &= writeReg(LoraAddr::DETECTION_THRESHOLD, 0x0a, 10);
	}

	uint8_t modem_config = 0;
	ret &= readReg(LoraAddr::MODEM_CONFIG_2, &modem_config, 1);
	ret &= writeReg(LoraAddr::MODEM_CONFIG_2,(modem_config & 0x0f) | ((sf << 4) & 0xf0));
	if(mode != (static_cast<uint8_t>(LoraMode::LONG_RANGE_MODE) | static_cast<uint8_t>(LoraMode::SLEEP)))
		ret &= writeReg(LoraAddr::OP_MODE, mode);

	return ret;
}

bool LoRa1276F30_Radio::setSignalBandwidth(uint32_t sbw) {
	uint8_t bw = 9;

	if (sbw <= 7.8E3) {
		bw = 0;
	} else if (sbw <= 10.4E3) {
		bw = 1;
	} else if (sbw <= 15.6E3) {
		bw = 2;
	} else if (sbw <= 20.8E3) {
		bw = 3;
	} else if (sbw <= 31.25E3) {
		bw = 4;
	} else if (sbw <= 41.7E3) {
		bw = 5;
	} else if (sbw <= 62.5E3) {
		bw = 6;
	} else if (sbw <= 125E3) {
		bw = 7;
	} else if (sbw <= 250E3) {
		bw = 8;
	} else if (sbw <= 500E3){
		bw = 9;
	}
	bool ret = true;
	uint8_t modem_config = 0;
	ret &= readReg(LoraAddr::MODEM_CONFIG_1, &modem_config, 1);
	ret &= writeRegSafe(LoraAddr::MODEM_CONFIG_1, (modem_config & 0x0f) | (bw << 4), 10);

	return ret;
}

bool LoRa1276F30_Radio::setCodingRate4(uint8_t denominator) {
	if (denominator < 5) {
		denominator = 5;
	} else if (denominator > 8) {
		denominator = 8;
	}

	uint8_t cr = denominator - 4;

	bool ret = true;

	uint8_t modem_config = 0;
	ret &= readReg(LoraAddr::MODEM_CONFIG_1, &modem_config, 1);
	ret &= writeRegSafe(LoraAddr::MODEM_CONFIG_1,(modem_config & 0xf1) | (cr << 1), 10);

	return ret;
}

bool LoRa1276F30_Radio::setPreambleLength(uint16_t length) {
	uint8_t mode = getMode();
	bool ret = true;
	if(mode != (static_cast<uint8_t>(LoraMode::LONG_RANGE_MODE) | static_cast<uint8_t>(LoraMode::SLEEP)))
		setState(LoraState::SLEEPING);

	ret &= writeReg(LoraAddr::PREAMBLE_MSB, (uint8_t) (length >> 8), 10);
	ret &= writeReg(LoraAddr::PREAMBLE_LSB, (uint8_t) (length >> 0), 10);
	if(mode != (static_cast<uint8_t>(LoraMode::LONG_RANGE_MODE) | static_cast<uint8_t>(LoraMode::SLEEP)))
		ret &= writeReg(LoraAddr::OP_MODE, mode, 10);

	return ret;
}

bool LoRa1276F30_Radio::setSyncWord(uint8_t sw) {
	return writeRegSafe(LoraAddr::SYNC_WORD, sw, 10);
}

bool LoRa1276F30_Radio::enableCRC() {
	uint8_t modem_config = 0;
	bool ret = true;
	ret &= readReg(LoraAddr::MODEM_CONFIG_2, &modem_config, 1);
	ret &= writeRegSafe(LoraAddr::MODEM_CONFIG_2,(modem_config | 0x04), 10);

	return ret;
}

bool LoRa1276F30_Radio::disableCRC() {
	uint8_t modem_config = 0;
	bool ret = true;
	ret &= readReg(LoraAddr::MODEM_CONFIG_2, &modem_config, 1);
	ret &= writeRegSafe(LoraAddr::MODEM_CONFIG_2,(modem_config & 0xfb), 10);

	return ret;
}

bool LoRa1276F30_Radio::explicitHeaderMode() {
	uint8_t modem_config = 0;
	bool ret = true;
	ret &= readReg(LoraAddr::MODEM_CONFIG_1, &modem_config, 1);
	ret &= writeRegSafe(LoraAddr::MODEM_CONFIG_1,(modem_config & 0xfe), 10);

	return ret;
}

bool LoRa1276F30_Radio::implicitHeaderMode() {
	uint8_t modem_config = 0;
	bool ret = true;
	ret &= readReg(LoraAddr::MODEM_CONFIG_1, &modem_config, 1);
	ret &= writeRegSafe(LoraAddr::MODEM_CONFIG_1,(modem_config | 0x01), 10);

	return ret;
}

/*void lora_handleDio0Rise()
{
	uint8_t irqFlags;
	lora_writeRegister(REG_PAYLOAD_LENGTH, lora_getMessageSize());
	irqFlags = lora_readRegister(REG_IRQ_FLAGS);
	lora_writeRegister(REG_IRQ_FLAGS, irqFlags);
	if (irqFlags & IRQ_RX_DONE_MASK)
	{
		if ((irqFlags & IRQ_PAYLOAD_CRC_ERROR_MASK) == 0)
		{
			_packetIndex = 0;
			if (_onReceive) _onReceive();
		}
	}
	else if (irqFlags & IRQ_TX_DONE_MASK)
	{
		if (_transmitCompleteClb) _transmitCompleteClb();
	}
}*/

bool LoRa1276F30_Radio::writeRegSafe(const LoraAddr &address, uint8_t reg, uint16_t delay) {
	uint8_t mode = getMode();
	bool ret = true;
	if(mode != (static_cast<uint8_t>(LoraMode::LONG_RANGE_MODE) | static_cast<uint8_t>(LoraMode::SLEEP)))
		setState(LoraState::SLEEPING);

	ret &= writeReg(address, reg, delay);
	if(mode != (static_cast<uint8_t>(LoraMode::LONG_RANGE_MODE) | static_cast<uint8_t>(LoraMode::SLEEP)))
		ret &= writeReg(LoraAddr::OP_MODE, mode, 10);

	return ret;
}

bool LoRa1276F30_Radio::writeReg(const LoraAddr &address, uint8_t reg, uint16_t delay) {
	uint8_t cmd[2];

	cmd[0] = WRITE_BIT | static_cast<uint8_t>(address);
	cmd[1] = reg;

	if(STRHAL_SPI_Master_Transceive(spiId, cmd, 2, 2, nullptr, 0, 100) != 0)
		return false;

	LL_mDelay(delay);
	return true;
}

bool LoRa1276F30_Radio::readReg(const LoraAddr &address, uint8_t *reg, uint8_t n) {
	uint8_t cmd;
	cmd = static_cast<uint8_t>(address);

	return STRHAL_SPI_Master_Transceive(spiId, &cmd, 1, 1, reg, n, 100) == ((int32_t) n);
}

uint8_t LoRa1276F30_Radio::getReg(const LoraAddr &address) {
	uint8_t reg = 0;
	(void) readReg(address, &reg, 1);
	return reg;
}

bool LoRa1276F30_Radio::writeFifo(const LoraAddr &address, uint8_t *data, uint8_t n) {

	uint8_t fifo[256];
	fifo[0] = WRITE_BIT | static_cast<uint8_t>(address);
	memcpy(&fifo[1], data, n);
	return STRHAL_SPI_Master_Transceive(spiId, fifo, n+1, n+1, nullptr, 0, 100) == ((int32_t) n);
}
