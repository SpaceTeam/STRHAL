#include "../../Inc/Modules/LPS25HB_Baro.h"
#include <stm32g4xx_ll_exti.h>
#include <stm32g4xx_ll_system.h>

#include <cstring>
#include <cstdio>

LPS25HB_Baro::LPS25HB_Baro(const STRHAL_SPI_Id_t &spiId, const STRHAL_SPI_Config_t &spiConf, const STRHAL_GPIO_t &dataReadyPin) :
		spiId(spiId), spiConf(spiConf), dataReadyPin(dataReadyPin)
{
}

int LPS25HB_Baro::init()
{
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

	STRHAL_GPIO_SingleInit(&dataReadyPin, STRHAL_GPIO_TYPE_IHZ);

	if (STRHAL_SPI_Master_Init(spiId, &spiConf) < 0)
		return -1;

	STRHAL_SPI_Master_Run(spiId);

	// Set Power-Down mode - reset CTRL_1 register
	if (!writeReg(BaroAddr::CTRL_REG1, 0x0, 100))
		return -1;

	if (!writeReg(BaroAddr::CTRL_REG1, 0x44, 50) || !writeReg(BaroAddr::CTRL_REG2, 0x08, 50) || !writeReg(BaroAddr::CTRL_REG3, 0x00, 50) || !writeReg(BaroAddr::CTRL_REG4, 0x01, 50)
	//|| !writeReg(BaroAddr::FIFO_CTRL, 0x20, 50)
			|| !writeReg(BaroAddr::RES_CONF, 0x07, 50))
		return -1;

	uint8_t ctrlReg1Value;
	if (!readReg(BaroAddr::CTRL_REG1, &ctrlReg1Value, 1))
		return -1;

	ctrlReg1Value |= 0x80;

	// Power up
	if (!writeReg(BaroAddr::CTRL_REG1, ctrlReg1Value, 100))
		return -1;

	if (whoAmI() != 0xBD)
		return -1;

	LL_mDelay(10);

	return 0;
}

int LPS25HB_Baro::exec()
{
	uint64_t time = STRHAL_Systick_GetTick();
	if ((time - timeLastSample) < EXEC_SAMPLE_TICKS)
		return 0;

	timeLastSample = time;

	(void) read();

	return 0;
}

int LPS25HB_Baro::reset()
{
	measDataTail = 0;
	measDataNum = 0;
	return 0;
}

bool LPS25HB_Baro::writeReg(const BaroAddr &address, uint8_t reg, uint16_t delay)
{
	uint8_t cmd[2];

	cmd[0] = static_cast<uint8_t>(address);
	cmd[1] = reg;

	if (STRHAL_SPI_Master_Transceive(spiId, cmd, 2, 2, nullptr, 0, 100) != 0)
		return false;

	LL_mDelay(delay);
	return true;
}

bool LPS25HB_Baro::readReg(const BaroAddr &address, uint8_t *reg, uint8_t n)
{
	uint8_t cmd;
	cmd = READ_BIT | static_cast<uint8_t>(address);

	if (n > 1)
		cmd |= 0x40; // set MS bit in address to increment

	return STRHAL_SPI_Master_Transceive(spiId, &cmd, 1, 1, reg, n, 100) == ((int32_t) n);
}

uint8_t LPS25HB_Baro::whoAmI() const
{
	uint8_t command = static_cast<uint8_t>(BaroAddr::WHO_AM_I) | READ_BIT;

	uint8_t imuId;
	STRHAL_SPI_Master_Transceive(spiId, &command, 1, 1, &imuId, 1, 100);

	return imuId;
}

bool LPS25HB_Baro::measurementReady()
{
	return (measDataNum == 0) ? false : true;
}

int LPS25HB_Baro::read()
{
	uint8_t i = (measDataTail + measDataNum) % BUF_DATA_SIZE;

	uint8_t tmp[3];
	uint32_t tmpPressure;

	if (!readReg(BaroAddr::PRESS_OUT_XL, &tmp[0], 3))
		return -1;

	tmpPressure = tmp[2] << 16 | tmp[1] << 8 | tmp[0];

	// convert 24bit 2s complement to 32bit 2s complement - not necessary if sent to LL Server in 24bit
	//if(tmpPressure & 0x00800000)
	//tmpPressure |= 0xFF000000;

	measData[i] = (int32_t) tmpPressure;

	if (!readReg(BaroAddr::TEMP_OUT_L, &tmp[0], 2))
		return -1;

	//measData[i].temp = tmp[0] << 8 | tmp[1]; discarding temperature measurement

	/*char buf[64];
	 sprintf(buf,"%ld\n",measData[i]);
	 STRHAL_UART_Debug_Write(buf, strlen(buf));*/

	measDataNum++;
	measDataNum %= BUF_DATA_SIZE;

	return 0;
}

void LPS25HB_Baro::getMeasurement(int32_t &measurement)
{
	measurement = measData[measDataTail++];
	measDataTail %= BUF_DATA_SIZE;
	measDataNum--;
}

/*
 void EXTI3_IRQHandler(void) {
 if(LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_3)) {
 LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_3);
 }
 }*/
