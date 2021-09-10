#include "main.h"
#include "systick.h"

void PWM_InitPwmIn(void)
{
	LL_TIM_InitTypeDef TIM_InitStruct =
	{ 0 };

	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);

	LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOE);

	LL_GPIO_InitTypeDef GPIO_InitStruct =
	{ 0 };
	GPIO_InitStruct.Pin = LL_GPIO_PIN_11;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
	LL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);

	TIM_InitStruct.Prescaler = 31;

	TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
	TIM_InitStruct.Autoreload = 0xFFFF;
	TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV2;
	TIM_InitStruct.RepetitionCounter = 0;
	LL_TIM_Init(TIM1, &TIM_InitStruct);

	//LL_TIM_DisableARRPreload(TIM1);
	LL_TIM_EnableARRPreload(TIM1);
	LL_TIM_SetClockSource(TIM1, LL_TIM_CLOCKSOURCE_INTERNAL);

	LL_TIM_IC_InitTypeDef TIM_IC_InitStruct =
	{ 0 };
	LL_TIM_IC_StructInit(&TIM_IC_InitStruct);

	TIM_IC_InitStruct.ICPolarity = LL_TIM_IC_POLARITY_RISING;
	TIM_IC_InitStruct.ICActiveInput = LL_TIM_ACTIVEINPUT_INDIRECTTI;
	TIM_IC_InitStruct.ICPrescaler = LL_TIM_ICPSC_DIV1;
	TIM_IC_InitStruct.ICFilter = LL_TIM_IC_FILTER_FDIV1;
	LL_TIM_IC_Init(TIM1, LL_TIM_CHANNEL_CH1, &TIM_IC_InitStruct);

	TIM_IC_InitStruct.ICPolarity = LL_TIM_IC_POLARITY_FALLING;
	TIM_IC_InitStruct.ICActiveInput = LL_TIM_ACTIVEINPUT_DIRECTTI;
	TIM_IC_InitStruct.ICPrescaler = LL_TIM_ICPSC_DIV1;
	TIM_IC_InitStruct.ICFilter = LL_TIM_IC_FILTER_FDIV1;
	LL_TIM_IC_Init(TIM1, LL_TIM_CHANNEL_CH2, &TIM_IC_InitStruct);

	LL_TIM_SetSlaveMode(TIM1, LL_TIM_SLAVEMODE_DISABLED);

	LL_TIM_SetTriggerInput(TIM1, LL_TIM_TS_TI2FP2);
	LL_TIM_SetSlaveMode(TIM1, LL_TIM_SLAVEMODE_RESET);
	LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH1 | LL_TIM_CHANNEL_CH2);
	LL_TIM_EnableCounter(TIM1);

}

#define PWM_MIN 3675
#define PWM_MAX 7480

//50Hz
#define PWM_PERIOD_MIN50 8250
#define PWM_PERIOD_MAX50 8400

//60Hz
#define PWM_PERIOD_MIN60 51000
#define PWM_PERIOD_MAX60 62000
//50Hz
//#define PWM_MIN 850
//#define PWM_MAX 4500

//60Hz
//#define PWM_MIN 54440
//#define PWM_MAX 58260

Result_t PWM_GetPWM(uint16_t *return_var)
{
	static uint64_t old_pwm_read_tick = 0;
	uint64_t tick = Systick_GetTick();
	if (tick - old_pwm_read_tick < 20)
		return OOF_NO_NEW_DATA;
	old_pwm_read_tick = tick;

	if (!LL_TIM_IsActiveFlag_CC2(TIM1))
		return OOF_NO_NEW_DATA;

	uint32_t period = LL_TIM_IC_GetCaptureCH2(TIM1);
	if (period > PWM_PERIOD_MAX50 || period < PWM_PERIOD_MIN50)
		if(period > PWM_PERIOD_MAX60 || period < PWM_PERIOD_MIN60)
			return OOF_NO_NEW_DATA;

	uint32_t position = period - LL_TIM_IC_GetCaptureCH1(TIM1);

	static double prev_pos = 0;
	if (position < PWM_MIN)
		position = PWM_MIN;
	if (position > PWM_MAX)
		position = PWM_MAX;
	position -= PWM_MIN;
	double pos = position;
	pos = ((double) UINT14_MAX * pos / (double) (PWM_MAX - PWM_MIN) + 0.5);
	pos = 0.2 * pos + 0.8 * prev_pos;
	pos = (pos > UINT14_MAX) ? UINT14_MAX : pos;
	pos = (pos < 0) ? 0 : pos;
	prev_pos = pos;
	*return_var = (uint16_t) pos<<2;
	return NOICE;

}