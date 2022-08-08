#include "../../Inc/Modules/LoRa1276F30_Radio.h"
#include <stm32g4xx_ll_exti.h>
#include <stm32g4xx_ll_system.h>

#include <cstring>
#include <cstdio>

LoRa1276F30_Radio::LoRa1276F30_Radio(const STRHAL_SPI_Id_t &spiId, const STRHAL_SPI_Config_t &spiConf, const STRHAL_GPIO_t &dio1, const STRHAL_GPIO_t &dio3, const STRHAL_GPIO_t &busyPin) :
		spiId(spiId), spiConf(spiConf), dio1(dio1), dio3(dio3), busyPin(busyPin)
{
}

int LoRa1276F30_Radio::init()
{
	// Uncomment if baro interrupt should be used
	/*LL_EXTI_InitTypeDef EXTI_InitStruct = {0};
	 EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_1;
	 EXTI_InitStruct.Line_32_63 = LL_EXTI_LINE_NONE;
	 EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
	 EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING;
	 EXTI_InitStruct.LineCommand = ENABLE;
	 if(LL_EXTI_Init(&EXTI_InitStruct) != 0)
	 return -1;

	 LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTC, LL_SYSCFG_EXTI_LINE1);

	 NVIC_SetPriority(EXTI1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 3, 1));
	 NVIC_EnableIRQ(EXTI1_IRQn);*/

	STRHAL_GPIO_SingleInit(&dio1, STRHAL_GPIO_TYPE_IHZ);
	STRHAL_GPIO_SingleInit(&dio3, STRHAL_GPIO_TYPE_IHZ);
	STRHAL_GPIO_SingleInit(&busyPin, STRHAL_GPIO_TYPE_IHZ);

	if (STRHAL_SPI_Master_Init(spiId, &spiConf) < 0)
		return -1;

	STRHAL_SPI_Master_Run(spiId);

	LL_mDelay(10);

	bool ret = true;
	ret &= setPacketType();
	waitForBusy();
	ret &= calibrateImage();
	waitForBusy();
	ret &= setRFFrequency();
	waitForBusy();
	ret &= setPAConfig();
	waitForBusy();
	ret &= SetTxParams();
	waitForBusy();
	ret &= SetBufferBaseAddress();
	waitForBusy();
	ret &= setModulationParams();
	waitForBusy();
	ret &= setPacketParams();
	waitForBusy();
	//ret &= writeReg(LoraAddr::SYNC_WORD_MSB, 0x00, 1);
	//waitForBusy();
	//ret &= writeReg(LoraAddr::SYNC_WORD_LSB, SYNC_WORD, 1);
	//waitForBusy();
	//uint8_t txModReg = getReg(LoraAddr::TX_MODULATION);
	//waitForBusy();
	//ret &= writeReg(LoraAddr::TX_MODULATION, txModReg & 0xFB, 1);
	//waitForBusy();
	uint8_t txClampConfig = getReg(LoraAddr::TX_CLAMP_CONFIG);
	waitForBusy();
	ret &= writeReg(LoraAddr::TX_CLAMP_CONFIG, txClampConfig | 0x1E, 1);

	if (!ret)
		return -1;

	return 0;
}

int LoRa1276F30_Radio::reset()
{
	return 0;
}

bool LoRa1276F30_Radio::isBusy()
{
	return (STRHAL_GPIO_Read(&busyPin) == STRHAL_GPIO_VALUE_L) ? false : true;
}

void LoRa1276F30_Radio::waitForBusy()
{
	while (isBusy());
}

