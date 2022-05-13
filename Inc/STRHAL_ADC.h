#ifndef STRHAL_ADC_H
#define STRHAL_ADC_H

#include <stm32g4xx.h>
#include <stm32g4xx_ll_adc.h>
#include <stm32g4xx_ll_bus.h>
#include <stm32g4xx_ll_dma.h>
#include <stm32g4xx_ll_dmamux.h>
#include <stm32g4xx_ll_gpio.h>
#include <stm32g4xx_ll_rcc.h>
#include <stm32g4xx_ll_utils.h>


#ifdef __cplusplus
extern "C" {
#endif

/***** DEFINES *****/
//ADC
//#define STRHAL_ADC_N_CHANNELS 19
//#define STRHAL_ADC_CHANNEL_SAMPLINGTIME LL_ADC_SAMPLINGTIME_12CYCLES_5
#define STRHAL_ADC_CHANNEL_SAMPLINGTIME LL_ADC_SAMPLINGTIME_247CYCLES_5
#define STRHAL_ADC_SINGLEDIFF LL_ADC_SINGLE_ENDED
#define STRHAL_ADC_RESOLUTION LL_ADC_RESOLUTION_12B
//#define STRHAL_ADC_COMMONCLOCK LL_ADC_CLOCK_SYNC_PCLK_DIV2
#define STRHAL_ADC_COMMONCLOCK LL_ADC_CLOCK_SYNC_PCLK_DIV4
//DMA
#define STRHAL_ADC_DMA DMA1
#define STRHAL_ADC_DMA_CHANNEL LL_DMA_CHANNEL_3 // the 3 DMA channels start here
#define STRHAL_ADC_DMA_PRIORITY LL_DMA_PRIORITY_VERYHIGH

typedef enum {
	STRHAL_ADC_TYPE_SINGLE,
	STRHAL_ADC_TYPE_CONT
} STRHAL_ADC_Channel_Type_t;

typedef enum {
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

	STRHAL_ADC_CHANNEL_LAST
} STRHAL_ADC_ChanneSTRHAL_t;

typedef enum {
	STRHAL_ADC_INTYPE_REGULAR,
	STRHAL_ADC_INTYPE_OPAMP
} STRHAL_ADC_InType_t;

typedef struct {
	GPIO_TypeDef * port;
	uint32_t pin;
	STRHAL_ADC_InType_t type;
	uint32_t channel;
} STRHAL_ADC_AnalogPin_t;

typedef struct {
	ADC_TypeDef *ADCx;
	STRHAL_ADC_ChanneSTRHAL_t channeSTRHAL;
} STRHAL_ADC_Channel_t;

typedef volatile uint16_t STRHAL_ADC_Data_t;

void STRHAL_ADC_Init();

STRHAL_ADC_Data_t * STRHAL_ADC_SubscribeChannel(STRHAL_ADC_Channel_t *channel, STRHAL_ADC_InType_t type);

void STRHAL_ADC_Run();

/*
PRESS_0 PB15 ADC2_IN15 ADC2
PRESS_1 PB13 ADC3_IN5 ADC3
PRESS_2 PB11 ADC12_IN14 ADC1
PRESS_3 PD10 ADC3_IN7 ADC3
PRESS_4 PB14 ADC1_IN5 ADC1
PRESS_5 PB12 ADC1_IN11 ADC1

TEMP_0 PC0 ADC12_IN6 ADC1
TEMP_1 PC1 ADC12_IN7 ADC1
TEMP_2 PC2 ADC12_IN8 ADC1

SERVO_0_CURR PA0 ADC12_IN1 ADC1
SERVO_1_CURR PA2 ADC1_IN3 ADC1
SERVO_2_CURR PA4 ADC2_IN17 ADC2

SERVO_0_FDBK PC3 ADC1_IN9 ADC1
SERVO_1_FDBK PA1 ADC1_IN2 ADC1
SERVO_2_FDBK PA3 ADC1_IN4 ADC1

SOLENOID_0_CURR OPAMP PA7 OPAMP2_VINP ADC2_IN16 ADC2
SOLENOID_1_CURR OPAMP PB0 OPAMP3_VINP ADC2_IN18 ADC2

PYRO_0_CURR PE9 ADC3_IN2 ADC3
PYRO_1_CURR PE8 ADC3_IN6 ADC3
PYRO_2_CURR PE7 ADC3_IN4 ADC3

12V_SENSE PA6 ADC2_IN3 ADC2
12VA_SENSE PC5 ADC2_IN11 ADC2
5V_SENSE PC4 ADC2_IN5 ADC2
PYRO_SENSE PB1 ADC1_IN12 ADC1
*/

/*typedef enum {
	STRHAL_ADC_PRESS_0,
	STRHAL_ADC_PRESS_1,
	STRHAL_ADC_PRESS_2,
	STRHAL_ADC_PRESS_3,
	STRHAL_ADC_PRESS_4,
	STRHAL_ADC_PRESS_5,
	STRHAL_ADC_TEMP_0,
	STRHAL_ADC_TEMP_1,
	STRHAL_ADC_TEMP_2,
	STRHAL_ADC_SERVO_0_CURR,
	STRHAL_ADC_SERVO_1_CURR,
	STRHAL_ADC_SERVO_2_CURR,
	STRHAL_ADC_SERVO_0_FDBK,
	STRHAL_ADC_SERVO_1_FDBK,
	STRHAL_ADC_SERVO_2_FDBK,
	STRHAL_ADC_SOLENOID_0_CURR,
	STRHAL_ADC_SOLENOID_1_CURR,
	STRHAL_ADC_PYRO_0_CURR,
	STRHAL_ADC_PYRO_1_CURR,
	STRHAL_ADC_PYRO_2_CURR,
	STRHAL_ADC_12V_SENSE,
	STRHAL_ADC_12VA_SENSE,
	STRHAL_ADC_5V_SENSE,
	STRHAL_ADC_PYRO_SENSE,

	STRHAL_ADC_CHANNEL_LAST
} STRHAL_ADC_Channel_Id_t;*/

/*{
[STRHAL_ADC_SERVO_0_CURR] =	{ ADC1, LL_ADC_CHANNEL_1 },
[STRHAL_ADC_SERVO_1_FDBK] =	{ ADC1, LL_ADC_CHANNEL_2 },
[STRHAL_ADC_SERVO_1_CURR] =	{ ADC1, LL_ADC_CHANNEL_3 },
[STRHAL_ADC_SERVO_2_FDBK] =	{ ADC1, LL_ADC_CHANNEL_4 },
[STRHAL_ADC_PRESS_4] =			{ ADC1, LL_ADC_CHANNEL_5 },
[STRHAL_ADC_TEMP_0] =			{ ADC1, LL_ADC_CHANNEL_6 },
[STRHAL_ADC_TEMP_1] =			{ ADC1, LL_ADC_CHANNEL_7 },
[STRHAL_ADC_TEMP_2] =			{ ADC1, LL_ADC_CHANNEL_8 },
[STRHAL_ADC_SERVO_0_FDBK] =	{ ADC1, LL_ADC_CHANNEL_9 },
[STRHAL_ADC_PRESS_5] =			{ ADC1, LL_ADC_CHANNEL_11 },
[STRHAL_ADC_PYRO_SENSE] =		{ ADC1, LL_ADC_CHANNEL_12 }
[STRHAL_ADC_PRESS_2] =			{ ADC1, LL_ADC_CHANNEL_14 },

[STRHAL_ADC_12V_SENSE] =		{ ADC2, LL_ADC_CHANNEL_3 },
[STRHAL_ADC_5V_SENSE] =		{ ADC2, LL_ADC_CHANNEL_5 },
[STRHAL_ADC_12VA_SENSE] =		{ ADC2, LL_ADC_CHANNEL_11 },
[STRHAL_ADC_PRESS_0] =			{ ADC2, LL_ADC_CHANNEL_15 },
[STRHAL_ADC_SERVO_2_CURR] =	{ ADC2, LL_ADC_CHANNEL_17 },
[STRHAL_ADC_SOLENOID_0_CURR] =	{ ADC2, LL_ADC_CHANNEL_VOPAMP2 },
[STRHAL_ADC_SOLENOID_1_CURR] =	{ ADC2, LL_ADC_CHANNEL_VOPAMP3_ADC2 },

[STRHAL_ADC_PYRO_0_CURR] =		{ ADC3, LL_ADC_CHANNEL_2 },
[STRHAL_ADC_PYRO_2_CURR] =		{ ADC3, LL_ADC_CHANNEL_4 },
[STRHAL_ADC_PRESS_1] =			{ ADC3, LL_ADC_CHANNEL_5 },
[STRHAL_ADC_PYRO_1_CURR] =		{ ADC3, LL_ADC_CHANNEL_6 },
[STRHAL_ADC_PRESS_3] =			{ ADC3, LL_ADC_CHANNEL_7 },
};*/


#ifdef __cplusplus
}
#endif

#endif
