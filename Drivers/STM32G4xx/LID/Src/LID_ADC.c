#include "../Inc/LID_ADC.h"
#include <stddef.h>

const static uint32_t adcRanks[] =
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

const static uint32_t adcSeqRanks[] =
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

const static LID_ADC_AnalogPin_t gpioMapping[3][LID_ADC_CHANNEL_LAST] = {
		{
			[LID_ADC_CHANNEL_1] = { .port = GPIOA,	.pin = LL_GPIO_PIN_15,	.type = LID_ADC_INTYPE_REGULAR, .channel = LL_ADC_CHANNEL_1 },	//LID_ADC_SERVO_0_CURR
			[LID_ADC_CHANNEL_2] = { .port = GPIOA,	.pin = LL_GPIO_PIN_1,	.type = LID_ADC_INTYPE_REGULAR, .channel = LL_ADC_CHANNEL_2 },	//LID_ADC_SERVO_1_FDBK
			[LID_ADC_CHANNEL_3] = { .port = GPIOA,	.pin = LL_GPIO_PIN_2,	.type = LID_ADC_INTYPE_REGULAR, .channel = LL_ADC_CHANNEL_3 },	//LID_ADC_SERVO_1_CURR
			[LID_ADC_CHANNEL_4] = { .port = GPIOA,	.pin = LL_GPIO_PIN_3,	.type = LID_ADC_INTYPE_REGULAR, .channel = LL_ADC_CHANNEL_4 },	//LID_ADC_SERVO_2_FDBK
			[LID_ADC_CHANNEL_5] = { .port = GPIOB,	.pin = LL_GPIO_PIN_14,	.type = LID_ADC_INTYPE_REGULAR, .channel = LL_ADC_CHANNEL_5 },	//LID_ADC_PRESS_4
			[LID_ADC_CHANNEL_6] = { .port = GPIOC,	.pin = LL_GPIO_PIN_0,	.type = LID_ADC_INTYPE_REGULAR, .channel = LL_ADC_CHANNEL_6 },	//LID_ADC_TEMP_0
			[LID_ADC_CHANNEL_7] = { .port = GPIOC,	.pin = LL_GPIO_PIN_1,	.type = LID_ADC_INTYPE_REGULAR, .channel = LL_ADC_CHANNEL_7 },	//LID_ADC_TEMP_1
			[LID_ADC_CHANNEL_8] = { .port = GPIOC,	.pin = LL_GPIO_PIN_2,	.type = LID_ADC_INTYPE_REGULAR, .channel = LL_ADC_CHANNEL_8 },	//LID_ADC_TEMP_2
			[LID_ADC_CHANNEL_9] = { .port = GPIOC,	.pin = LL_GPIO_PIN_3,	.type = LID_ADC_INTYPE_REGULAR, .channel = LL_ADC_CHANNEL_9 },	//LID_ADC_SERVO_0_FDBK
			[LID_ADC_CHANNEL_11] = { .port = GPIOB,	.pin = LL_GPIO_PIN_12,	.type = LID_ADC_INTYPE_REGULAR, .channel = LL_ADC_CHANNEL_11 },	//LID_ADC_PRESS_5
			[LID_ADC_CHANNEL_12] = { .port = GPIOB,	.pin = LL_GPIO_PIN_1,	.type = LID_ADC_INTYPE_REGULAR, .channel = LL_ADC_CHANNEL_12 },	//LID_ADC_PYRO_SENSE
			[LID_ADC_CHANNEL_14] = { .port = GPIOB,	.pin = LL_GPIO_PIN_11,	.type = LID_ADC_INTYPE_REGULAR, .channel = LL_ADC_CHANNEL_14 }	//LID_ADC_PRESS_2
		},
		{
			[LID_ADC_CHANNEL_3] = { .port = GPIOA,	.pin = LL_GPIO_PIN_6,	.type = LID_ADC_INTYPE_REGULAR, .channel = LL_ADC_CHANNEL_3 },	//LID_ADC_12V_SENSE
			[LID_ADC_CHANNEL_5] = { .port = GPIOC,	.pin = LL_GPIO_PIN_4,	.type = LID_ADC_INTYPE_REGULAR, .channel = LL_ADC_CHANNEL_5 },	//LID_ADC_5V_SENSE
			[LID_ADC_CHANNEL_11] = { .port = GPIOC,	.pin = LL_GPIO_PIN_5,	.type = LID_ADC_INTYPE_REGULAR, .channel = LL_ADC_CHANNEL_11 },	//LID_ADC_12VA_SENSE
			[LID_ADC_CHANNEL_15] = { .port = GPIOB,	.pin = LL_GPIO_PIN_15,	.type = LID_ADC_INTYPE_REGULAR, .channel = LL_ADC_CHANNEL_15 },	//LID_ADC_PRESS_0
			[LID_ADC_CHANNEL_17] = { .port = GPIOA,	.pin = LL_GPIO_PIN_4,	.type = LID_ADC_INTYPE_REGULAR, .channel = LL_ADC_CHANNEL_17 },	//LID_ADC_SERVO_2_CURR
			[LID_ADC_CHANNEL_12] = { .port = GPIOA,	.pin = LL_GPIO_PIN_7,	.type = LID_ADC_INTYPE_REGULAR, .channel = LL_ADC_CHANNEL_12 },	//LID_ADC_SOLENOID_0_CURR
			[LID_ADC_CHANNEL_13] = { .port = GPIOB,	.pin = LL_GPIO_PIN_0,	.type = LID_ADC_INTYPE_REGULAR, .channel = LL_ADC_CHANNEL_13 }	//LID_ADC_SOLENOID_1_CURR
		},
		{
			[LID_ADC_CHANNEL_2] = { .port = GPIOE,	.pin = LL_GPIO_PIN_9,	.type = LID_ADC_INTYPE_REGULAR, .channel = LL_ADC_CHANNEL_2 },	//LID_ADC_PYRO_0_CURR
			[LID_ADC_CHANNEL_4] = { .port = GPIOE,	.pin = LL_GPIO_PIN_7,	.type = LID_ADC_INTYPE_REGULAR, .channel = LL_ADC_CHANNEL_4 },	//LID_ADC_PYRO_2_CURR
			[LID_ADC_CHANNEL_5] = { .port = GPIOB,	.pin = LL_GPIO_PIN_13,	.type = LID_ADC_INTYPE_REGULAR, .channel = LL_ADC_CHANNEL_5 },	//LID_ADC_PRESS_1
			[LID_ADC_CHANNEL_6] = { .port = GPIOE,	.pin = LL_GPIO_PIN_8,	.type = LID_ADC_INTYPE_REGULAR, .channel = LL_ADC_CHANNEL_6 },	//LID_ADC_PYRO_1_CURR
			[LID_ADC_CHANNEL_7] = { .port = GPIOD,	.pin = LL_GPIO_PIN_10,	.type = LID_ADC_INTYPE_REGULAR, .channel = LL_ADC_CHANNEL_7 }	//LID_ADC_PRESS_3
		}
};