bool LoRa1276F30_Radio::sendBytes(uint8_t *buffer, uint8_t n)
{
	//if(!SetBufferBaseAddress())
	//return -1;
	//waitForBusy();

	uint8_t cmd[256];
	memset(cmd, 0, 256);
	cmd[0] = static_cast<uint8_t>(LoraOpcode::WRITE_BUFFER);
	cmd[1] = 0x00; // Offset
	memcpy(&cmd[2], buffer, n);

	if (STRHAL_SPI_Master_Transceive(spiId, cmd, n + 2, n + 2, nullptr, 0, 100) != 0)
		return false;
	waitForBusy();
	/*uint8_t rec[256];
	 memset(rec,0,256);
	 uint8_t read[] = {0x1E, 0x00, 0x00};
	 if(STRHAL_SPI_Master_Transceive(spiId, read, 3, 3, rec, 12, 100) != 12)
	 return false;
	 waitForBusy();
	 STRHAL_UART_Write_DMA(STRHAL_UART_DEBUG, (char *) rec, 12);*/
	/*uint8_t stat = getErrors();
	 waitForBusy();
	 STRHAL_UART_Write_DMA(STRHAL_UART_DEBUG, (char *) &stat, 1);*/

	if (!setModulationParams())
		return -1;
	waitForBusy();
	if (!setPacketParams())
		return -1;
	waitForBusy();

	//uint8_t txModReg = getReg(LoraAddr::TX_MODULATION);
	//waitForBusy();
	//if(!writeReg(LoraAddr::TX_MODULATION, txModReg & 0xFB, 1))
	//return -1;
	//waitForBusy();
	if (!setTx())
		return -1;
	waitForBusy();

	uint8_t state = 0;
	while (!state)
	{
		state = getStatus() & 0x20;
	}
	return true;
}

bool LoRa1276F30_Radio::writeReg(const LoraAddr &address, uint8_t reg, uint16_t delay)
{
	uint8_t cmd[4];
	uint16_t addr = static_cast<uint16_t>(address);
	cmd[0] = static_cast<uint8_t>(LoraOpcode::WRITE_REGISTER);
	cmd[1] = (uint8_t) (addr >> 8);
	cmd[2] = (uint8_t) (addr & 0xFF);
	cmd[3] = reg;

	if (STRHAL_SPI_Master_Transceive(spiId, cmd, 4, 4, nullptr, 0, 100) != 0)
		return false;

	LL_mDelay(delay);
	return true;
}

bool LoRa1276F30_Radio::writeCommand(const LoraOpcode &opcode, uint8_t *parameter, uint8_t n, uint16_t delay)
{
	uint8_t cmd[16];
	cmd[0] = static_cast<uint8_t>(opcode);
	for (int i = 0; i < n; i++)
	{
		cmd[i + 1] = parameter[i];
	}

	if (STRHAL_SPI_Master_Transceive(spiId, cmd, n + 1, n + 1, nullptr, 0, 100) != 0)
		return false;

	LL_mDelay(delay);
	return true;
}

bool LoRa1276F30_Radio::readReg(const LoraAddr &address, uint8_t *reg, uint8_t n)
{
	uint8_t cmd[3];
	uint16_t addr = static_cast<uint16_t>(address);
	cmd[0] = static_cast<uint8_t>(LoraOpcode::READ_REGISTER);
	cmd[1] = (uint8_t) (addr >> 8);
	cmd[2] = (uint8_t) (addr & 0xFF);
	cmd[3] = 0;

	return STRHAL_SPI_Master_Transceive(spiId, cmd, 4, 4, reg, n, 100) == ((int32_t) n);
}

uint8_t LoRa1276F30_Radio::getStatus()
{
	uint8_t cmd[2];
	cmd[0] = static_cast<uint8_t>(LoraOpcode::GET_STATUS);
	cmd[1] = 0;
	uint8_t state = 0;

	if (STRHAL_SPI_Master_Transceive(spiId, cmd, 2, 2, &state, 1, 100) != 1)
		return 0;

	return state;
}

