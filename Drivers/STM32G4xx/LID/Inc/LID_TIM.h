#ifndef LID_TIM_H
#define LID_TIM_H

#include <stdint.h>
#include <stm32g4xx.h>

typedef struct {
	TIM_TypeDef *timx;
	uint16_t cfreq;
} LID_TIM_Timer_t;

typedef struct {
	TIM_TypeDef *timx;
	GPIO_TypeDef *port;
	uint8_t pin;
	uint8_t afn;
	uint8_t n;
} LID_TIM_Channel_t;

const LID_TIM_Channel_t LID_TIM1_CH1_PB4 = {.timx = TIM1, .port = GPIOB, .pin = 4, .n = 1, .afn = 10};

typedef enum {
	LID_TIMER_PWM_SO, //single out channel
	LID_TIMER_PWM_IN, //input measure duty cycle
} LID_TIM_PWM_ChannelType_t;


typedef struct {
	uint16_t freq;
	uint16_t duty;
} LID_TIM_PWM_Param_t;

typedef struct {
	LID_TIM_Channel_t *realChannel;
	LID_TIM_PWM_ChannelType_t type;
	LID_TIM_PWM_Param_t param;
} LID_TIM_PWM_Channel_t;


void LID_TIM_Init();

void LID_TIM_Timer_Init(LID_TIM_Timer_t *tim, TIM_TypeDef *timx, uint16_t cfreq);

int LID_TIM_PWM_AddChannel(LID_TIM_PWM_Channel_t *pwmChannel, LID_TIM_Channel_t *timChannel, LID_TIM_PWM_ChannelType_t pwmType);

void LID_TIM_PWM_Read(LID_TIM_PWM_Channel_t *pwmChannel, LID_TIM_PWM_Param_t *param);
uint16_t LID_TIM_PWM_SetFreq(LID_TIM_PWM_Channel_t *pwmChannel, uint16_t frequency);
uint16_t LID_TIM_PWM_SetDuty(LID_TIM_PWM_Channel_t *pwmChannel, uint16_t duty);
int LID_TIM_PWM_Enable(LID_TIM_PWM_Channel_t *pwmChannel, int enable);



#endif /*LID_TIM_H*/
