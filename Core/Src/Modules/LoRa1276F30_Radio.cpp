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

	// Set Power-Down mode - reset CTRL_1 register
	/*if(!writeReg(BaroAddr::CTRL_REG1, 0x0, 100))
		return -1;

	if(!writeReg(BaroAddr::CTRL_REG1, 0x44, 50)
			 ||	!writeReg(BaroAddr::CTRL_REG2, 0x08, 50)
			 || !writeReg(BaroAddr::CTRL_REG3, 0x00, 50)
			 || !writeReg(BaroAddr::CTRL_REG4, 0x01, 50)
			 //|| !writeReg(BaroAddr::FIFO_CTRL, 0x20, 50)
			 || !writeReg(BaroAddr::RES_CONF, 0x07, 50)
			)
		return -1;

	uint8_t ctrlReg1Value;
	if(!readReg(BaroAddr::CTRL_REG1, &ctrlReg1Value, 1))
		return -1;

	ctrlReg1Value |= 0x80;

	// Power up
	if(!writeReg(BaroAddr::CTRL_REG1, ctrlReg1Value, 100))
		return -1;*/

	LL_mDelay(10);

	return 0;
}

int LoRa1276F30_Radio::reset() {
	return 0;
}
