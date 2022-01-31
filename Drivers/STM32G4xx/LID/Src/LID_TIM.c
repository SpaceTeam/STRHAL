#include "../Inc/LID_TIM.h"

#include <stm32g4xx_ll_tim.h>
#include <stm32g4xx_ll_gpio.h>


typedef enum {
	LID_TIM_TIMER_TYPE_STD,
	LID_TIM_TIMER_TYPE_ADV
} LID_TIM_TimerType_t;

typedef struct {
	TIM_TypeDef *timx;
	uint16_t cfreq;
	uint8_t ch_n;
	LID_TIM_TimerType_t type;
} LID_TIM_Timer_t;

typedef struct {
	LID_TIM_Timer_t *tim;
	GPIO_TypeDef *port;
	uint32_t pin;
	uint32_t afn;
	uint32_t n;
} LID_TIM_Channel_t;


static LID_TIM_Timer_t _tims[LID_TIM_N_TIM] = {
	[LID_TIM_TIM1] = {
		.timx = TIM1,
		.type = LID_TIM_TIMER_TYPE_ADV,
		.ch_n = 4
	},
	[LID_TIM_TIM2] = {
		.timx = TIM2,
		.type = LID_TIM_TIMER_TYPE_ADV,
		.ch_n = 4
	},
	[LID_TIM_TIM3] = {
		.timx = TIM3,
		.type = LID_TIM_TIMER_TYPE_STD,
		.ch_n = 4
	},
	[LID_TIM_TIM4] = {
		.timx = TIM4,
		.type = LID_TIM_TIMER_TYPE_STD,
		.ch_n = 4
	},
};