static uint32_t adc1_length = 0;
static LID_ADC_Data_t adc1_data[18] =
{ 0 };

static uint32_t adc2_length = 0;
static LID_ADC_Data_t adc2_data[18] =
{ 0 };

static uint32_t adc3_length = 0;
static LID_ADC_Data_t adc3_data[18] =
{ 0 };

static volatile uint64_t LID_ADC_ChannelState = 0;

static void LID_ADC_RegInit(ADC_TypeDef *ADCx) {
	LL_ADC_InitTypeDef ADC_InitStruct =
	{ 0 };
	LL_ADC_REG_InitTypeDef ADC_REG_InitStruct =
	{ 0 };
	ADC_InitStruct.Resolution = LID_ADC_RESOLUTION;
	ADC_InitStruct.LowPowerMode = LL_ADC_LP_MODE_NONE;
	LL_ADC_Init(ADCx, &ADC_InitStruct);
	ADC_REG_InitStruct.TriggerSource = LL_ADC_REG_TRIG_SOFTWARE;
	ADC_REG_InitStruct.SequencerLength = LL_ADC_REG_SEQ_SCAN_DISABLE;
	ADC_REG_InitStruct.SequencerDiscont = DISABLE;
	ADC_REG_InitStruct.ContinuousMode = LL_ADC_REG_CONV_CONTINUOUS;
	//ADC_REG_InitStruct.ContinuousMode = LL_ADC_REG_CONV_SINGLE;
	ADC_REG_InitStruct.Overrun = LL_ADC_REG_OVR_DATA_OVERWRITTEN;
	ADC_REG_InitStruct.DMATransfer = LL_ADC_REG_DMA_TRANSFER_UNLIMITED;
	LL_ADC_REG_Init(ADCx, &ADC_REG_InitStruct);
	LL_ADC_SetOverSamplingScope(ADCx, LL_ADC_OVS_DISABLE);
	LL_ADC_DisableDeepPowerDown(ADCx);
	LL_ADC_EnableInternalRegulator(ADCx);
	//LL_ADC_EnableIT_EOS(adc);
}

