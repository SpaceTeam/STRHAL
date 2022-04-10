#ifndef SPEAKER_H
#define SPEAKER_H

#include <STRHAL.h>

class Speaker {
	public:
		Speaker(const STRHAL_TIM_TimerId_t &pwm_timer, const STRHAL_TIM_ChanneSTRHAL_t &control);

		int init();

		static constexpr uint16_t PWM_FREQ = 500;
		static constexpr uint16_t PWM_RES = (1000 / PWM_FREQ)*1800;
		static constexpr uint16_t PWM_PSC = STRHAL_SYSCLK_FREQ / PWM_RES / PWM_FREQ;

		int setPWM(uint16_t freq);
		void enable(bool ena);
		void beep(uint8_t n, uint16_t tot1, uint16_t tot2);

	private:
		STRHAL_TIM_TimerId_t pwm_tim;
		STRHAL_TIM_ChanneSTRHAL_t ctrl_chid;
		STRHAL_TIM_PWM_Channel_t pwm_ch;

};

#endif /*SERVOCHANNEL_H*/
