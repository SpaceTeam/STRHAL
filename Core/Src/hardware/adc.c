#include "adc.h"
#include "main.h"

volatile uint16_t data[14] =
{ 0 };

void Adc_Init()
{

	LL_RCC_SetADCClockSource(LL_RCC_ADC_CLKSOURCE_PLL2P);

	// DMA interrupt init
	// DMA1_Stream0_IRQn interrupt configuration
	NVIC_SetPriority(DMA1_Stream0_IRQn,
			NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
	NVIC_EnableIRQ(DMA1_Stream0_IRQn);
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

	//CLOCKs
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_ADC12);

	LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOA);
	LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOB);

	LL_ADC_InitTypeDef ADC_InitStruct =
	{ 0 };
	LL_ADC_REG_InitTypeDef ADC_REG_InitStruct =
	{ 0 };
	LL_ADC_CommonInitTypeDef ADC_CommonInitStruct =
	{ 0 };
	LL_GPIO_InitTypeDef GPIO_InitStruct =
	{ 0 };

	//GPIO Init
	/**ADC1 GPIO Configuration
	 PA6   ------> ADC1_INP3
	 PA7   ------> ADC1_INP7
	 PB0   ------> ADC1_INP9
	 PB1   ------> ADC1_INP5
	 */

	/**Node input GPIO Configuration
	 PC0   ------> Node 0 | ADC1_INP10
	 PC1   ------> Node 1 | ADC1_INP11
	       ------> Node 2
	       ------> Node 3
	 PC4   ------> Node 4 | ADC1_INP4
	 PC5   ------> Node 5 | ADC1_INP8
	 PA0   ------> Node 6 | ADC1_INP16
	 PA1   ------> Node 7 | ADC1_INP17
	 PA2   ------> Node 8 | ADC1_INP14
	 PA3   ------> Node 9 | ADC1_INP15
	 PA4   ------> Node 10 | ADC1_INP18
	 PA5   ------> Node 11 | ADC1_INP19
	 */
	GPIO_InitStruct.Pin = LL_GPIO_PIN_0 | LL_GPIO_PIN_1 | LL_GPIO_PIN_2 | LL_GPIO_PIN_3 | LL_GPIO_PIN_4 | LL_GPIO_PIN_5 | LL_GPIO_PIN_6 | LL_GPIO_PIN_7;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LL_GPIO_PIN_0 | LL_GPIO_PIN_1;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LL_GPIO_PIN_0 | LL_GPIO_PIN_1 | LL_GPIO_PIN_4 | LL_GPIO_PIN_5;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	//ADC
	ADC_InitStruct.Resolution = LL_ADC_RESOLUTION_16B;
	ADC_InitStruct.LowPowerMode = LL_ADC_LP_MODE_NONE;
	LL_ADC_Init(ADC1, &ADC_InitStruct);
	ADC_REG_InitStruct.TriggerSource = LL_ADC_REG_TRIG_SOFTWARE;
	ADC_REG_InitStruct.SequencerLength = LL_ADC_REG_SEQ_SCAN_ENABLE_14RANKS;
	ADC_REG_InitStruct.SequencerDiscont = DISABLE;
	ADC_REG_InitStruct.ContinuousMode = LL_ADC_REG_CONV_CONTINUOUS;
	ADC_REG_InitStruct.Overrun = LL_ADC_REG_OVR_DATA_OVERWRITTEN;
	ADC_REG_InitStruct.DataTransferMode = LL_ADC_REG_DMA_TRANSFER_UNLIMITED;
	LL_ADC_REG_Init(ADC1, &ADC_REG_InitStruct);

	LL_ADC_SetOverSamplingScope(ADC1, LL_ADC_OVS_DISABLE);

	LL_ADC_DisableDeepPowerDown(ADC1);
	LL_ADC_EnableInternalRegulator(ADC1);

	/* Delay for ADC internal voltage regulator stabilization. */
	uint32_t wait_loop_index;
	wait_loop_index = ((LL_ADC_DELAY_INTERNAL_REGUL_STAB_US
			* (SystemCoreClock / (100000 * 2))) / 10);
	while (wait_loop_index != 0)
	{
		wait_loop_index--;
	}

	ADC_CommonInitStruct.CommonClock = LL_ADC_CLOCK_ASYNC_DIV16;
	ADC_CommonInitStruct.Multimode = LL_ADC_MULTI_INDEPENDENT;
	LL_ADC_CommonInit(__LL_ADC_COMMON_INSTANCE(ADC1), &ADC_CommonInitStruct);

	// Configure Regular Channel Rank 1 Channel 3
	LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_3);
	LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_3,
	LL_ADC_SAMPLINGTIME_64CYCLES_5);
	LL_ADC_SetChannelSingleDiff(ADC1, LL_ADC_CHANNEL_3, LL_ADC_SINGLE_ENDED);
	LL_ADC_SetChannelPreSelection(ADC1, LL_ADC_CHANNEL_3);

	// Configure Regular Channel Rank 2 Channel 5
	LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_2, LL_ADC_CHANNEL_5);
	LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_5,
	LL_ADC_SAMPLINGTIME_64CYCLES_5);
	LL_ADC_SetChannelSingleDiff(ADC1, LL_ADC_CHANNEL_5, LL_ADC_SINGLE_ENDED);
	LL_ADC_SetChannelPreSelection(ADC1, LL_ADC_CHANNEL_5);

	// Configure Regular Channel Rank 3 Channel 7
	LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_3, LL_ADC_CHANNEL_7);
	LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_7,
	LL_ADC_SAMPLINGTIME_64CYCLES_5);
	LL_ADC_SetChannelSingleDiff(ADC1, LL_ADC_CHANNEL_7, LL_ADC_SINGLE_ENDED);
	LL_ADC_SetChannelPreSelection(ADC1, LL_ADC_CHANNEL_7);

	// Configure Regular Channel Rank 4 Channel 9
	LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_4, LL_ADC_CHANNEL_9);
	LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_9,
	LL_ADC_SAMPLINGTIME_64CYCLES_5);
	LL_ADC_SetChannelSingleDiff(ADC1, LL_ADC_CHANNEL_9, LL_ADC_SINGLE_ENDED);
	LL_ADC_SetChannelPreSelection(ADC1, LL_ADC_CHANNEL_9);

	// Configure Regular Channel Rank 5 Channel 10
	LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_5, LL_ADC_CHANNEL_10);
	LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_10,
	LL_ADC_SAMPLINGTIME_64CYCLES_5);
	LL_ADC_SetChannelSingleDiff(ADC1, LL_ADC_CHANNEL_10, LL_ADC_SINGLE_ENDED);
	LL_ADC_SetChannelPreSelection(ADC1, LL_ADC_CHANNEL_10);

	// Configure Regular Channel Rank 6 Channel 11
	LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_6, LL_ADC_CHANNEL_11);
	LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_11,
	LL_ADC_SAMPLINGTIME_64CYCLES_5);
	LL_ADC_SetChannelSingleDiff(ADC1, LL_ADC_CHANNEL_11, LL_ADC_SINGLE_ENDED);
	LL_ADC_SetChannelPreSelection(ADC1, LL_ADC_CHANNEL_11);

	// Configure Regular Channel Rank 7 Channel 4
	LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_7, LL_ADC_CHANNEL_4);
	LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_4,
	LL_ADC_SAMPLINGTIME_64CYCLES_5);
	LL_ADC_SetChannelSingleDiff(ADC1, LL_ADC_CHANNEL_4, LL_ADC_SINGLE_ENDED);
	LL_ADC_SetChannelPreSelection(ADC1, LL_ADC_CHANNEL_4);

	// Configure Regular Channel Rank 8 Channel 8
	LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_8, LL_ADC_CHANNEL_8);
	LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_8,
	LL_ADC_SAMPLINGTIME_64CYCLES_5);
	LL_ADC_SetChannelSingleDiff(ADC1, LL_ADC_CHANNEL_8, LL_ADC_SINGLE_ENDED);
	LL_ADC_SetChannelPreSelection(ADC1, LL_ADC_CHANNEL_8);

	// Configure Regular Channel Rank 9 Channel 16
	LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_9, LL_ADC_CHANNEL_16);
	LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_16,
	LL_ADC_SAMPLINGTIME_64CYCLES_5);
	LL_ADC_SetChannelSingleDiff(ADC1, LL_ADC_CHANNEL_16, LL_ADC_SINGLE_ENDED);
	LL_ADC_SetChannelPreSelection(ADC1, LL_ADC_CHANNEL_16);

	// Configure Regular Channel Rank 10 Channel 17
	LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_10, LL_ADC_CHANNEL_17);
	LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_17,
	LL_ADC_SAMPLINGTIME_64CYCLES_5);
	LL_ADC_SetChannelSingleDiff(ADC1, LL_ADC_CHANNEL_17, LL_ADC_SINGLE_ENDED);
	LL_ADC_SetChannelPreSelection(ADC1, LL_ADC_CHANNEL_17);

	// Configure Regular Channel Rank 11 Channel 14
	LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_11, LL_ADC_CHANNEL_14);
	LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_14,
	LL_ADC_SAMPLINGTIME_64CYCLES_5);
	LL_ADC_SetChannelSingleDiff(ADC1, LL_ADC_CHANNEL_14, LL_ADC_SINGLE_ENDED);
	LL_ADC_SetChannelPreSelection(ADC1, LL_ADC_CHANNEL_14);

	// Configure Regular Channel Rank 12 Channel 15
	LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_12, LL_ADC_CHANNEL_15);
	LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_15,
	LL_ADC_SAMPLINGTIME_64CYCLES_5);
	LL_ADC_SetChannelSingleDiff(ADC1, LL_ADC_CHANNEL_15, LL_ADC_SINGLE_ENDED);
	LL_ADC_SetChannelPreSelection(ADC1, LL_ADC_CHANNEL_15);

	// Configure Regular Channel Rank 13 Channel 18
	LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_13, LL_ADC_CHANNEL_18);
	LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_18,
	LL_ADC_SAMPLINGTIME_64CYCLES_5);
	LL_ADC_SetChannelSingleDiff(ADC1, LL_ADC_CHANNEL_18, LL_ADC_SINGLE_ENDED);
	LL_ADC_SetChannelPreSelection(ADC1, LL_ADC_CHANNEL_18);

	// Configure Regular Channel Rank 14 Channel 19
	LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_14, LL_ADC_CHANNEL_19);
	LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_19,
	LL_ADC_SAMPLINGTIME_64CYCLES_5);
	LL_ADC_SetChannelSingleDiff(ADC1, LL_ADC_CHANNEL_19, LL_ADC_SINGLE_ENDED);
	LL_ADC_SetChannelPreSelection(ADC1, LL_ADC_CHANNEL_19);

	LL_ADC_Enable(ADC1);

	//DMA Init
	LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_STREAM_0,
	LL_DMA_DIRECTION_PERIPH_TO_MEMORY);

	LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_STREAM_0, LL_DMA_PERIPH_NOINCREMENT);
	LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_STREAM_0, LL_DMA_MEMORY_INCREMENT);

	LL_DMA_SetPeriphSize(DMA1, LL_DMA_STREAM_0, LL_DMA_PDATAALIGN_HALFWORD);
	LL_DMA_SetMemorySize(DMA1, LL_DMA_STREAM_0, LL_DMA_MDATAALIGN_HALFWORD);

	LL_DMA_SetPeriphAddress(DMA1, LL_DMA_STREAM_0, LL_ADC_DMA_GetRegAddr(ADC1, LL_ADC_DMA_REG_REGULAR_DATA));
	LL_DMA_SetMemoryAddress(DMA1, LL_DMA_STREAM_0, (uint32_t) data);
	LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_0, 14);

	LL_DMA_SetPeriphRequest(DMA1, LL_DMA_STREAM_0, LL_DMAMUX1_REQ_ADC1);

	LL_DMA_SetStreamPriorityLevel(DMA1, LL_DMA_STREAM_0, LL_DMA_PRIORITY_HIGH);
	LL_DMA_SetMode(DMA1, LL_DMA_STREAM_0, LL_DMA_MODE_CIRCULAR);
	LL_DMA_DisableFifoMode(DMA1, LL_DMA_STREAM_0);

	LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_0);
	LL_ADC_REG_StartConversion(ADC1);

}

// ADC has to be disabled!
//TODO: GM Is not in use
void Adc_Calibrate(void)
{
	LL_ADC_StartCalibration(ADC1, LL_ADC_CALIB_OFFSET_LINEARITY,
	LL_ADC_SINGLE_ENDED);
	while (LL_ADC_IsCalibrationOnGoing(ADC1))
		;
}

volatile int32_t * Adc_GetData(uint8_t index)
{
	return &data[index];
}