static const LID_TIM_Channel_t _tim_channels[LID_TIM_N_TIM_CHANNELS] = {
	[LID_TIM_TIM2_CH1_PA0] = {
		.tim = &_tims[LID_TIM_TIM2],
		.port = GPIOA,
		.pin = LL_GPIO_PIN_0,
		.afn = LL_GPIO_AF_1
	},
	[LID_TIM_TIM2_CH2_PA1] = {
		.tim = &_tims[LID_TIM_TIM2],
		.port = GPIOA,
		.pin = LL_GPIO_PIN_1,
		.afn = LL_GPIO_AF_1
	},
	[LID_TIM_TIM2_CH3_PA2] = {
		.tim = &_tims[LID_TIM_TIM2],
		.port = GPIOA,
		.pin = LL_GPIO_PIN_2,
		.afn = LL_GPIO_AF_1
	},
	[LID_TIM_TIM2_CH4_PA3] = {
		.tim = &_tims[LID_TIM_TIM2],
		.port = GPIOA,
		.pin = LL_GPIO_PIN_3,
		.afn = LL_GPIO_AF_1
	},
	[LID_TIM_TIM2_CH1_PA5] = {
		.tim = &_tims[LID_TIM_TIM2],
		.port = GPIOA,
		.pin = LL_GPIO_PIN_5,
		.afn = LL_GPIO_AF_1
	},
	[LID_TIM_TIM2_CH2_PB3] = {
		.tim = &_tims[LID_TIM_TIM2],
		.port = GPIOB,
		.pin = LL_GPIO_PIN_3,
		.afn = LL_GPIO_AF_1
	},
	[LID_TIM_TIM2_CH3_PB10] = {
		.tim = &_tims[LID_TIM_TIM2],
		.port = GPIOB,
		.pin = LL_GPIO_PIN_10,
		.afn = LL_GPIO_AF_1
	},
	[LID_TIM_TIM2_CH4_PB11] = {
		.tim = &_tims[LID_TIM_TIM2],
		.port = GPIOB,
		.pin = LL_GPIO_PIN_11,
		.afn = LL_GPIO_AF_1
	},
	[LID_TIM_TIM2_CH1_PD3] = {
		.tim = &_tims[LID_TIM_TIM2],
		.port = GPIOB,
		.pin = LL_GPIO_PIN_11,
		.afn = LL_GPIO_AF_1
	},
	[LID_TIM_TIM2_CH2_PD4] = {
		.tim = &_tims[LID_TIM_TIM2],
		.port = GPIOD,
		.pin = LL_GPIO_PIN_4,
		.afn = LL_GPIO_AF_2
	},
	[LID_TIM_TIM2_CH3_PD7] = {
		.tim = &_tims[LID_TIM_TIM2],
		.port = GPIOD,
		.pin = LL_GPIO_PIN_7,
		.afn = LL_GPIO_AF_2
	},
	[LID_TIM_TIM2_CH4_PD6] = {
		.tim = &_tims[LID_TIM_TIM2],
		.port = GPIOD,
		.pin = LL_GPIO_PIN_6,
		.afn = LL_GPIO_AF_2
	},
	[LID_TIM_TIM2_CH1_PA15] = {
		.tim = &_tims[LID_TIM_TIM2],
		.port = GPIOA,
		.pin = LL_GPIO_PIN_15,
		.afn = LL_GPIO_AF_1
	},


	[LID_TIM_TIM3_CH1_PA6] = {
		.tim = &_tims[LID_TIM_TIM3],
		.port = GPIOA,
		.pin = LL_GPIO_PIN_6,
		.afn = LL_GPIO_AF_2,
	},
	[LID_TIM_TIM3_CH2_PA7] = {
		.tim = &_tims[LID_TIM_TIM3],
		.port = GPIOA,
		.pin = LL_GPIO_PIN_7,
		.afn = LL_GPIO_AF_2,
	},
	[LID_TIM_TIM3_CH3_PB0] = {
		.tim = &_tims[LID_TIM_TIM3],
		.port = GPIOB,
		.pin = LL_GPIO_PIN_0,
		.afn = LL_GPIO_AF_2,
	},
	[LID_TIM_TIM3_CH4_PB1] = {
		.tim = &_tims[LID_TIM_TIM3],
		.port = GPIOB,
		.pin = LL_GPIO_PIN_1,
		.afn = LL_GPIO_AF_2,
	},
	[LID_TIM_TIM3_CH1_PB4] = {
		.tim = &_tims[LID_TIM_TIM3],
		.port = GPIOB,
		.pin = LL_GPIO_PIN_4,
		.afn = LL_GPIO_AF_2,
	},
	[LID_TIM_TIM3_CH2_PB5] = {
		.tim = &_tims[LID_TIM_TIM3],
		.port = GPIOB,
		.pin = LL_GPIO_PIN_5,
		.afn = LL_GPIO_AF_2,
	},
	[LID_TIM_TIM3_CH1_PC6] = {
		.tim = &_tims[LID_TIM_TIM3],
		.port = GPIOC,
		.pin = LL_GPIO_PIN_6,
		.afn = LL_GPIO_AF_2,
	},
	[LID_TIM_TIM3_CH2_PC7] = {
		.tim = &_tims[LID_TIM_TIM3],
		.port = GPIOC,
		.pin = LL_GPIO_PIN_7,
		.afn = LL_GPIO_AF_2,
	},
	[LID_TIM_TIM3_CH3_PC8] = {
		.tim = &_tims[LID_TIM_TIM3],
		.port = GPIOC,
		.pin = LL_GPIO_PIN_8,
		.afn = LL_GPIO_AF_2,
	},
	[LID_TIM_TIM3_CH4_PC9] = {
		.tim = &_tims[LID_TIM_TIM3],
		.port = GPIOC,
		.pin = LL_GPIO_PIN_9,
		.afn = LL_GPIO_AF_2,
	},
	[LID_TIM_TIM3_CH1_PE2] = {
		.tim = &_tims[LID_TIM_TIM3],
		.port = GPIOE,
		.pin = LL_GPIO_PIN_2,
		.afn = LL_GPIO_AF_2,
	},
	[LID_TIM_TIM3_CH2_PE3] = {
		.tim = &_tims[LID_TIM_TIM3],
		.port = GPIOE,
		.pin = LL_GPIO_PIN_3,
		.afn = LL_GPIO_AF_2,
	},
	[LID_TIM_TIM3_CH3_PE4] = {
		.tim = &_tims[LID_TIM_TIM3],
		.port = GPIOE,
		.pin = LL_GPIO_PIN_4,
		.afn = LL_GPIO_AF_2,
	},
	[LID_TIM_TIM3_CH4_PE5] = {
		.tim = &_tims[LID_TIM_TIM3],
		.port = GPIOE,
		.pin = LL_GPIO_PIN_5,
		.afn = LL_GPIO_AF_2,
	},


	[LID_TIM_TIM4_CH1_PB6] = {
		.tim = &_tims[LID_TIM_TIM4],
		.port = GPIOB,
		.pin = LL_GPIO_PIN_6,
		.afn = LL_GPIO_AF_2
	},
	[LID_TIM_TIM4_CH2_PB7] = {
		.tim = &_tims[LID_TIM_TIM4],
		.port = GPIOB,
		.pin = LL_GPIO_PIN_7,
		.afn = LL_GPIO_AF_2
	},
	[LID_TIM_TIM4_CH3_PB8] = {
		.tim = &_tims[LID_TIM_TIM4],
		.port = GPIOB,
		.pin = LL_GPIO_PIN_8,
		.afn = LL_GPIO_AF_2
	},
	[LID_TIM_TIM4_CH4_PB9] = {
		.tim = &_tims[LID_TIM_TIM4],
		.port = GPIOB,
		.pin = LL_GPIO_PIN_9,
		.afn = LL_GPIO_AF_2
	},
	[LID_TIM_TIM4_CH1_PD12] = {
		.tim = &_tims[LID_TIM_TIM4],
		.port = GPIOD,
		.pin = LL_GPIO_PIN_12,
		.afn = LL_GPIO_AF_2
	},
	[LID_TIM_TIM4_CH2_PD13] = {
		.tim = &_tims[LID_TIM_TIM4],
		.port = GPIOD,
		.pin = LL_GPIO_PIN_13,
		.afn = LL_GPIO_AF_2
	},
	[LID_TIM_TIM4_CH3_PD14] = {
		.tim = &_tims[LID_TIM_TIM4],
		.port = GPIOD,
		.pin = LL_GPIO_PIN_14,
		.afn = LL_GPIO_AF_2
	},
	[LID_TIM_TIM4_CH4_PD15] = {
		.tim = &_tims[LID_TIM_TIM4],
		.port = GPIOD,
		.pin = LL_GPIO_PIN_15,
		.afn = LL_GPIO_AF_2
	},
};

