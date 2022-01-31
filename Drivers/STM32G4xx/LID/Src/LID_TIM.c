#include "../Inc/LID_TIM.h"

#include <stm32g4xx_ll_bus.h>
#include <stm32g4xx_ll_tim.h>
#include <stm32g4xx_ll_gpio.h>


typedef enum {
	LID_TIM_TIMER_TYPE_STD,
	LID_TIM_TIMER_TYPE_ADV
} LID_TIM_TimerType_t;

typedef enum {
	LID_TIM_USAGE_000 = 0x00,
	LID_TIM_USAGE_BAS,
	LID_TIM_USAGE_PWM,
} LID_TIM_Usage_t;

typedef struct {
	TIM_TypeDef *timx;
	uint16_t cfreq;
	uint8_t ch_n;
	LID_TIM_TimerType_t type;
	LID_TIM_Usage_t utype;
} LID_TIM_Timer_t;

typedef struct {
	LID_TIM_Timer_t *tim;
	GPIO_TypeDef *port;
	uint32_t pin;
	uint32_t afn;
	uint32_t n;
	__IO uint32_t *ccr;
} LID_TIM_Channel_t;


static LID_TIM_Timer_t _tims[LID_TIM_N_TIM] = {
	[LID_TIM_TIM1] = {
		.timx = TIM1,
		.type = LID_TIM_TIMER_TYPE_ADV,
		.ch_n = 4,
		.utype = LID_TIM_USAGE_000,
	},
	[LID_TIM_TIM2] = {
		.timx = TIM2,
		.type = LID_TIM_TIMER_TYPE_ADV,
		.ch_n = 4,
		.utype = LID_TIM_USAGE_000,
	},
	[LID_TIM_TIM3] = {
		.timx = TIM3,
		.type = LID_TIM_TIMER_TYPE_STD,
		.ch_n = 4,
		.utype = LID_TIM_USAGE_000,
	},
	[LID_TIM_TIM4] = {
		.timx = TIM4,
		.type = LID_TIM_TIMER_TYPE_STD,
		.ch_n = 4,
		.utype = LID_TIM_USAGE_000,
	},
	[LID_TIM_TIM8] = {
		.timx = TIM8,
		.type = LID_TIM_TIMER_TYPE_ADV,
		.ch_n = 4,
		.utype = LID_TIM_USAGE_000,
	},
};

