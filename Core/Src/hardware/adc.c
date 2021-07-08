#include "adc.h"
#include "main.h"
#include "tim.h"
#include "iob.h"

#include "serial.h"
#include <string.h>
#include <stdio.h>

volatile uint32_t adc1_samples_count = 0;
volatile uint32_t adc3_samples_count = 0;

static uint32_t adc1_length = 0;
static AdcData_t adc1_data[14] =
{ 0 };

static uint32_t adc3_length = 0;
static AdcData_t adc3_data[2] =
{ 0 };

static uint32_t adc_single_length = 0;
static ADC_CHANNEL_ID adc_single_id[14] =
{ 0 };
static AdcData_t adc_single_data[14] =
{ 0 };

volatile uint8_t adc_state = 0, adc1_state = 0, adc3_state = 0;

/*
 PC0   ------> Channel  0  | ADC1_INP10
 PC1   ------> Channel  1  | ADC1_INP11
 PC2   ------> Channel  2  | ADC3_INP0
 PC3   ------> Channel  3  | ADC3_INP1
 PC4   ------> Channel  4  | ADC1_INP4
 PC5   ------> Channel  5  | ADC1_INP8

 PA0   ------> Channel  6  | ADC1_INP16
 PA1   ------> Channel  7  | ADC1_INP17
 PA2   ------> Channel  8  | ADC1_INP14
 PA3   ------> Channel  9  | ADC1_INP15
 PA4   ------> Channel 10  | ADC1_INP18
 PA5   ------> Channel 11  | ADC1_INP19
 PA6   ------> Bus Volt 1  | ADC1_INP3
 PA7   ------> Bus Volt 2  | ADC1_INP7

 PB0   ------> Pwr Volt    | ADC1_INP9
 PB1   ------> Pwr Current | ADC1_INP5
 */

const static AdcChannel_t adc_channel_list[] =
{ [ANALOG_IN_1] =
{ ADC1, LL_ADC_CHANNEL_10 }, [ANALOG_IN_2] =
{ ADC1, LL_ADC_CHANNEL_11 }, [ANALOG_IN_3] =
{ ADC3, LL_ADC_CHANNEL_0 }, [ANALOG_IN_4] =
{ ADC3, LL_ADC_CHANNEL_1 }, [ANALOG_IN_5] =
{ ADC1, LL_ADC_CHANNEL_4 }, [ANALOG_IN_6] =
{ ADC1, LL_ADC_CHANNEL_8 }, [ANALOG_IN_7] =
{ ADC1, LL_ADC_CHANNEL_16 }, [ANALOG_IN_8] =
{ ADC1, LL_ADC_CHANNEL_17 }, [ANALOG_IN_9] =
{ ADC1, LL_ADC_CHANNEL_14 }, [ANALOG_IN_10] =
{ ADC1, LL_ADC_CHANNEL_15 }, [ANALOG_IN_11] =
{ ADC1, LL_ADC_CHANNEL_18 }, [ANALOG_IN_12] =
{ ADC1, LL_ADC_CHANNEL_19 }, [PWR_BUS_VOLTAGE_1] =
{ ADC1, LL_ADC_CHANNEL_3 }, [PWR_BUS_VOLTAGE_2] =
{ ADC1, LL_ADC_CHANNEL_7 }, [SUPPLY_VOLTAGE_SENSE] =
{ ADC1, LL_ADC_CHANNEL_9 }, [SUPPLY_CURRENT_SENSE] =
{ ADC1, LL_ADC_CHANNEL_5 } };

const static uint32_t adc_ranks[] =
{
LL_ADC_REG_RANK_1,
LL_ADC_REG_RANK_2,
LL_ADC_REG_RANK_3,
LL_ADC_REG_RANK_4,
LL_ADC_REG_RANK_5,
LL_ADC_REG_RANK_6,
LL_ADC_REG_RANK_7,
LL_ADC_REG_RANK_8,
LL_ADC_REG_RANK_9,
LL_ADC_REG_RANK_10,
LL_ADC_REG_RANK_11,
LL_ADC_REG_RANK_12,
LL_ADC_REG_RANK_13,
LL_ADC_REG_RANK_14,
LL_ADC_REG_RANK_15,
LL_ADC_REG_RANK_16 };