void LID_TIM_Init() {

}

int LID_TIM_Timer_Init(LID_TIM_TimerId_t id, uint16_t cfreq) {
	if(id > LID_TIM_N_TIM || id < 0)
		return -1;

	LID_TIM_Timer_t *tim = &_tims[id];
	LL_TIM_InitTypeDef TIM_InitStruct = {0};
	TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
	TIM_InitStruct.CounterMode = LL_TIM_COUNTERDIRECTION_UP;
	TIM_InitStruct.Autoreload = 1000;
	TIM_InitStruct.Prescaler = 999;
	LL_TIM_Init(tim->timx, &TIM_InitStruct);
	tim->cfreq = cfreq;

	return 0;
}

int LID_TIM_PWM_AddChannel(LID_TIM_PWM_Channel_t *pwmChannel, LID_TIM_ChannelId_t channelId, LID_TIM_PWM_ChannelType_t pwmType) {
	const LID_TIM_Channel_t *ch = &_tim_channels[channelId];

	if(pwmType == LID_TIM_PWM_CHANNELTYPE_SO) {
		LL_TIM_OC_InitTypeDef OC_InitStruct = {0};
		OC_InitStruct.OCMode = LL_TIM_OCMODE_PWM1;
		OC_InitStruct.OCState = LL_TIM_OCSTATE_DISABLE;
		OC_InitStruct.OCPolarity = LL_TIM_OCPOLARITY_HIGH;
		OC_InitStruct.OCNPolarity = LL_TIM_OCPOLARITY_LOW;
		OC_InitStruct.OCIdleState = LL_TIM_OCIDLESTATE_LOW;
		OC_InitStruct.OCNIdleState = LL_TIM_OCIDLESTATE_HIGH;
		LL_TIM_OC_Init(ch->tim->timx, ch->n, &OC_InitStruct);
	}
	else if(pwmType == LID_TIM_PWM_CHANNELTYPE_IN) {
		LL_TIM_IC_InitTypeDef IC_InitStruct = {0};
	}


	return 0;
}

void LID_TIM_PWM_Read(LID_TIM_PWM_Channel_t *pwmChannel, LID_TIM_PWM_Param_t *param) {

}

uint16_t LID_TIM_PWM_SetFreq(LID_TIM_PWM_Channel_t *pwmChannel, uint16_t frequency) {
	return frequency;
}

uint16_t LID_TIM_PWM_SetDuty(LID_TIM_PWM_Channel_t *pwmChannel, uint16_t duty) {

	return duty;
}

int LID_TIM_PWM_Enable(LID_TIM_PWM_Channel_t *pwmChannel, int enable) {

	return enable;
}