static const LID_TIM_Channel_t _tim_channels[LID_TIM_N_TIM_CHANNELS] = {
	/*TIM 1 channels*/
	[LID_TIM_TIM1_CH1N_PA7] = {
		.tim = &_tims[LID_TIM_TIM1],
		.port = GPIOA,
		.pin = LL_GPIO_PIN_7,
		.afn = LL_GPIO_AF_6,
		.n = LL_TIM_CHANNEL_CH1N,
		.ccr = &TIM1->CCR1,
	},
	[LID_TIM_TIM1_CH1_PA8] = {
		.tim = &_tims[LID_TIM_TIM1],
		.port = GPIOA,
		.pin = LL_GPIO_PIN_8,
		.afn = LL_GPIO_AF_6,
		.n = LL_TIM_CHANNEL_CH1,
		.ccr = &TIM1->CCR1,
	},
	[LID_TIM_TIM1_CH2_PA9] = {
		.tim = &_tims[LID_TIM_TIM1],
		.port = GPIOA,
		.pin = LL_GPIO_PIN_9,
		.afn = LL_GPIO_AF_6,
		.n = LL_TIM_CHANNEL_CH2,
		.ccr = &TIM1->CCR2,
	},
	[LID_TIM_TIM1_CH3_PA10] = {
		.tim = &_tims[LID_TIM_TIM1],
		.port = GPIOA,
		.pin = LL_GPIO_PIN_10,
		.afn = LL_GPIO_AF_6,
		.n = LL_TIM_CHANNEL_CH3,
		.ccr = &TIM1->CCR3,
	},
	[LID_TIM_TIM1_CH1N_PA11] = {
		.tim = &_tims[LID_TIM_TIM1],
		.port = GPIOA,
		.pin = LL_GPIO_PIN_11,
		.afn = LL_GPIO_AF_6,
		.n = LL_TIM_CHANNEL_CH1N,
		.ccr = &TIM1->CCR1,
	},
	[LID_TIM_TIM1_CH4_PA11] = {
		.tim = &_tims[LID_TIM_TIM1],
		.port = GPIOA,
		.pin = LL_GPIO_PIN_11,
		.afn = LL_GPIO_AF_11,
		.n = LL_TIM_CHANNEL_CH4,
		.ccr = &TIM1->CCR4,
	},
	[LID_TIM_TIM1_CH2N_PA12] = {
		.tim = &_tims[LID_TIM_TIM1],
		.port = GPIOA,
		.pin = LL_GPIO_PIN_12,
		.afn = LL_GPIO_AF_6,
		.n = LL_TIM_CHANNEL_CH2N,
		.ccr = &TIM1->CCR2,
	},
	[LID_TIM_TIM1_CH2N_PB0] = {
		.tim = &_tims[LID_TIM_TIM1],
		.port = GPIOB,
		.pin = LL_GPIO_PIN_0,
		.afn = LL_GPIO_AF_6,
		.n = LL_TIM_CHANNEL_CH2N,
		.ccr = &TIM1->CCR2,
	},
	[LID_TIM_TIM1_CH3N_PB1] = {
		.tim = &_tims[LID_TIM_TIM1],
		.port = GPIOB,
		.pin = LL_GPIO_PIN_1,
		.afn = LL_GPIO_AF_6,
		.n = LL_TIM_CHANNEL_CH3N,
		.ccr = &TIM1->CCR3,
	},
	[LID_TIM_TIM1_CH3N_PB9] = {
		.tim = &_tims[LID_TIM_TIM1],
		.port = GPIOB,
		.pin = LL_GPIO_PIN_9,
		.afn = LL_GPIO_AF_12,
		.n = LL_TIM_CHANNEL_CH3N,
		.ccr = &TIM1->CCR3,
	},
	[LID_TIM_TIM1_CH1N_PB13] = {
		.tim = &_tims[LID_TIM_TIM1],
		.port = GPIOB,
		.pin = LL_GPIO_PIN_13,
		.afn = LL_GPIO_AF_6,
		.n = LL_TIM_CHANNEL_CH1N,
		.ccr = &TIM1->CCR1,
	},
	[LID_TIM_TIM1_CH2N_PB14] = {
		.tim = &_tims[LID_TIM_TIM1],
		.port = GPIOB,
		.pin = LL_GPIO_PIN_14,
		.afn = LL_GPIO_AF_6,
		.n = LL_TIM_CHANNEL_CH2N,
		.ccr = &TIM1->CCR2,
	},
	[LID_TIM_TIM1_CH3N_PB15] = {
		.tim = &_tims[LID_TIM_TIM1],
		.port = GPIOB,
		.pin = LL_GPIO_PIN_15,
		.afn = LL_GPIO_AF_4,
		.n = LL_TIM_CHANNEL_CH3N,
		.ccr = &TIM1->CCR3,
	},
	[LID_TIM_TIM1_CH1_PC0] = {
		.tim = &_tims[LID_TIM_TIM1],
		.port = GPIOC,
		.pin = LL_GPIO_PIN_0,
		.afn = LL_GPIO_AF_2,
		.n = LL_TIM_CHANNEL_CH1,
		.ccr = &TIM1->CCR1,
	},
	[LID_TIM_TIM1_CH2_PC1] = {
		.tim = &_tims[LID_TIM_TIM1],
		.port = GPIOC,
		.pin = LL_GPIO_PIN_1,
		.afn = LL_GPIO_AF_2,
		.n = LL_TIM_CHANNEL_CH2,
		.ccr = &TIM1->CCR2,
	},
	[LID_TIM_TIM1_CH3_PC2] = {
		.tim = &_tims[LID_TIM_TIM1],
		.port = GPIOC,
		.pin = LL_GPIO_PIN_2,
		.afn = LL_GPIO_AF_2,
		.n = LL_TIM_CHANNEL_CH3,
		.ccr = &TIM1->CCR3,
	},
	[LID_TIM_TIM1_CH4_PC3] = {
		.tim = &_tims[LID_TIM_TIM1],
		.port = GPIOC,
		.pin = LL_GPIO_PIN_3,
		.afn = LL_GPIO_AF_2,
		.n = LL_TIM_CHANNEL_CH4,
		.ccr = &TIM1->CCR4,
	},
	[LID_TIM_TIM1_CH4N_PC5] = {
		.tim = &_tims[LID_TIM_TIM1],
		.port = GPIOC,
		.pin = LL_GPIO_PIN_5,
		.afn = LL_GPIO_AF_6,
		.ccr = &TIM1->CCR4,
	},
	[LID_TIM_TIM1_CH1N_PC13] = {
		.tim = &_tims[LID_TIM_TIM1],
		.port = GPIOC,
		.pin = LL_GPIO_PIN_13,
		.afn = LL_GPIO_AF_4,
		.n = LL_TIM_CHANNEL_CH1N,
		.ccr = &TIM1->CCR1,
	},
	[LID_TIM_TIM1_CH1N_PE8] = {
		.tim = &_tims[LID_TIM_TIM1],
		.port = GPIOE,
		.pin = LL_GPIO_PIN_8,
		.afn = LL_GPIO_AF_2,
		.n = LL_TIM_CHANNEL_CH1N,
		.ccr = &TIM1->CCR1,
	},
	[LID_TIM_TIM1_CH1_PE9] = {
		.tim = &_tims[LID_TIM_TIM1],
		.port = GPIOE,
		.pin = LL_GPIO_PIN_9,
		.afn = LL_GPIO_AF_2,
		.n = LL_TIM_CHANNEL_CH1,
		.ccr = &TIM1->CCR1,
	},
	[LID_TIM_TIM1_CH2N_PE10] = {
		.tim = &_tims[LID_TIM_TIM1],
		.port = GPIOE,
		.pin = LL_GPIO_PIN_10,
		.afn = LL_GPIO_AF_2,
		.n = LL_TIM_CHANNEL_CH2N,
		.ccr = &TIM1->CCR2,
	},
	[LID_TIM_TIM1_CH2_PE11] = {
		.tim = &_tims[LID_TIM_TIM1],
		.port = GPIOE,
		.pin = LL_GPIO_PIN_11,
		.afn = LL_GPIO_AF_2,
		.n = LL_TIM_CHANNEL_CH2,
		.ccr = &TIM1->CCR2,
	},
	[LID_TIM_TIM1_CH3N_PE12] = {
		.tim = &_tims[LID_TIM_TIM1],
		.port = GPIOE,
		.pin = LL_GPIO_PIN_12,
		.afn = LL_GPIO_AF_2,
		.n = LL_TIM_CHANNEL_CH3N,
		.ccr = &TIM1->CCR3,
	},
	[LID_TIM_TIM1_CH3_PE13] = {
		.tim = &_tims[LID_TIM_TIM1],
		.port = GPIOE,
		.pin = LL_GPIO_PIN_13,
		.afn = LL_GPIO_AF_2,
		.n = LL_TIM_CHANNEL_CH3,
		.ccr = &TIM1->CCR3,
	},
	[LID_TIM_TIM1_CH4_PE14] = {
		.tim = &_tims[LID_TIM_TIM1],
		.port = GPIOE,
		.pin = LL_GPIO_PIN_14,
		.afn = LL_GPIO_AF_2,
		.n = LL_TIM_CHANNEL_CH4,
		.ccr = &TIM1->CCR4,
	},
	[LID_TIM_TIM1_CH4N_PE15] = {
		.tim = &_tims[LID_TIM_TIM1],
		.port = GPIOE,
		.pin = LL_GPIO_PIN_15,
		.afn = LL_GPIO_AF_6,
		.n = LL_TIM_CHANNEL_CH4N,
	},
	[LID_TIM_TIM1_CH3N_PF0] = {
		.tim = &_tims[LID_TIM_TIM1],
		.port = GPIOF,
		.pin = LL_GPIO_PIN_0,
		.afn = LL_GPIO_AF_6,
		.n = LL_TIM_CHANNEL_CH3N,
		.ccr = &TIM1->CCR3,
	},


	/*TIM 2 channels*/
	[LID_TIM_TIM2_CH1_PA0] = {
		.tim = &_tims[LID_TIM_TIM2],
		.port = GPIOA,
		.pin = LL_GPIO_PIN_0,
		.afn = LL_GPIO_AF_1,
		.n = LL_TIM_CHANNEL_CH1,
		.ccr = &TIM2->CCR1,
	},
	[LID_TIM_TIM2_CH2_PA1] = {
		.tim = &_tims[LID_TIM_TIM2],
		.port = GPIOA,
		.pin = LL_GPIO_PIN_1,
		.afn = LL_GPIO_AF_1,
		.n = LL_TIM_CHANNEL_CH2,
		.ccr = &TIM2->CCR2,
	},
	[LID_TIM_TIM2_CH3_PA2] = {
		.tim = &_tims[LID_TIM_TIM2],
		.port = GPIOA,
		.pin = LL_GPIO_PIN_2,
		.afn = LL_GPIO_AF_1,
		.n = LL_TIM_CHANNEL_CH3,
		.ccr = &TIM2->CCR3,
	},
	[LID_TIM_TIM2_CH4_PA3] = {
		.tim = &_tims[LID_TIM_TIM2],
		.port = GPIOA,
		.pin = LL_GPIO_PIN_3,
		.afn = LL_GPIO_AF_1,
		.n = LL_TIM_CHANNEL_CH4,
		.ccr = &TIM2->CCR4,
	},
	[LID_TIM_TIM2_CH1_PA5] = {
		.tim = &_tims[LID_TIM_TIM2],
		.port = GPIOA,
		.pin = LL_GPIO_PIN_5,
		.afn = LL_GPIO_AF_1,
		.n = LL_TIM_CHANNEL_CH1,
		.ccr = &TIM2->CCR1,
	},
	[LID_TIM_TIM2_CH3_PA9] = {
		.tim = &_tims[LID_TIM_TIM2],
		.port = GPIOA,
		.pin = LL_GPIO_PIN_9,
		.afn = LL_GPIO_AF_10,
		.n = LL_TIM_CHANNEL_CH3,
		.ccr = &TIM2->CCR3,
	},
	[LID_TIM_TIM2_CH4_PA10] = {
		.tim = &_tims[LID_TIM_TIM2],
		.port = GPIOA,
		.pin = LL_GPIO_PIN_10,
		.afn = LL_GPIO_AF_10,
		.n = LL_TIM_CHANNEL_CH4,
		.ccr = &TIM2->CCR4,
	},
	[LID_TIM_TIM2_CH1_PA15] = {
		.tim = &_tims[LID_TIM_TIM2],
		.port = GPIOA,
		.pin = LL_GPIO_PIN_15,
		.afn = LL_GPIO_AF_1,
		.n = LL_TIM_CHANNEL_CH1,
		.ccr = &TIM2->CCR1,
	},
	[LID_TIM_TIM2_CH2_PB3] = {
		.tim = &_tims[LID_TIM_TIM2],
		.port = GPIOB,
		.pin = LL_GPIO_PIN_3,
		.afn = LL_GPIO_AF_1,
		.n = LL_TIM_CHANNEL_CH2,
		.ccr = &TIM2->CCR2,
	},
	[LID_TIM_TIM2_CH3_PB10] = {
		.tim = &_tims[LID_TIM_TIM2],
		.port = GPIOB,
		.pin = LL_GPIO_PIN_10,
		.afn = LL_GPIO_AF_1,
		.n = LL_TIM_CHANNEL_CH3,
		.ccr = &TIM2->CCR3,
	},
	[LID_TIM_TIM2_CH4_PB11] = {
		.tim = &_tims[LID_TIM_TIM2],
		.port = GPIOB,
		.pin = LL_GPIO_PIN_11,
		.afn = LL_GPIO_AF_1,
		.n = LL_TIM_CHANNEL_CH4,
		.ccr = &TIM2->CCR4,
	},
	[LID_TIM_TIM2_CH1_PD3] = {
		.tim = &_tims[LID_TIM_TIM2],
		.port = GPIOD,
		.pin = LL_GPIO_PIN_3,
		.afn = LL_GPIO_AF_2,
		.n = LL_TIM_CHANNEL_CH1,
		.ccr = &TIM2->CCR1,
	},
	[LID_TIM_TIM2_CH2_PD4] = {
		.tim = &_tims[LID_TIM_TIM2],
		.port = GPIOD,
		.pin = LL_GPIO_PIN_4,
		.afn = LL_GPIO_AF_2,
		.n = LL_TIM_CHANNEL_CH2,
		.ccr = &TIM2->CCR2,
	},
	[LID_TIM_TIM2_CH4_PD6] = {
		.tim = &_tims[LID_TIM_TIM2],
		.port = GPIOD,
		.pin = LL_GPIO_PIN_6,
		.afn = LL_GPIO_AF_2,
		.n = LL_TIM_CHANNEL_CH4,
		.ccr = &TIM2->CCR4,
	},
	[LID_TIM_TIM2_CH3_PD7] = {
		.tim = &_tims[LID_TIM_TIM2],
		.port = GPIOD,
		.pin = LL_GPIO_PIN_7,
		.afn = LL_GPIO_AF_2,
		.n = LL_TIM_CHANNEL_CH3,
		.ccr = &TIM2->CCR3,
	},

	[LID_TIM_TIM3_CH2_PA4] = {
		.tim = &_tims[LID_TIM_TIM3],
		.port = GPIOA,
		.pin = LL_GPIO_PIN_4,
		.afn = LL_GPIO_AF_2,
		.n = LL_TIM_CHANNEL_CH2,
		.ccr = &TIM3->CCR2,
	},
	[LID_TIM_TIM3_CH1_PA6] = {
		.tim = &_tims[LID_TIM_TIM3],
		.port = GPIOA,
		.pin = LL_GPIO_PIN_6,
		.afn = LL_GPIO_AF_2,
		.n = LL_TIM_CHANNEL_CH1,
		.ccr = &TIM3->CCR1,
	},
	[LID_TIM_TIM3_CH2_PA7] = {
		.tim = &_tims[LID_TIM_TIM3],
		.port = GPIOA,
		.pin = LL_GPIO_PIN_7,
		.afn = LL_GPIO_AF_2,
		.n = LL_TIM_CHANNEL_CH2,
		.ccr = &TIM3->CCR2,
	},
	[LID_TIM_TIM3_CH3_PB0] = {
		.tim = &_tims[LID_TIM_TIM3],
		.port = GPIOB,
		.pin = LL_GPIO_PIN_0,
		.afn = LL_GPIO_AF_2,
		.n = LL_TIM_CHANNEL_CH3,
		.ccr = &TIM3->CCR3,
	},
	[LID_TIM_TIM3_CH4_PB1] = {
		.tim = &_tims[LID_TIM_TIM3],
		.port = GPIOB,
		.pin = LL_GPIO_PIN_1,
		.afn = LL_GPIO_AF_2,
		.n = LL_TIM_CHANNEL_CH4,
		.ccr = &TIM3->CCR4,
	},
	[LID_TIM_TIM3_CH1_PB4] = {
		.tim = &_tims[LID_TIM_TIM3],
		.port = GPIOB,
		.pin = LL_GPIO_PIN_4,
		.afn = LL_GPIO_AF_2,
		.n = LL_TIM_CHANNEL_CH1,
		.ccr = &TIM3->CCR1,
	},
	[LID_TIM_TIM3_CH2_PB5] = {
		.tim = &_tims[LID_TIM_TIM3],
		.port = GPIOB,
		.pin = LL_GPIO_PIN_5,
		.afn = LL_GPIO_AF_2,
		.n = LL_TIM_CHANNEL_CH2,
		.ccr = &TIM3->CCR2,
	},
	[LID_TIM_TIM3_CH1_PC6] = {
		.tim = &_tims[LID_TIM_TIM3],
		.port = GPIOC,
		.pin = LL_GPIO_PIN_6,
		.afn = LL_GPIO_AF_2,
		.n = LL_TIM_CHANNEL_CH1,
		.ccr = &TIM3->CCR1,
	},
	[LID_TIM_TIM3_CH2_PC7] = {
		.tim = &_tims[LID_TIM_TIM3],
		.port = GPIOC,
		.pin = LL_GPIO_PIN_7,
		.afn = LL_GPIO_AF_2,
		.n = LL_TIM_CHANNEL_CH2,
		.ccr = &TIM3->CCR2,
	},
	[LID_TIM_TIM3_CH3_PC8] = {
		.tim = &_tims[LID_TIM_TIM3],
		.port = GPIOC,
		.pin = LL_GPIO_PIN_8,
		.afn = LL_GPIO_AF_2,
		.n = LL_TIM_CHANNEL_CH3,
		.ccr = &TIM3->CCR3,
	},
	[LID_TIM_TIM3_CH4_PC9] = {
		.tim = &_tims[LID_TIM_TIM3],
		.port = GPIOC,
		.pin = LL_GPIO_PIN_9,
		.afn = LL_GPIO_AF_2,
		.n = LL_TIM_CHANNEL_CH4,
		.ccr = &TIM3->CCR4,
	},
	[LID_TIM_TIM3_CH1_PE2] = {
		.tim = &_tims[LID_TIM_TIM3],
		.port = GPIOE,
		.pin = LL_GPIO_PIN_2,
		.afn = LL_GPIO_AF_2,
		.n = LL_TIM_CHANNEL_CH1,
		.ccr = &TIM3->CCR1,
	},
	[LID_TIM_TIM3_CH2_PE3] = {
		.tim = &_tims[LID_TIM_TIM3],
		.port = GPIOE,
		.pin = LL_GPIO_PIN_3,
		.afn = LL_GPIO_AF_2,
		.n = LL_TIM_CHANNEL_CH2,
		.ccr = &TIM3->CCR2,
	},
	[LID_TIM_TIM3_CH3_PE4] = {
		.tim = &_tims[LID_TIM_TIM3],
		.port = GPIOE,
		.pin = LL_GPIO_PIN_4,
		.afn = LL_GPIO_AF_2,
		.n = LL_TIM_CHANNEL_CH3,
		.ccr = &TIM3->CCR3,
	},
	[LID_TIM_TIM3_CH4_PE5] = {
		.tim = &_tims[LID_TIM_TIM3],
		.port = GPIOE,
		.pin = LL_GPIO_PIN_5,
		.afn = LL_GPIO_AF_2,
		.n = LL_TIM_CHANNEL_CH4,
		.ccr = &TIM3->CCR4,
	},

	/*TIM4 channels*/
	[LID_TIM_TIM4_CH1_PA11] = {
		.tim = &_tims[LID_TIM_TIM4],
		.port = GPIOA,
		.pin = LL_GPIO_PIN_11,
		.afn = LL_GPIO_AF_10,
		.n = LL_TIM_CHANNEL_CH1,
		.ccr = &TIM4->CCR1,
	},
	[LID_TIM_TIM4_CH2_PA12] = {
		.tim = &_tims[LID_TIM_TIM4],
		.port = GPIOA,
		.pin = LL_GPIO_PIN_12,
		.afn = LL_GPIO_AF_10,
		.n = LL_TIM_CHANNEL_CH2,
		.ccr = &TIM4->CCR2,
	},
	[LID_TIM_TIM4_CH3_PA13] = {
		.tim = &_tims[LID_TIM_TIM4],
		.port = GPIOA,
		.pin = LL_GPIO_PIN_13,
		.afn = LL_GPIO_AF_10,
		.n = LL_TIM_CHANNEL_CH3,
		.ccr = &TIM4->CCR3,
	},
	[LID_TIM_TIM4_CH1_PB6] = {
		.tim = &_tims[LID_TIM_TIM4],
		.port = GPIOB,
		.pin = LL_GPIO_PIN_6,
		.afn = LL_GPIO_AF_2,
		.n = LL_TIM_CHANNEL_CH1,
		.ccr = &TIM4->CCR1,
	},
	[LID_TIM_TIM4_CH2_PB7] = {
		.tim = &_tims[LID_TIM_TIM4],
		.port = GPIOB,
		.pin = LL_GPIO_PIN_7,
		.afn = LL_GPIO_AF_2,
		.n = LL_TIM_CHANNEL_CH2,
		.ccr = &TIM4->CCR2,
	},
	[LID_TIM_TIM4_CH3_PB8] = {
		.tim = &_tims[LID_TIM_TIM4],
		.port = GPIOB,
		.pin = LL_GPIO_PIN_8,
		.afn = LL_GPIO_AF_2,
		.n = LL_TIM_CHANNEL_CH3,
		.ccr = &TIM4->CCR3,
	},
	[LID_TIM_TIM4_CH4_PB9] = {
		.tim = &_tims[LID_TIM_TIM4],
		.port = GPIOB,
		.pin = LL_GPIO_PIN_9,
		.afn = LL_GPIO_AF_2,
		.n = LL_TIM_CHANNEL_CH4,
		.ccr = &TIM4->CCR4,
	},
	[LID_TIM_TIM4_CH1_PD12] = {
		.tim = &_tims[LID_TIM_TIM4],
		.port = GPIOD,
		.pin = LL_GPIO_PIN_12,
		.afn = LL_GPIO_AF_2,
		.n = LL_TIM_CHANNEL_CH1,
		.ccr = &TIM4->CCR1,
	},
	[LID_TIM_TIM4_CH2_PD13] = {
		.tim = &_tims[LID_TIM_TIM4],
		.port = GPIOD,
		.pin = LL_GPIO_PIN_13,
		.afn = LL_GPIO_AF_2,
		.n = LL_TIM_CHANNEL_CH2,
		.ccr = &TIM4->CCR2,
	},
	[LID_TIM_TIM4_CH3_PD14] = {
		.tim = &_tims[LID_TIM_TIM4],
		.port = GPIOD,
		.pin = LL_GPIO_PIN_14,
		.afn = LL_GPIO_AF_2,
		.n = LL_TIM_CHANNEL_CH3,
		.ccr = &TIM4->CCR3,
	},
	[LID_TIM_TIM4_CH4_PD15] = {
		.tim = &_tims[LID_TIM_TIM4],
		.port = GPIOD,
		.pin = LL_GPIO_PIN_15,
		.afn = LL_GPIO_AF_2,
		.n = LL_TIM_CHANNEL_CH4,
		.ccr = &TIM4->CCR4,
	},

	/*TIM8 Channels*/
	[LID_TIM_TIM8_CH1N_PA7] = {
		.tim = &_tims[LID_TIM_TIM8],
		.port = GPIOA,
		.pin = LL_GPIO_PIN_7,
		.afn = LL_GPIO_AF_4,
		.n = LL_TIM_CHANNEL_CH1N,
		.ccr = &TIM8->CCR1,
	},
	[LID_TIM_TIM8_CH2_PA14] = {
		.tim = &_tims[LID_TIM_TIM8],
		.port = GPIOA,
		.pin = LL_GPIO_PIN_14,
		.afn = LL_GPIO_AF_5,
		.n = LL_TIM_CHANNEL_CH2,
		.ccr = &TIM8->CCR2,
	},
	[LID_TIM_TIM8_CH1_PA15] = {
		.tim = &_tims[LID_TIM_TIM8],
		.port = GPIOA,
		.pin = LL_GPIO_PIN_15,
		.afn = LL_GPIO_AF_2,
		.n = LL_TIM_CHANNEL_CH1,
		.ccr = &TIM8->CCR1,
	},
	[LID_TIM_TIM8_CH2N_PB0] = {
		.tim = &_tims[LID_TIM_TIM8],
		.port = GPIOB,
		.pin = LL_GPIO_PIN_0,
		.afn = LL_GPIO_AF_4,
		.n = LL_TIM_CHANNEL_CH2N,
		.ccr = &TIM8->CCR2,
	},
	[LID_TIM_TIM8_CH3N_PB1] = {
		.tim = &_tims[LID_TIM_TIM8],
		.port = GPIOB,
		.pin = LL_GPIO_PIN_1,
		.afn = LL_GPIO_AF_4,
		.n = LL_TIM_CHANNEL_CH3N,
		.ccr = &TIM8->CCR3,
	},
	[LID_TIM_TIM8_CH1N_PB3] = {
		.tim = &_tims[LID_TIM_TIM8],
		.port = GPIOB,
		.pin = LL_GPIO_PIN_3,
		.afn = LL_GPIO_AF_4,
		.n = LL_TIM_CHANNEL_CH1N,
		.ccr = &TIM8->CCR3,
	},
	[LID_TIM_TIM8_CH2N_PB4] = {
		.tim = &_tims[LID_TIM_TIM8],
		.port = GPIOB,
		.pin = LL_GPIO_PIN_4,
		.afn = LL_GPIO_AF_4,
		.n = LL_TIM_CHANNEL_CH2N,
		.ccr = &TIM8->CCR2,
	},
	[LID_TIM_TIM8_CH3N_PB5] = {
		.tim = &_tims[LID_TIM_TIM8],
		.port = GPIOB,
		.pin = LL_GPIO_PIN_5,
		.afn = LL_GPIO_AF_3,
		.n = LL_TIM_CHANNEL_CH3N,
		.ccr = &TIM8->CCR3,
	},
	[LID_TIM_TIM8_CH1_PB6] = {
		.tim = &_tims[LID_TIM_TIM8],
		.port = GPIOB,
		.pin = LL_GPIO_PIN_6,
		.afn = LL_GPIO_AF_5,
		.n = LL_TIM_CHANNEL_CH1,
		.ccr = &TIM8->CCR1,
	},
	[LID_TIM_TIM8_CH2_PB8] = {
		.tim = &_tims[LID_TIM_TIM8],
		.port = GPIOB,
		.pin = LL_GPIO_PIN_8,
		.afn = LL_GPIO_AF_10,
		.n = LL_TIM_CHANNEL_CH2,
		.ccr = &TIM8->CCR2,
	},
	[LID_TIM_TIM8_CH3_PB9] = {
		.tim = &_tims[LID_TIM_TIM8],
		.port = GPIOB,
		.pin = LL_GPIO_PIN_9,
		.afn = LL_GPIO_AF_10,
		.n = LL_TIM_CHANNEL_CH3,
		.ccr = &TIM8->CCR3,
	},
	[LID_TIM_TIM8_CH1_PC6] = {
		.tim = &_tims[LID_TIM_TIM8],
		.port = GPIOC,
		.pin = LL_GPIO_PIN_6,
		.afn = LL_GPIO_AF_4,
		.n = LL_TIM_CHANNEL_CH1,
		.ccr = &TIM8->CCR1,
	},
	[LID_TIM_TIM8_CH2_PC7] = {
		.tim = &_tims[LID_TIM_TIM8],
		.port = GPIOC,
		.pin = LL_GPIO_PIN_7,
		.afn = LL_GPIO_AF_4,
		.n = LL_TIM_CHANNEL_CH2,
		.ccr = &TIM8->CCR2,
	},
	[LID_TIM_TIM8_CH3_PC8] = {
		.tim = &_tims[LID_TIM_TIM8],
		.port = GPIOC,
		.pin = LL_GPIO_PIN_8,
		.afn = LL_GPIO_AF_4,
		.n = LL_TIM_CHANNEL_CH3,
		.ccr = &TIM8->CCR3,
	},
	[LID_TIM_TIM8_CH4_PC9] = {
		.tim = &_tims[LID_TIM_TIM8],
		.port = GPIOC,
		.pin = LL_GPIO_PIN_9,
		.afn = LL_GPIO_AF_4,
		.n = LL_TIM_CHANNEL_CH4,
		.ccr = &TIM8->CCR4,
	},
	[LID_TIM_TIM8_CH1N_PC10] = {
		.tim = &_tims[LID_TIM_TIM8],
		.port = GPIOC,
		.pin = LL_GPIO_PIN_10,
		.afn = LL_GPIO_AF_4,
		.n = LL_TIM_CHANNEL_CH1N,
		.ccr = &TIM8->CCR1,
	},
	[LID_TIM_TIM8_CH2N_PC11] = {
		.tim = &_tims[LID_TIM_TIM8],
		.port = GPIOC,
		.pin = LL_GPIO_PIN_11,
		.afn = LL_GPIO_AF_4,
		.n = LL_TIM_CHANNEL_CH2N,
		.ccr = &TIM8->CCR2,
	},
	[LID_TIM_TIM8_CH3N_PC12] = {
		.tim = &_tims[LID_TIM_TIM8],
		.port = GPIOC,
		.pin = LL_GPIO_PIN_12,
		.afn = LL_GPIO_AF_4,
		.n = LL_TIM_CHANNEL_CH3N,
		.ccr = &TIM8->CCR3,
	},
	[LID_TIM_TIM8_CH4N_PC13] = {
		.tim = &_tims[LID_TIM_TIM8],
		.port = GPIOC,
		.pin = LL_GPIO_PIN_13,
		.afn = LL_GPIO_AF_4,
		.n = LL_TIM_CHANNEL_CH4N,
		.ccr = &TIM8->CCR4,
	},
	[LID_TIM_TIM8_CH4N_PD0] = {
		.tim = &_tims[LID_TIM_TIM8],
		.port = GPIOC,
		.pin = LL_GPIO_PIN_0,
		.afn = LL_GPIO_AF_6,
		.n = LL_TIM_CHANNEL_CH4N,
		.ccr = &TIM8->CCR4,
	},
	[LID_TIM_TIM8_CH4_PD1] = {
		.tim = &_tims[LID_TIM_TIM8],
		.port = GPIOC,
		.pin = LL_GPIO_PIN_1,
		.afn = LL_GPIO_AF_4,
		.n = LL_TIM_CHANNEL_CH4,
		.ccr = &TIM8->CCR4,
	},
};