static void LID_ADC_DmaInit(DMA_TypeDef * DMAx, uint32_t dmaStream, uint32_t dest, uint32_t src, uint32_t periph)
{
	LL_DMA_InitTypeDef DMA_InitStruct = {0};

	DMA_InitStruct.Direction = LL_DMA_DIRECTION_PERIPH_TO_MEMORY;
	DMA_InitStruct.MemoryOrM2MDstAddress = dest;
	DMA_InitStruct.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_HALFWORD;
	DMA_InitStruct.MemoryOrM2MDstIncMode = LL_DMA_MEMORY_INCREMENT;
	DMA_InitStruct.Mode = LL_DMA_MODE_CIRCULAR;
	DMA_InitStruct.NbData = 0;
	DMA_InitStruct.PeriphOrM2MSrcAddress = src;
	DMA_InitStruct.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_HALFWORD;
	DMA_InitStruct.PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT;
	DMA_InitStruct.PeriphRequest = periph;
	DMA_InitStruct.Priority = LID_ADC_DMA_PRIORITY;

	LL_DMA_Init(DMAx, dmaStream, &DMA_InitStruct);
}

static void LID_ADC_Calibrate()
{
	LL_ADC_StartCalibration(ADC1, LID_ADC_SINGLEDIFF);
	while (LL_ADC_IsCalibrationOnGoing(ADC1));

	LL_ADC_StartCalibration(ADC2, LID_ADC_SINGLEDIFF);
	while (LL_ADC_IsCalibrationOnGoing(ADC2));

	LL_ADC_StartCalibration(ADC3, LID_ADC_SINGLEDIFF);
	while (LL_ADC_IsCalibrationOnGoing(ADC3));
}

void LID_ADC_Init() {
	LL_RCC_SetADCClockSource(LL_RCC_ADC12_CLKSOURCE_SYSCLK);
	LL_RCC_SetADCClockSource(LL_RCC_ADC345_CLKSOURCE_SYSCLK);

	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_ADC12);
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_ADC345);

	LL_ADC_CommonInitTypeDef ADC_CommonInitStruct =
	{ 0 };

	LID_ADC_RegInit(ADC1);
	LID_ADC_RegInit(ADC2);
	LID_ADC_RegInit(ADC3);

	// Delay for ADC internal voltage regulator stabilization.
	uint32_t wait_loop_index;
	wait_loop_index = ((LL_ADC_DELAY_INTERNAL_REGUL_STAB_US * (SystemCoreClock / (100000 * 2))) / 10);
	while (wait_loop_index != 0)
	{
		wait_loop_index--;
	}

	ADC_CommonInitStruct.CommonClock = LID_ADC_COMMONCLOCK;
	ADC_CommonInitStruct.Multimode = LL_ADC_MULTI_INDEPENDENT;
	LL_ADC_CommonInit(__LL_ADC_COMMON_INSTANCE(ADC1), &ADC_CommonInitStruct);
	LL_ADC_CommonInit(__LL_ADC_COMMON_INSTANCE(ADC2), &ADC_CommonInitStruct);
	LL_ADC_CommonInit(__LL_ADC_COMMON_INSTANCE(ADC3), &ADC_CommonInitStruct);

	LID_ADC_Calibrate();

	//Init DMA for ADC123
	LID_ADC_DmaInit(LID_ADC_DMA, LID_ADC_DMA_CHANNEL, (uint32_t) adc1_data, LL_ADC_DMA_GetRegAddr(ADC1, LL_ADC_DMA_REG_REGULAR_DATA), LL_DMAMUX_REQ_ADC1);
	LID_ADC_DmaInit(LID_ADC_DMA, LID_ADC_DMA_CHANNEL+1, (uint32_t) adc2_data, LL_ADC_DMA_GetRegAddr(ADC2, LL_ADC_DMA_REG_REGULAR_DATA), LL_DMAMUX_REQ_ADC2);
	LID_ADC_DmaInit(LID_ADC_DMA, LID_ADC_DMA_CHANNEL+2, (uint32_t) adc3_data, LL_ADC_DMA_GetRegAddr(ADC3, LL_ADC_DMA_REG_REGULAR_DATA), LL_DMAMUX_REQ_ADC3);
}

