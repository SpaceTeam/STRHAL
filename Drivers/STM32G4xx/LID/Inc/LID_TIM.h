#ifndef LID_TIM_H
#define LID_TIM_H

#include <stdint.h>
#include <stm32g4xx.h>

#define LID_TIM_N_TIM 7
#define LID_TIM_N_TIM_CHANNELS 64

typedef enum {
	LID_TIM_TIM1 = 0,
	LID_TIM_TIM2 = 1,
	LID_TIM_TIM3 = 2,
	LID_TIM_TIM4 = 3,
	LID_TIM_TIM5 = 4,
} LID_TIM_TimerId_t;

typedef enum {
	/*TIM2 Channels */
	LID_TIM_TIM2_CH1_PA0,
	LID_TIM_TIM2_CH2_PA1,
	LID_TIM_TIM2_CH3_PA2,
	LID_TIM_TIM2_CH4_PA3,

	LID_TIM_TIM2_CH1_PA5,
	LID_TIM_TIM2_CH2_PB3,
	LID_TIM_TIM2_CH3_PB10,
	LID_TIM_TIM2_CH4_PB11,

	LID_TIM_TIM2_CH1_PA15,

	LID_TIM_TIM2_CH1_PD3,
	LID_TIM_TIM2_CH2_PD4,
	LID_TIM_TIM2_CH3_PD7,
	LID_TIM_TIM2_CH4_PD6,

	/*TIM3 Channels*/
	LID_TIM_TIM3_CH1_PA6,
	LID_TIM_TIM3_CH2_PA7,
	LID_TIM_TIM3_CH3_PB0,
	LID_TIM_TIM3_CH4_PB1,

	LID_TIM_TIM3_CH1_PB4,
	LID_TIM_TIM3_CH2_PB5,

	LID_TIM_TIM3_CH1_PC6,
	LID_TIM_TIM3_CH2_PC7,
	LID_TIM_TIM3_CH3_PC8,
	LID_TIM_TIM3_CH4_PC9,

	LID_TIM_TIM3_CH1_PE2,
	LID_TIM_TIM3_CH2_PE3,
	LID_TIM_TIM3_CH3_PE4,
	LID_TIM_TIM3_CH4_PE5,

	/*TIM4 Channels*/
	LID_TIM_TIM4_CH1_PB6,
	LID_TIM_TIM4_CH2_PB7,
	LID_TIM_TIM4_CH3_PB8,
	LID_TIM_TIM4_CH4_PB9,

	LID_TIM_TIM4_CH1_PD12,
	LID_TIM_TIM4_CH2_PD13,
	LID_TIM_TIM4_CH3_PD14,
	LID_TIM_TIM4_CH4_PD15,

} LID_TIM_ChannelId_t;


typedef enum {
	LID_TIM_PWM_CHANNELTYPE_SO, //single out channel
	LID_TIM_PWM_CHANNELTYPE_IN, //input measure duty cycle
} LID_TIM_PWM_ChannelType_t;


typedef struct {
	uint16_t freq;
	uint16_t duty;
} LID_TIM_PWM_Param_t;

typedef struct {
	LID_TIM_ChannelId_t channelId;
	LID_TIM_PWM_ChannelType_t type;
	LID_TIM_PWM_Param_t param;
} LID_TIM_PWM_Channel_t;


void LID_TIM_Init();

int LID_TIM_Timer_Init(LID_TIM_TimerId_t id, uint16_t cfreq);

int LID_TIM_PWM_AddChannel(LID_TIM_PWM_Channel_t *pwmChannel, LID_TIM_ChannelId_t channelId, LID_TIM_PWM_ChannelType_t pwmType);

void LID_TIM_PWM_Read(LID_TIM_PWM_Channel_t *pwmChannel, LID_TIM_PWM_Param_t *param);
uint16_t LID_TIM_PWM_SetFreq(LID_TIM_PWM_Channel_t *pwmChannel, uint16_t frequency);
uint16_t LID_TIM_PWM_SetDuty(LID_TIM_PWM_Channel_t *pwmChannel, uint16_t duty);
int LID_TIM_PWM_Enable(LID_TIM_PWM_Channel_t *pwmChannel, int enable);



#endif /*LID_TIM_H*/
