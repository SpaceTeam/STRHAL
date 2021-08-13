#include "main.h"

void PWM_InitPwmIn(void)
{
	LL_TIM_InitTypeDef TIM_InitStruct =	{ 0 };
	LL_TIM_OC_InitTypeDef TIM_OC_InitStruct = { 0 };
	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);

	TIM_InitStruct.Prescaler = 239;
	TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
	TIM_InitStruct.Autoreload = 0xFFFFFFFFU;
	TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
	LL_TIM_Init(TIM1, &TIM_InitStruct);
	LL_TIM_IC_InitTypeDef TIM_IC_InitStruct = {0};
	LL_TIM_IC_StructInit(&TIM_IC_InitStruct);

	TIM_IC_InitStruct.ICPolarity    = LL_TIM_IC_POLARITY_RISING;
	TIM_IC_InitStruct.ICActiveInput = LL_TIM_ACTIVEINPUT_DIRECTTI;
	TIM_IC_InitStruct.ICPrescaler   = LL_TIM_ICPSC_DIV1;
	TIM_IC_InitStruct.ICFilter      = LL_TIM_IC_FILTER_FDIV1;
	LL_TIM_IC_Init(TIM1, LL_TIM_CHANNEL_CH2, &TIM_IC_InitStruct);

	TIM_IC_InitStruct.ICPolarity    = LL_TIM_IC_POLARITY_FALLING;
	TIM_IC_InitStruct.ICActiveInput = LL_TIM_ACTIVEINPUT_INDIRECTTI;
	TIM_IC_InitStruct.ICPrescaler   = LL_TIM_ICPSC_DIV1;
	TIM_IC_InitStruct.ICFilter      = LL_TIM_IC_FILTER_FDIV1;
	LL_TIM_IC_Init(TIM1, LL_TIM_CHANNEL_CH2, &TIM_IC_InitStruct);

	LL_TIM_SetTriggerInput(TIM1, LL_TIM_TS_TI1FP1);
	LL_TIM_SetSlaveMode(TIM1, LL_TIM_SLAVEMODE_RESET);
	LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH1 | LL_TIM_CHANNEL_CH2);

}



Result_t PWM_GetPWM(uint16_t *return_var)
{
	/*
	uint32_t position = pwm_in[i];
	if (position < 1000)
		position = 1000;
	if (position > 2000)
		position = 2000;
	position -= 1000;
	*return_var = (uint32_t) ((65535.0 * 150.0) * (position / 1000.0) + 0.5);
*/
}