void LID_TIM_Init() {
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4);

	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM8);
}

int32_t LID_TIM_PWM_Init(LID_TIM_TimerId_t id, uint16_t psc, uint16_t res) {
	if(id > LID_TIM_N_TIM || id < 0)
		return -1;

	LID_TIM_Timer_t *tim = &_tims[id];
	if(tim->utype != LID_TIM_USAGE_000)
		return -1;

	LL_TIM_InitTypeDef TIM_InitStruct = {0};
	TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
	TIM_InitStruct.CounterMode = LL_TIM_COUNTERDIRECTION_UP;
	TIM_InitStruct.Autoreload = res;
	TIM_InitStruct.Prescaler = psc;
	LL_TIM_Init(tim->timx, &TIM_InitStruct);
	tim->cfreq = 0;
	tim->utype = LID_TIM_USAGE_PWM;
	LL_TIM_EnableCounter(tim->timx);

	return 0;
}

int LID_TIM_PWM_AddChannel(LID_TIM_PWM_Channel_t *pwmChannel, LID_TIM_ChannelId_t channelId, LID_TIM_PWM_ChannelType_t pwmType) {
	if(channelId > LID_TIM_N_TIM_CHANNELS)
		return -1;

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
		//LL_TIM_IC_InitTypeDef IC_InitStruct = {0};
	}

	return 0;
}

