#include "speaker.h"
#include "main.h"

typedef struct
{
	uint16_t tick_diff[2];
	uint8_t count;
	uint8_t state;
	uint8_t new_config;
} Speaker_t;

static Speaker_t speaker = {
		0 };

void Speaker_Init(void)
{
	LL_TIM_InitTypeDef TIM_InitStruct = {
			0 };
	LL_TIM_OC_InitTypeDef TIM_OC_InitStruct = {
			0 };
	LL_TIM_BDTR_InitTypeDef TIM_BDTRInitStruct = {
			0 };

	LL_GPIO_InitTypeDef GPIO_InitStruct = {
			0 };
	/* Peripheral clock enable */
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);

	TIM_InitStruct.Prescaler = 63;

	TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
	TIM_InitStruct.Autoreload = 0xFFFF;
	TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV2;
	TIM_InitStruct.RepetitionCounter = 0;
	LL_TIM_Init(TIM1, &TIM_InitStruct);

	LL_TIM_DisableARRPreload(TIM1);
	LL_TIM_SetClockSource(TIM1, LL_TIM_CLOCKSOURCE_INTERNAL);
	/*
	LL_TIM_OC_EnablePreload(TIM1, LL_TIM_CHANNEL_CH1);
	TIM_OC_InitStruct.OCMode = LL_TIM_OCMODE_PWM1;
	TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_DISABLE;
	TIM_OC_InitStruct.OCNState = LL_TIM_OCSTATE_DISABLE;
	TIM_OC_InitStruct.CompareValue = 5;
	TIM_OC_InitStruct.OCPolarity = LL_TIM_OCPOLARITY_HIGH;
	TIM_OC_InitStruct.OCNPolarity = LL_TIM_OCPOLARITY_HIGH;
	TIM_OC_InitStruct.OCIdleState = LL_TIM_OCIDLESTATE_LOW;
	TIM_OC_InitStruct.OCNIdleState = LL_TIM_OCIDLESTATE_LOW;
	LL_TIM_OC_Init(TIM1, LL_TIM_CHANNEL_CH1, &TIM_OC_InitStruct);
	LL_TIM_OC_DisableFast(TIM1, LL_TIM_CHANNEL_CH1);
	LL_TIM_SetTriggerOutput(TIM1, LL_TIM_TRGO_RESET);
	LL_TIM_SetTriggerOutput2(TIM1, LL_TIM_TRGO2_RESET);
	LL_TIM_DisableMasterSlaveMode(TIM1);
	TIM_BDTRInitStruct.OSSRState = LL_TIM_OSSR_DISABLE;
	TIM_BDTRInitStruct.OSSIState = LL_TIM_OSSI_DISABLE;
	TIM_BDTRInitStruct.LockLevel = LL_TIM_LOCKLEVEL_OFF;
	TIM_BDTRInitStruct.DeadTime = 0;
	TIM_BDTRInitStruct.BreakState = LL_TIM_BREAK_DISABLE;
	TIM_BDTRInitStruct.BreakPolarity = LL_TIM_BREAK_POLARITY_HIGH;
	TIM_BDTRInitStruct.BreakFilter = LL_TIM_BREAK_FILTER_FDIV1;
	TIM_BDTRInitStruct.Break2State = LL_TIM_BREAK2_DISABLE;
	TIM_BDTRInitStruct.Break2Polarity = LL_TIM_BREAK2_POLARITY_HIGH;
	TIM_BDTRInitStruct.Break2Filter = LL_TIM_BREAK2_FILTER_FDIV1;
	TIM_BDTRInitStruct.AutomaticOutput = LL_TIM_AUTOMATICOUTPUT_DISABLE;
	LL_TIM_BDTR_Init(TIM1, &TIM_BDTRInitStruct);

*/
	/*
	LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOE);
	 //TIM1 GPIO Configuration
	 //PE8     ------> TIM1_CH1N
	 //PE9     ------> TIM1_CH1

	GPIO_InitStruct.Pin = PIN_SPEAKER_N_Pin | PIN_SPEAKER_Pin;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
	LL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	LL_TIM_EnableAllOutputs(TIM1);
	*/
	LL_TIM_EnableCounter(TIM1);

}

void Speaker_Update(uint64_t tick)
{
	static uint64_t old_tick;

	if (((tick >= old_tick + speaker.tick_diff[speaker.state]) && speaker.count) || speaker.new_config)
	{
		old_tick = tick;
		speaker.new_config = 0;
		if (!speaker.state || speaker.tick_diff[speaker.state] == 0xFFFF)
		{
			LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH1);
			LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH1N);
			//DEBUG_LED_ON;

			speaker.state = 1;
		}
		else
		{
			//DEBUG_LED_OFF;

			LL_TIM_CC_DisableChannel(TIM1, LL_TIM_CHANNEL_CH1);
			LL_TIM_CC_DisableChannel(TIM1, LL_TIM_CHANNEL_CH1N);
			speaker.state = 0;

			if (speaker.count != 0xFF)
				speaker.count--;
		}
	}
}

void Speaker_SetPwmFrequency(uint16_t time)
{

	LL_TIM_CC_DisableChannel(TIM1, LL_TIM_CHANNEL_CH1);
	LL_TIM_CC_DisableChannel(TIM1, LL_TIM_CHANNEL_CH1N);

	uint32_t arr = 40000 / time;

	LL_TIM_SetAutoReload(TIM1, arr - 1);
	LL_TIM_OC_SetCompareCH1(TIM1, arr >> 1);

	//TODO SET PWM Frequency
	//LL_TIM_Enab

}

void Speaker_Set(uint16_t tone, uint16_t beep_on_period, uint16_t beep_off_period, uint8_t count)
{

	LL_TIM_CC_DisableChannel(TIM1, LL_TIM_CHANNEL_CH1);
	LL_TIM_CC_DisableChannel(TIM1, LL_TIM_CHANNEL_CH1N);
	Speaker_SetPwmFrequency(tone);

	speaker.tick_diff[0] = beep_off_period;
	speaker.tick_diff[1] = beep_on_period;
	speaker.count = count;
	speaker.state = 0;
	speaker.new_config = 1;

}

