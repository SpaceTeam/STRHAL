#ifndef SPEAKER_H
#define SPEAKER_H

#include <STRHAL.h>

class Speaker
{
	public:
		Speaker(const STRHAL_TIM_TimerId_t &pwmTimer, const STRHAL_TIM_ChannelId_t &control);

		int init();

		static constexpr uint16_t PWM_FREQ = 500;
		static constexpr uint16_t PWM_RES = (1000 / PWM_FREQ) * 1800;
		static constexpr uint16_t PWM_PSC = STRHAL_SYSCLK_FREQ / PWM_RES / PWM_FREQ;

		int setPWM(uint16_t freq);
		void enable(bool ena);
		void beep(uint8_t n, uint16_t tot1, uint16_t tot2);

	private:
		STRHAL_TIM_TimerId_t pwmTimer;
		STRHAL_TIM_ChannelId_t ctrlChannelId;
		STRHAL_TIM_PWM_Channel_t pwmChannel;

};

#endif /*SERVOCHANNEL_H*/