LID_ADC_Data_t * LID_ADC_subscribeChannel(LID_ADC_Channel_t *channel, LID_ADC_Channel_Type_t UNUSED) {

	LID_ADC_AnalogPin_t analogPin;
	LID_ADC_Data_t *data_ptr = NULL;
	uint32_t *length_ptr = NULL;
	uint32_t dmaChannel = 0;
	uint64_t adcChannelMsk = 0;
	if(channel->ADCx == ADC1) {
		analogPin = gpioMapping[1][channel->channelId];
		data_ptr = adc1_data;
		length_ptr = &adc1_length;
		dmaChannel = LID_ADC_DMA_CHANNEL;
		adcChannelMsk = (1U) << (channel->channelId);
	} else if (channel->ADCx == ADC2) {
		analogPin = gpioMapping[2][channel->channelId];
		data_ptr = adc2_data;
		length_ptr = &adc2_length;
		dmaChannel = LID_ADC_DMA_CHANNEL+1;
		adcChannelMsk = (1U) << (LID_ADC_CHANNEL_LAST + channel->channelId);
	} else if (channel->ADCx == ADC2) {
		analogPin = gpioMapping[3][channel->channelId];
		data_ptr = adc3_data;
		length_ptr = &adc3_length;
		dmaChannel = LID_ADC_DMA_CHANNEL+2;
		adcChannelMsk = (1U) << (2*LID_ADC_CHANNEL_LAST + channel->channelId);
	} else {
		return NULL;
	}

	// wrong input type passed
	if(analogPin.type != channel->type) return NULL;

	// channel already initialized
	if(!(adcChannelMsk & LID_ADC_ChannelState)) return NULL;

	LID_ADC_ChannelState |= adcChannelMsk;

	// enabling gpio port necessary?
	//LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);

	LL_GPIO_InitTypeDef GPIO_InitStruct =
	{ 0 };

	GPIO_InitStruct.Pin = analogPin.pin;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(analogPin.port, &GPIO_InitStruct);

	LL_ADC_SetChannelSamplingTime(channel->ADCx, channel->channelId, LID_ADC_CHANNEL_SAMPLINGTIME);
	LL_ADC_SetChannelSingleDiff(channel->ADCx, channel->channelId, LID_ADC_SINGLEDIFF);
	//LL_ADC_SetChannelPreSelection(channel->ADCx, channel->channelId);


	LL_ADC_REG_SetSequencerRanks(channel->ADCx, adcRanks[*length_ptr], channel->channelId);
	LL_DMA_SetDataLength(LID_ADC_DMA, dmaChannel, *length_ptr);
	uint32_t length = *length_ptr;
	(*length_ptr)++;
	return &data_ptr[length];
}

void LID_ADC_run() {
	LL_ADC_REG_SetSequencerLength(ADC1, adcSeqRanks[adc1_length-1]);
	LL_ADC_REG_SetSequencerLength(ADC2, adcSeqRanks[adc2_length-1]);
	LL_ADC_REG_SetSequencerLength(ADC3, adcSeqRanks[adc3_length-1]);
	LL_ADC_Enable(ADC1);
	LL_ADC_Enable(ADC2);
	LL_ADC_Enable(ADC3);

	LL_DMA_EnableChannel(LID_ADC_DMA, LID_ADC_DMA_CHANNEL);
	LL_DMA_EnableChannel(LID_ADC_DMA, LID_ADC_DMA_CHANNEL+1);
	LL_DMA_EnableChannel(LID_ADC_DMA, LID_ADC_DMA_CHANNEL+2);

	LL_ADC_REG_StartConversion(ADC1);
	LL_ADC_REG_StartConversion(ADC2);
	LL_ADC_REG_StartConversion(ADC3);
}
