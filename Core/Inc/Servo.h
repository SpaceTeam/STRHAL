#ifndef SERVO_H
#define SERVO_H

#include "./AbstractChannel.h"

#include <can_houbolt/channels/servo_channel_def.h>
#include <LID.h>

class Servo : public AbstractChannel {
	public:
		Servo(CHANNEL_TYPE t, uint8_t channel_id, const LID_TIM_TimerId_t &pwm_timer, const LID_TIM_ChannelId_t &control, const LID_ADC_Channel_t &feedback, const LID_ADC_Channel_t &current, const LID_GPIO_t &led_o);

		Servo(const Servo &other) = delete;
		Servo& operator=(const Servo &other) = delete;

		int init() override;
		int reset() override;
		int exec() override;


		int prcMsg(uint8_t cmd_id, uint8_t variable_id, uint32_t data, uint32_t &ret) override;
		int move();

		int setTargetPos(uint16_t pos);
		uint16_t getTargetPos() const;

		uint16_t getPos() const;

		static constexpr uint16_t SERVO_PWM_FREQ = 50;

	protected:
		int setVar(uint8_t variable_id, uint32_t data) override;
		int getVar(uint8_t variable_id, uint32_t &data) const override;

	private:
		LID_TIM_TimerId_t pwm_tim;
		LID_TIM_ChannelId_t ctrl_chid;
		LID_ADC_Channel_t fdbk_ch, curr_ch;
		LID_GPIO_t led_o;
		LID_TIM_PWM_Channel_t pwm_ch;

		LID_ADC_Data_t *fdbk_meas = nullptr;
		LID_ADC_Data_t *curr_meas = nullptr;

		uint32_t t_pos;
		uint32_t c_pos;

		uint32_t min_pos = 0;
		uint32_t max_pos = 360;
};

#endif /*SERVO_H*/
