#include "dac.h"
#include "main.h"

void Dac_Init(void)
{


	  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};


	  LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOA);
	  GPIO_InitStruct.Pin = LL_GPIO_PIN_4;
	  GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
	  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_DAC12);
	  LL_DAC_InitTypeDef DAC_InitStruct = {0};

	  DAC_InitStruct.TriggerSource = LL_DAC_TRIG_SOFTWARE;
	  DAC_InitStruct.WaveAutoGeneration = LL_DAC_WAVE_AUTO_GENERATION_NONE;
	  DAC_InitStruct.OutputBuffer = LL_DAC_OUTPUT_BUFFER_ENABLE;
	  DAC_InitStruct.OutputConnection = LL_DAC_OUTPUT_CONNECT_GPIO;
	  DAC_InitStruct.OutputMode = LL_DAC_OUTPUT_MODE_NORMAL;
	  LL_DAC_Init(DAC1, LL_DAC_CHANNEL_1, &DAC_InitStruct);
	  LL_DAC_DisableTrigger(DAC1, LL_DAC_CHANNEL_1);

	  LL_DAC_Enable(DAC1, LL_DAC_CHANNEL_1);
}

void Dac_SetValue(uint16_t value)
{
	LL_DAC_ConvertData12RightAligned(DAC1, LL_DAC_CHANNEL_1, value);
}
