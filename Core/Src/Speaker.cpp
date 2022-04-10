#include "../Inc/Speaker.h"

Speaker::Speaker(const STRHAL_TIM_TimerId_t &pwm_timer, const STRHAL_TIM_ChanneSTRHAL_t &control) :
	pwm_tim(pwm_timer), ctrl_chid(control) {
}

int Speaker::init() {
	if(STRHAL_TIM_PWM_Init(pwm_tim, PWM_PSC, PWM_RES) < 0)
		return -1;

	if(STRHAL_TIM_PWM_AddChannel(&pwm_ch, ctrl_chid, STRHAL_TIM_PWM_CHANNELTYPE_SO) < 0)
		return -1;

	if(STRHAL_TIM_PWM_SetDuty(&pwm_ch, 1800) != 1800) {
		return -1;
	}

	return 0;
}

int Speaker::setPWM(uint16_t freq) {
	uint16_t res = (1000 / freq)*1800;
	uint16_t psc = STRHAL_SYSCLK_FREQ / res / freq;
	enable(false);
	int32_t ret = STRHAL_TIM_PWM_SetFreq(pwm_tim, psc, res);
	if(ret != (STRHAL_SYSCLK_FREQ / (psc *res))) {
		return -1;
	} else {
		if(STRHAL_TIM_PWM_SetDuty(&pwm_ch, res/2) != res/2) {
			return -1;
		}
		return 0;
	}
}

void Speaker::enable(bool ena) {
	STRHAL_TIM_PWM_Enable(&pwm_ch, ena);
}

void Speaker::beep(uint8_t n, uint16_t tot1, uint16_t tot2) {
	for(int i = 0; i < n; i++) {
		STRHAL_TIM_PWM_Enable(&pwm_ch, true);
		LL_mDelay(tot1);
		STRHAL_TIM_PWM_Enable(&pwm_ch, false);
		LL_mDelay(tot2);
	}
}