const static uint32_t adc_seq_ranks[] =
{
LL_ADC_REG_SEQ_SCAN_DISABLE,
LL_ADC_REG_SEQ_SCAN_ENABLE_2RANKS,
LL_ADC_REG_SEQ_SCAN_ENABLE_3RANKS,
LL_ADC_REG_SEQ_SCAN_ENABLE_4RANKS,
LL_ADC_REG_SEQ_SCAN_ENABLE_5RANKS,
LL_ADC_REG_SEQ_SCAN_ENABLE_6RANKS,
LL_ADC_REG_SEQ_SCAN_ENABLE_7RANKS,
LL_ADC_REG_SEQ_SCAN_ENABLE_8RANKS,
LL_ADC_REG_SEQ_SCAN_ENABLE_9RANKS,
LL_ADC_REG_SEQ_SCAN_ENABLE_10RANKS,
LL_ADC_REG_SEQ_SCAN_ENABLE_11RANKS,
LL_ADC_REG_SEQ_SCAN_ENABLE_12RANKS,
LL_ADC_REG_SEQ_SCAN_ENABLE_13RANKS,
LL_ADC_REG_SEQ_SCAN_ENABLE_14RANKS,
LL_ADC_REG_SEQ_SCAN_ENABLE_15RANKS,
LL_ADC_REG_SEQ_SCAN_ENABLE_16RANKS };

static void Adc_InitDMA(DMA_TypeDef * dma, uint32_t dma_stream, uint32_t dest, uint32_t src, uint32_t length)
{
	LL_DMA_SetDataTransferDirection(dma, dma_stream, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);
	LL_DMA_SetPeriphIncMode(dma, dma_stream, LL_DMA_PERIPH_NOINCREMENT);
	LL_DMA_SetMemoryIncMode(dma, dma_stream, LL_DMA_MEMORY_INCREMENT);
	LL_DMA_SetPeriphSize(dma, dma_stream, LL_DMA_PDATAALIGN_HALFWORD);
	LL_DMA_SetMemorySize(dma, dma_stream, LL_DMA_MDATAALIGN_HALFWORD);
	LL_DMA_SetPeriphAddress(dma, dma_stream, src);
	LL_DMA_SetMemoryAddress(dma, dma_stream, dest);
	LL_DMA_SetDataLength(dma, dma_stream, length);
	if(dma == DMA1)
	{
		LL_DMA_SetPeriphRequest(dma, dma_stream, LL_DMAMUX1_REQ_ADC1);
	}
	else
	{
		LL_DMA_SetPeriphRequest(dma, dma_stream, LL_DMAMUX1_REQ_ADC3);
	}
	LL_DMA_SetStreamPriorityLevel(dma, dma_stream, LL_DMA_PRIORITY_HIGH);
	LL_DMA_SetMode(dma, dma_stream, LL_DMA_MODE_CIRCULAR);
	LL_DMA_DisableFifoMode(dma, dma_stream);
	LL_DMA_EnableStream(dma, dma_stream);
}

static void Adc_SetupAdc(ADC_TypeDef *adc)
{
	LL_ADC_InitTypeDef ADC_InitStruct =
	{ 0 };
	LL_ADC_REG_InitTypeDef ADC_REG_InitStruct =
	{ 0 };
	ADC_InitStruct.Resolution = LL_ADC_RESOLUTION_16B;
	ADC_InitStruct.LowPowerMode = LL_ADC_LP_MODE_NONE;
	LL_ADC_Init(adc, &ADC_InitStruct);
	//ADC_REG_InitStruct.TriggerSource = LL_ADC_REG_TRIG_SOFTWARE;
	ADC_REG_InitStruct.TriggerSource = LL_ADC_REG_TRIG_EXT_TIM3_TRGO;
	ADC_REG_InitStruct.SequencerLength = LL_ADC_REG_SEQ_SCAN_DISABLE;
	ADC_REG_InitStruct.SequencerDiscont = DISABLE;
	//ADC_REG_InitStruct.ContinuousMode = LL_ADC_REG_CONV_CONTINUOUS;
	ADC_REG_InitStruct.ContinuousMode = LL_ADC_REG_CONV_SINGLE;
	ADC_REG_InitStruct.Overrun = LL_ADC_REG_OVR_DATA_OVERWRITTEN;
	ADC_REG_InitStruct.DataTransferMode = LL_ADC_REG_DMA_TRANSFER_UNLIMITED;
	LL_ADC_REG_Init(adc, &ADC_REG_InitStruct);
	LL_ADC_SetOverSamplingScope(adc, LL_ADC_OVS_DISABLE);
	LL_ADC_DisableDeepPowerDown(adc);
	LL_ADC_EnableInternalRegulator(adc);
	LL_ADC_EnableIT_EOS(adc);
}

