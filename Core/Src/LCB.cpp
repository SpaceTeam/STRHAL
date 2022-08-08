#include "../Inc/LCB.h"

#include <cstdio>
#include <cstring>

LCB::LCB(uint32_t node_id, uint32_t fw_version, uint32_t refresh_divider) :
		GenericChannel(node_id, fw_version, refresh_divider), ledRed(
		{ GPIOD, 1, STRHAL_GPIO_TYPE_OPP }), ledGreen(
		{ GPIOD, 2, STRHAL_GPIO_TYPE_OPP }),

		//sense_5V(0, {ADC2, STRHAL_ADC_CHANNEL_5}, 1),
		speaker(STRHAL_TIM_TIM2, STRHAL_TIM_TIM2_CH3_PB10)
{
	com = Communication::instance(this, nullptr);
	flash = W25Qxx_Flash::instance(0x1F);
	//registerChannel(&sense_5V);

}

int LCB::init()
{
	if (STRHAL_Init(STRHAL_SYSCLK_SRC_EXT, 8000000) != STRHAL_NOICE)
		return -1;

	// init status LEDs
	STRHAL_GPIO_SingleInit(&ledRed, STRHAL_GPIO_TYPE_OPP);
	STRHAL_GPIO_SingleInit(&ledGreen, STRHAL_GPIO_TYPE_OPP);

	if (flash == nullptr)
		return -1;

	if (flash->init() != 0)
		return -1;

	if (com == nullptr)
		return -1;

	if (com->init() != 0)
		return -1;

	if (GenericChannel::init() != 0)
		return -1;

	speaker.init();

	STRHAL_GPIO_Write(&ledGreen, STRHAL_GPIO_VALUE_H);
	return 0;
}

int LCB::exec()
{
	//STRHAL_OPAMP_Run();
	STRHAL_ADC_Run();
	STRHAL_QSPI_Run();

	if (com->exec() != 0)
		return -1;

	STRHAL_GPIO_Write(&ledRed, STRHAL_GPIO_VALUE_H);
	STRHAL_UART_Debug_Write_Blocking("RUNNING\n", 8, 50);

	speaker.beep(3, 300, 200);

	while (1)
	{
		if (GenericChannel::exec() != 0)
			return -1;
	}

	speaker.beep(6, 100, 100);

	return 0;
}
