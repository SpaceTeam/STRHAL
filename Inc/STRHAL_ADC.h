#ifndef STRHAL_ADC_H
#define STRHAL_ADC_H

#include <stm32h7xx.h>
#include <stm32h7xx_ll_adc.h>
#include <stm32h7xx_ll_bus.h>
#include <stm32h7xx_ll_gpio.h>
#include <stm32h7xx_ll_dma.h>
#include <stm32h7xx_ll_dmamux.h>
#include <stm32h7xx_ll_rcc.h>
#include <stm32h7xx_ll_utils.h>

#ifdef __cplusplus
extern "C"
{
#endif

/***** DEFINES *****/
//ADC
//#define STRHAL_ADC_N_CHANNELS 19
//#define STRHAL_ADC_CHANNEL_SAMPLINGTIME LL_ADC_SAMPLINGTIME_12CYCLES_5
#define STRHAL_ADC_CHANNEL_SAMPLINGTIME LL_ADC_SAMPLINGTIME_387CYCLES_5
#define STRHAL_ADC_SINGLEDIFF LL_ADC_SINGLE_ENDED
#define STRHAL_ADC_RESOLUTION LL_ADC_RESOLUTION_16B

#define STRHAL_ADC_COMMONCLOCK LL_ADC_CLOCK_SYNC_PCLK_DIV4 //40 MHz
//DMA
#define STRHAL_ADC_DMA DMA1
#define STRHAL_ADC_DMA_STREAM LL_DMA_STREAM_3
#define STRHAL_ADC_DMA_PRIORITY LL_DMA_PRIORITY_VERYHIGH

typedef enum
{
	STRHAL_ADC_TYPE_SINGLE,
	STRHAL_ADC_TYPE_CONT
} STRHAL_ADC_Channel_Type_t;

typedef enum
{
	STRHAL_ADC_CHANNEL_0 = 0,
	STRHAL_ADC_CHANNEL_1,
	STRHAL_ADC_CHANNEL_2,
	STRHAL_ADC_CHANNEL_3,
	STRHAL_ADC_CHANNEL_4,
	STRHAL_ADC_CHANNEL_5,
	STRHAL_ADC_CHANNEL_6,
	STRHAL_ADC_CHANNEL_7,
	STRHAL_ADC_CHANNEL_8,
	STRHAL_ADC_CHANNEL_9,
	STRHAL_ADC_CHANNEL_10,
	STRHAL_ADC_CHANNEL_11,
	STRHAL_ADC_CHANNEL_12,
	STRHAL_ADC_CHANNEL_13,
	STRHAL_ADC_CHANNEL_14,
	STRHAL_ADC_CHANNEL_15,
	STRHAL_ADC_CHANNEL_16,
	STRHAL_ADC_CHANNEL_17,
	STRHAL_ADC_CHANNEL_18,
	STRHAL_ADC_CHANNEL_19,

	STRHAL_ADC_CHANNEL_LAST
} STRHAL_ADC_ChannelId_t;

typedef enum
{
	STRHAL_ADC_INTYPE_REGULAR //STRHAL_ADC_INTYPE_OPAMP not available for h7 series
} STRHAL_ADC_InType_t;

typedef struct
{
		GPIO_TypeDef *port;
		uint32_t pin;
		STRHAL_ADC_InType_t type;
		uint32_t channel;
} STRHAL_ADC_AnalogPin_t;

typedef struct
{
		ADC_TypeDef *ADCx;
		STRHAL_ADC_ChannelId_t channelId;
} STRHAL_ADC_Channel_t;

typedef volatile uint16_t STRHAL_ADC_Data_t;

void STRHAL_ADC_Init();

STRHAL_ADC_Data_t* STRHAL_ADC_SubscribeChannel(STRHAL_ADC_Channel_t *channel, STRHAL_ADC_InType_t type);

void STRHAL_ADC_Run();

#ifdef __cplusplus
}
#endif

#endif /* STM32H7XX_STRHAL_INC_STRHAL_ADC_H_ */
