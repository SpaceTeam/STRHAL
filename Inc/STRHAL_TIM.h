#ifndef STRHAL_TIM_H
#define STRHAL_TIM_H

#include <stdint.h>
#include <stm32g4xx.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
	STRHAL_TIM_TIM1 = 0,
	STRHAL_TIM_TIM2 = 1,
	STRHAL_TIM_TIM3 = 2,
	STRHAL_TIM_TIM4 = 3,
	STRHAL_TIM_TIM8 = 4,

	STRHAL_TIM_N_TIM = 5
} STRHAL_TIM_TimerId_t;

typedef enum
{
	STRHAL_TIM_TIM6 = 0,
	STRHAL_TIM_TIM7 = 1,

	STRHAL_TIM_N_BASICTIM = 2
} STRHAL_TIM_BasicTimerId_t;

typedef enum
{
	/*TIM1 Channels */
	STRHAL_TIM_TIM1_CH1N_PA7,
	STRHAL_TIM_TIM1_CH1_PA8,
	STRHAL_TIM_TIM1_CH2_PA9,
	STRHAL_TIM_TIM1_CH3_PA10,
	STRHAL_TIM_TIM1_CH1N_PA11,
	STRHAL_TIM_TIM1_CH4_PA11,
	STRHAL_TIM_TIM1_CH2N_PA12,

	STRHAL_TIM_TIM1_CH2N_PB0,
	STRHAL_TIM_TIM1_CH3N_PB1,
	STRHAL_TIM_TIM1_CH3N_PB9,
	STRHAL_TIM_TIM1_CH1N_PB13,
	STRHAL_TIM_TIM1_CH2N_PB14,
	STRHAL_TIM_TIM1_CH3N_PB15,

	STRHAL_TIM_TIM1_CH1_PC0,
	STRHAL_TIM_TIM1_CH2_PC1,
	STRHAL_TIM_TIM1_CH3_PC2,
	STRHAL_TIM_TIM1_CH4_PC3,
	STRHAL_TIM_TIM1_CH4N_PC5,
	STRHAL_TIM_TIM1_CH1N_PC13,

	STRHAL_TIM_TIM1_CH1N_PE8,
	STRHAL_TIM_TIM1_CH1_PE9,
	STRHAL_TIM_TIM1_CH2N_PE10,
	STRHAL_TIM_TIM1_CH2_PE11,
	STRHAL_TIM_TIM1_CH3N_PE12,
	STRHAL_TIM_TIM1_CH3_PE13,
	STRHAL_TIM_TIM1_CH4_PE14,
	STRHAL_TIM_TIM1_CH4N_PE15,

	STRHAL_TIM_TIM1_CH3N_PF0,

	/*TIM2 Channels */
	STRHAL_TIM_TIM2_CH1_PA0,
	STRHAL_TIM_TIM2_CH2_PA1,
	STRHAL_TIM_TIM2_CH3_PA2,
	STRHAL_TIM_TIM2_CH4_PA3,
	STRHAL_TIM_TIM2_CH1_PA5,
	STRHAL_TIM_TIM2_CH3_PA9,
	STRHAL_TIM_TIM2_CH4_PA10,
	STRHAL_TIM_TIM2_CH1_PA15,

	STRHAL_TIM_TIM2_CH2_PB3,
	STRHAL_TIM_TIM2_CH3_PB10,
	STRHAL_TIM_TIM2_CH4_PB11,

	STRHAL_TIM_TIM2_CH1_PD3,
	STRHAL_TIM_TIM2_CH2_PD4,
	STRHAL_TIM_TIM2_CH4_PD6,
	STRHAL_TIM_TIM2_CH3_PD7,

	/*TIM3 Channels*/
	STRHAL_TIM_TIM3_CH2_PA4,
	STRHAL_TIM_TIM3_CH1_PA6,
	STRHAL_TIM_TIM3_CH2_PA7,

	STRHAL_TIM_TIM3_CH3_PB0,
	STRHAL_TIM_TIM3_CH4_PB1,
	STRHAL_TIM_TIM3_CH1_PB4,
	STRHAL_TIM_TIM3_CH2_PB5,
	STRHAL_TIM_TIM3_CH4_PB7,

	STRHAL_TIM_TIM3_CH1_PC6,
	STRHAL_TIM_TIM3_CH2_PC7,
	STRHAL_TIM_TIM3_CH3_PC8,
	STRHAL_TIM_TIM3_CH4_PC9,

	STRHAL_TIM_TIM3_CH1_PE2,
	STRHAL_TIM_TIM3_CH2_PE3,
	STRHAL_TIM_TIM3_CH3_PE4,
	STRHAL_TIM_TIM3_CH4_PE5,

	/*TIM4 Channels*/
	STRHAL_TIM_TIM4_CH1_PA11,
	STRHAL_TIM_TIM4_CH2_PA12,
	STRHAL_TIM_TIM4_CH3_PA13,

	STRHAL_TIM_TIM4_CH1_PB6,
	STRHAL_TIM_TIM4_CH2_PB7,
	STRHAL_TIM_TIM4_CH3_PB8,
	STRHAL_TIM_TIM4_CH4_PB9,

	STRHAL_TIM_TIM4_CH1_PD12,
	STRHAL_TIM_TIM4_CH2_PD13,
	STRHAL_TIM_TIM4_CH3_PD14,
	STRHAL_TIM_TIM4_CH4_PD15,

	/*TIM8 Channels*/
	STRHAL_TIM_TIM8_CH1N_PA7,
	STRHAL_TIM_TIM8_CH2_PA14,
	STRHAL_TIM_TIM8_CH1_PA15,

	STRHAL_TIM_TIM8_CH2N_PB0,
	STRHAL_TIM_TIM8_CH3N_PB1,
	STRHAL_TIM_TIM8_CH1N_PB3,
	STRHAL_TIM_TIM8_CH2N_PB4,
	STRHAL_TIM_TIM8_CH3N_PB5,
	STRHAL_TIM_TIM8_CH1_PB6,
	STRHAL_TIM_TIM8_CH2_PB8,
	STRHAL_TIM_TIM8_CH3_PB9,

	STRHAL_TIM_TIM8_CH1_PC6,
	STRHAL_TIM_TIM8_CH2_PC7,
	STRHAL_TIM_TIM8_CH3_PC8,
	STRHAL_TIM_TIM8_CH4_PC9,
	STRHAL_TIM_TIM8_CH1N_PC10,
	STRHAL_TIM_TIM8_CH2N_PC11,
	STRHAL_TIM_TIM8_CH3N_PC12,
	STRHAL_TIM_TIM8_CH4N_PC13,

	STRHAL_TIM_TIM8_CH4N_PD0,
	STRHAL_TIM_TIM8_CH4_PD1,

	STRHAL_TIM_N_TIM_CHANNELS
} STRHAL_TIM_ChannelId_t;