uint8_t LoRa1276F30_Radio::getErrors()
{
	uint8_t cmd[2];
	cmd[0] = static_cast<uint8_t>(LoraOpcode::GET_DEVICE_ERRORS);
	cmd[1] = 0;
	uint8_t errors = 0;

	if (STRHAL_SPI_Master_Transceive(spiId, cmd, 2, 2, &errors, 1, 100) != 1)
		return 0;

	return errors;
}

uint8_t LoRa1276F30_Radio::getReg(const LoraAddr &address)
{
	uint8_t reg = 0;
	(void) readReg(address, &reg, 1);
	return reg;
}

bool LoRa1276F30_Radio::setPacketType()
{
	uint8_t parameter = 0x01; // Lora Packet
	return writeCommand(LoraOpcode::SET_PACKET_TYPE, &parameter, 1, 10);
}

bool LoRa1276F30_Radio::calibrateImage()
{
	uint8_t parameter[2] =
	{ 0x6B, 0x6F }; // Calibrate for 433MHz
	return writeCommand(LoraOpcode::CALIBRATE_IMAGE, parameter, 2, 10);
}

bool LoRa1276F30_Radio::setRFFrequency()
{
	uint32_t frf = (FREQUENCY * (uint32_t(1) << 25)) / 32.0;
	uint8_t parameter[4] =
	{ (uint8_t) ((frf >> 24) & 0xFF), (uint8_t) ((frf >> 16) & 0xFF), (uint8_t) ((frf >> 8) & 0xFF), (uint8_t) (frf & 0xFF) };
	return writeCommand(LoraOpcode::SET_FREQUENCY, parameter, 4, 10);
}

bool LoRa1276F30_Radio::setPAConfig()
{
	uint8_t parameter[4] =
	{ 0x04, 0x07, 0x00, 0x01 }; // optimal PA settings for +22dBm
	//uint8_t parameter[4] = { 0x02, 0x02, 0x00, 0x01 };
	return writeCommand(LoraOpcode::SET_PA_CONFIG, parameter, 4, 10);
}

bool LoRa1276F30_Radio::SetTxParams()
{
	uint8_t parameter[2] =
	{ 0x16, 0x00 }; // +22dBm and 40us ramp up/down
	//uint8_t parameter[2] = { 0xEF, 0x02 };
	return writeCommand(LoraOpcode::SET_TX_PARAMS, parameter, 2, 10);
}

bool LoRa1276F30_Radio::SetBufferBaseAddress()
{
	uint8_t parameter[2] =
	{ 0x00, 0x00 }; // set addresses to 0
	return writeCommand(LoraOpcode::SET_BUFFER_BASE_ADDR, parameter, 2, 10);
}

bool LoRa1276F30_Radio::setModulationParams()
{
	uint8_t parameter[4] =
	{ SPREADING_FACTOR, 0x05, 0x02, 0x00 }; // SF7, BW500kHz, CR4/6, no low data optimize
	return writeCommand(LoraOpcode::SET_MODULATION_PARAM, parameter, 4, 10);
}

bool LoRa1276F30_Radio::setPacketParams()
{
	uint8_t parameter[6] =
	{ 0x00, PREAMBLE_LENGTH, 0x01, PKT_LENGTH, CRC_ENABLED, 0x00 }; // 8bit preamble, fixed length header, paket length, crc, normal iq
	return writeCommand(LoraOpcode::SET_PACKET_PARAM, parameter, 6, 10);
}

bool LoRa1276F30_Radio::setDioIrqParams()
{
	uint8_t parameter[8] =
	{ 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00 };
	return writeCommand(LoraOpcode::SET_DIO_IRQ_PARAM, parameter, 8, 10);
}

bool LoRa1276F30_Radio::setTx()
{
	uint8_t parameter[3] =
	{ 0x00, 0x00, 0x00 };
	return writeCommand(LoraOpcode::SET_TX, parameter, 3, 10);
}

/*
 void EXTI1_IRQHandler(void) {
 if(LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_1)) {
 LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_1);
 }
 }*/