int32_t LID_TIM_PWM_Read(LID_TIM_PWM_Channel_t *pwmChannel, uint16_t *duty) {
	if(pwmChannel->channelId > LID_TIM_N_TIM_CHANNELS)
		return -1;

	const LID_TIM_Channel_t *ch = &_tim_channels[pwmChannel->channelId];
	if(ch->tim->utype != LID_TIM_USAGE_PWM)
		return -1;

	*duty = (uint16_t) ((*ch->ccr) & 0xFFFF);
	uint16_t freq = SystemCoreClock /
			(LL_TIM_GetAutoReload(ch->tim->timx) * LL_TIM_GetPrescaler(ch->tim->timx));
	return freq;
}

int32_t LID_TIM_PWM_SetFreq(LID_TIM_TimerId_t id, uint16_t psc, uint16_t res) {
	if(id > LID_TIM_N_TIM_CHANNELS)
		return -1;

	const LID_TIM_Timer_t *tim = &_tims[id];
	if(tim->utype != LID_TIM_USAGE_PWM)
		return -1;

	LL_TIM_SetPrescaler(tim->timx, psc);
	LL_TIM_SetAutoReload(tim->timx, res);
	return SystemCoreClock / (psc *res);
}

int32_t LID_TIM_PWM_SetDuty(LID_TIM_PWM_Channel_t *pwmChannel, uint16_t duty) {
	if(pwmChannel->channelId > LID_TIM_N_TIM_CHANNELS)
		return -1;

	const LID_TIM_Channel_t *ch = &_tim_channels[pwmChannel->channelId];
	if(ch->tim->utype != LID_TIM_USAGE_PWM)
		return -1;

	*ch->ccr = duty;
	return duty;
}

int LID_TIM_PWM_Enable(LID_TIM_PWM_Channel_t *pwmChannel, int enable) {
	if(pwmChannel->channelId > LID_TIM_N_TIM_CHANNELS)
		return 0;

	const LID_TIM_Channel_t *ch = &_tim_channels[pwmChannel->channelId];
	if(ch->tim->utype != LID_TIM_USAGE_PWM)
		return 0;

	if(enable) {
		if(!LL_TIM_IsEnabledCounter(ch->tim->timx))
			LL_TIM_EnableCounter(ch->tim->timx);

		LL_TIM_CC_EnableChannel(ch->tim->timx, ch->n);
	} else {
		LL_TIM_CC_DisableChannel(ch->tim->timx, ch->n);
	}

	return enable;
}

