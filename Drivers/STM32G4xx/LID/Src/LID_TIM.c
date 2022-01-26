#include "../Inc/LID_TIM.h"

#include <stm32g4xx_ll_tim.h>


void LID_TIM_Init() {

}

void LID_TIM_Timer_Init(LID_TIM_Timer_t *tim, TIM_TypeDef *timx, uint16_t cfreq) {

}

int LID_TIM_PWM_AddChannel(LID_TIM_PWM_Channel_t *pwmChannel, LID_TIM_Channel_t *timChannel, LID_TIM_PWM_ChannelType_t pwmType) {
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

