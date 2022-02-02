#include "../Inc/LID_OPAMP.h"

void LID_OPAMP_Init() {

	// GPIO init
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOF);
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOE);
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOD);

	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_7, LL_GPIO_MODE_ANALOG);
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_0, LL_GPIO_MODE_ANALOG);

	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);

	LL_OPAMP_SetFunctionalMode(OPAMP2, LL_OPAMP_MODE_PGA);
	LL_OPAMP_SetPGAGain(OPAMP2, LL_OPAMP_PGA_GAIN_32_OR_MINUS_31);
	LL_OPAMP_SetInputNonInverting(OPAMP2, LL_OPAMP_INPUT_NONINVERT_IO0);
	LL_OPAMP_SetInputInverting(OPAMP2, LL_OPAMP_INPUT_INVERT_CONNECT_NO);
	LL_OPAMP_SetInternalOutput(OPAMP2,LL_OPAMP_INTERNAL_OUPUT_ENABLED);
	LL_OPAMP_SetFunctionalMode(OPAMP3, LL_OPAMP_MODE_PGA);
	LL_OPAMP_SetPGAGain(OPAMP3, LL_OPAMP_PGA_GAIN_32_OR_MINUS_31);
	LL_OPAMP_SetInputNonInverting(OPAMP3, LL_OPAMP_INPUT_NONINVERT_IO0);
	LL_OPAMP_SetInputInverting(OPAMP3, LL_OPAMP_INPUT_INVERT_CONNECT_NO);
	LL_OPAMP_SetInternalOutput(OPAMP3,LL_OPAMP_INTERNAL_OUPUT_ENABLED);
}

void LID_OPAMP_Run() {
	LL_OPAMP_Enable(OPAMP2);
	while(!LL_OPAMP_IsEnabled(OPAMP2))
		;
	LL_OPAMP_Enable(OPAMP3);
	while(!LL_OPAMP_IsEnabled(OPAMP3))
		;
}