typedef enum
{
	STRHAL_TIM_PWM_CHANNELTYPE_SO, //single out channel
	STRHAL_TIM_PWM_CHANNELTYPE_IN, //input measure duty cycle
} STRHAL_TIM_PWM_ChannelType_t;

typedef struct
{
		STRHAL_TIM_ChannelId_t channelId;
		STRHAL_TIM_PWM_ChannelType_t type;
} STRHAL_TIM_PWM_Channel_t;

void STRHAL_TIM_Init();
void STRHAL_TIM_Run();

int32_t STRHAL_TIM_PWM_Init(STRHAL_TIM_TimerId_t id, uint16_t psc, uint16_t res);
int STRHAL_TIM_PWM_AddChannel(STRHAL_TIM_PWM_Channel_t *pwmChannel, STRHAL_TIM_ChannelId_t channelId, STRHAL_TIM_PWM_ChannelType_t pwmType);
int32_t STRHAL_TIM_PWM_Read(STRHAL_TIM_PWM_Channel_t *pwmChannel, uint16_t *duty);
int32_t STRHAL_TIM_PWM_SetFreq(STRHAL_TIM_TimerId_t id, uint16_t psc, uint16_t res);
int32_t STRHAL_TIM_PWM_SetDuty(STRHAL_TIM_PWM_Channel_t *pwmChannel, uint16_t duty);
int STRHAL_TIM_PWM_Enable(STRHAL_TIM_PWM_Channel_t *pwmChannel, int enable);

typedef void (*STRHAL_TIM_Burnable_t)();

int32_t STRHAL_TIM_Heartbeat_Init(STRHAL_TIM_BasicTimerId_t id, uint16_t psc, uint16_t res);
int STRHAL_TIM_Heartbeat_Subscribe(STRHAL_TIM_BasicTimerId_t id, STRHAL_TIM_Burnable_t burn);
int STRHAL_TIM_Heartbeat_StartHeartbeat(STRHAL_TIM_BasicTimerId_t id);

#ifdef __cplusplus
}
#endif

#endif /*STRHAL_TIM_H*/