static void Adc_SetupAdcChannel(ADC_CHANNEL_ID id)
{
	LL_ADC_SetChannelSamplingTime(adc_channel_list[id].adc, adc_channel_list[id].channel, LL_ADC_SAMPLINGTIME_387CYCLES_5);
	LL_ADC_SetChannelSingleDiff(adc_channel_list[id].adc, adc_channel_list[id].channel, LL_ADC_SINGLE_ENDED);
	LL_ADC_SetChannelPreSelection(adc_channel_list[id].adc, adc_channel_list[id].channel);
}

void printCounts()
{
	char serial_str[50] =
	{ 0 };

	sprintf(serial_str,"%ld %ld\n",adc1_samples_count, adc3_samples_count);
	Serial_PutString(serial_str);
}

void Adc_Init()
{
	LL_RCC_SetADCClockSource(LL_RCC_ADC_CLKSOURCE_PLL2P);

	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_ADC12);
	LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_ADC3);

	LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOA);
	LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOB);
	LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOC);

	LL_ADC_CommonInitTypeDef ADC_CommonInitStruct =
	{ 0 };
	LL_GPIO_InitTypeDef GPIO_InitStruct =
	{ 0 };

	GPIO_InitStruct.Pin = LL_GPIO_PIN_0 | LL_GPIO_PIN_1 | LL_GPIO_PIN_2 | LL_GPIO_PIN_3 | LL_GPIO_PIN_4 | LL_GPIO_PIN_5 | LL_GPIO_PIN_6 | LL_GPIO_PIN_7;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LL_GPIO_PIN_0 | LL_GPIO_PIN_1;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LL_GPIO_PIN_0 | LL_GPIO_PIN_1 | LL_GPIO_PIN_2 | LL_GPIO_PIN_3 | LL_GPIO_PIN_4 | LL_GPIO_PIN_5;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	Adc_SetupAdc(ADC1);
	Adc_SetupAdc(ADC3);
	TIM3_Init();

	// Delay for ADC internal voltage regulator stabilization.
	uint32_t wait_loop_index;
	wait_loop_index = ((LL_ADC_DELAY_INTERNAL_REGUL_STAB_US * (SystemCoreClock / (100000 * 2))) / 10);
	while (wait_loop_index != 0)
	{
		wait_loop_index--;
	}

	ADC_CommonInitStruct.CommonClock = LL_ADC_CLOCK_ASYNC_DIV16;
	ADC_CommonInitStruct.Multimode = LL_ADC_MULTI_INDEPENDENT;
	LL_ADC_CommonInit(__LL_ADC_COMMON_INSTANCE(ADC1), &ADC_CommonInitStruct);
	LL_ADC_CommonInit(__LL_ADC_COMMON_INSTANCE(ADC3), &ADC_CommonInitStruct);

	for(uint32_t c = 0; c < ADC_CHANNEL_ID_LAST; c++)
		Adc_SetupAdcChannel(c);
}

AdcData_t* Adc_AddRegularChannel(ADC_CHANNEL_ID id)
{
	AdcData_t *data_ptr = NULL;
	uint32_t *length_ptr = NULL;
	ADC_TypeDef *adc = adc_channel_list[id].adc;

	if (adc == ADC1)
	{
		data_ptr = adc1_data;
		length_ptr = &adc1_length;
	}
	else
	{
		data_ptr = adc3_data;
		length_ptr = &adc3_length;
	}

	LL_ADC_REG_SetSequencerRanks(adc, adc_ranks[*length_ptr], adc_channel_list[id].channel);
	uint32_t length = *length_ptr;
	(*length_ptr)++;
	return &data_ptr[length];
}

AdcSingleData_t* Adc_AddSingleChannel(ADC_CHANNEL_ID id)
{
	AdcSingleData_t *single_data = NULL;

	single_data->data = Adc_AddRegularChannel(id);
	single_data->last_measurement = &adc_single_data[adc_single_length];

	adc_single_id[adc_single_length] = id;
	adc_single_length++;

	return single_data;
}

// ADC has to be disabled!
//TODO: GM Is not in use
void Adc_Calibrate(void)
{
	LL_ADC_StartCalibration(ADC1, LL_ADC_CALIB_OFFSET_LINEARITY, LL_ADC_SINGLE_ENDED);
	while (LL_ADC_IsCalibrationOnGoing(ADC1));

	LL_ADC_StartCalibration(ADC3, LL_ADC_CALIB_OFFSET_LINEARITY, LL_ADC_SINGLE_ENDED);
	while (LL_ADC_IsCalibrationOnGoing(ADC3));
}

void Adc_StartAdc(void)
{
	NVIC_EnableIRQ(ADC_IRQn);
	NVIC_EnableIRQ(ADC3_IRQn);
	LL_ADC_REG_SetSequencerLength(ADC1, adc_seq_ranks[adc1_length-1]);
	LL_ADC_REG_SetSequencerLength(ADC3, adc_seq_ranks[adc3_length-1]);
	LL_ADC_Enable(ADC1);
	LL_ADC_Enable(ADC3);
	Adc_InitDMA(DMA1, LL_DMA_STREAM_0, (uint32_t) adc1_data, LL_ADC_DMA_GetRegAddr(ADC1, LL_ADC_DMA_REG_REGULAR_DATA), adc1_length);
	Adc_InitDMA(DMA2, LL_DMA_STREAM_0, (uint32_t) adc3_data, LL_ADC_DMA_GetRegAddr(ADC3, LL_ADC_DMA_REG_REGULAR_DATA), adc3_length);

	LL_TIM_EnableCounter(TIM3);
	LL_ADC_REG_StartConversion(ADC1);
}

void ADC_IRQHandler(void)
{
	if(LL_ADC_IsActiveFlag_EOS(ADC1) != 0)
	{
		adc1_samples_count++;
		LL_ADC_REG_StartConversion(ADC3);
		LL_ADC_ClearFlag_EOS(ADC1);
	}
}

void ADC3_IRQHandler(void)
{
	if(LL_ADC_IsActiveFlag_EOS(ADC3) != 0)
	{
		adc3_samples_count++;

		if(adc_state < adc_single_length)
		{
			// enable next single channel
			IOB_Pins_t next_single = iob_channels[adc_single_id[adc_state]];
			LL_GPIO_SetOutputPin(next_single.enable.port, next_single.enable.pin);
			// after first single channel measurement - disable previous channel
			if(adc_state > 0)
			{
				IOB_Pins_t prev_single = iob_channels[adc_single_id[adc_state-1]];
				LL_GPIO_ResetOutputPin(prev_single.enable.port, prev_single.enable.pin);

				// transfer last single measurement to corresponding measurement array
				ADC_CHANNEL_ID id = adc_single_id[adc_state-1];
				ADC_TypeDef *adc = adc_channel_list[id].adc;

				if (adc == ADC1)
				{
					adc_single_data[adc_state-1] = adc1_data[adc1_state];
					adc1_state++;
				}
				else
				{
					adc_single_data[adc_state-1] = adc3_data[adc3_state];
					adc3_state++;
				}
			}
			adc_state++;
		}
		else
		{
			// prepare for normal adc scan - make sure all single channels are off - disable last single channel
			IOB_Pins_t last_single = iob_channels[adc_single_id[adc_state]];
			LL_GPIO_ResetOutputPin(last_single.enable.port, last_single.enable.pin);
			// transfer last single measurement to corresponding measurement array
			ADC_CHANNEL_ID id = adc_single_id[adc_state-1];
			ADC_TypeDef *adc = adc_channel_list[id].adc;

			if (adc == ADC1)
			{
				adc_single_data[adc_state-1] = adc1_data[adc1_state];
			}
			else
			{
				adc_single_data[adc_state-1] = adc3_data[adc3_state];
			}
			adc_state = 0;
			adc1_state = 0;
			adc3_state = 0;
		}

		LL_ADC_ClearFlag_EOS(ADC3);
	}
}
