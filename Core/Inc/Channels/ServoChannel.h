#ifndef SERVOCHANNEL_H
#define SERVOCHANNEL_H

#include "AbstractChannel.h"
#include "ADCChannel.h"

#include <can_houbolt/channels/servo_channel_def.h>
#include <LID.h>

struct ServoRefPos {
	uint16_t start;
	uint16_t end;
};

enum class ServoState : int {
	IDLE = 0,
	READY,
	FAULT,
	MOVIN,
	CALIB,
};

class ServoChannel : public AbstractChannel {
	public:
		ServoChannel(uint8_t channel_id, const LID_TIM_TimerId_t &pwm_timer, const LID_TIM_ChannelId_t &control, const LID_ADC_Channel_t &feedbackChannel, const LID_ADC_Channel_t &currentChannel, const LID_GPIO_t &led_o);

		int init() override;
		int reset() override;
		int exec() override;

		int prcMsg(uint8_t cmd_id, uint8_t *ret_data, uint8_t &ret_n) override;
		int getSensorData(uint8_t *data, uint8_t &n) override;


		void setTargetPos(uint16_t pos);
		uint16_t getTargetPos() const;

		uint16_t getPos() const;
		uint16_t getFeedbackMeas() const;
		uint16_t getCurrentMeas() const;

		static constexpr uint16_t PWM_FREQ = 50;
		static constexpr uint16_t PWM_RES = (1000 / PWM_FREQ)*1800;
		static constexpr uint16_t PWM_PSC = LID_SYSCLK_FREQ / PWM_RES;

		static constexpr uint16_t POS_DEV = (UINT16_MAX / 180);
		static constexpr uint8_t TARG_HIT_MIN = 5;

		static constexpr uint64_t EXEC_SAMPLE_TICKS = 5;

		static constexpr ServoRefPos pwm0Ref = {1, 345};
		static constexpr ServoRefPos com0Ref = {0, UINT16_MAX};
		static constexpr ServoRefPos adc0Ref = {0, 0x3F};

		static uint16_t tPosFromCanonic(uint16_t pos, const ServoRefPos &frame);
		static uint16_t tPosToCanonic(uint16_t pos, const ServoRefPos &frame);
		static uint16_t distPos(uint16_t pos1, uint16_t pos2);

	protected:
		int setVar(uint8_t variable_id, int32_t data) override;
		int getVar(uint8_t variable_id, int32_t *data) const override;

	private:
		LID_TIM_TimerId_t pwm_tim;
		LID_TIM_ChannelId_t ctrl_chid;
		LID_TIM_PWM_Channel_t pwm_ch;

		LID_ADC_Data_t *fdbk_meas = nullptr;
		LID_ADC_Data_t *curr_meas = nullptr;

		LID_ADC_Channel_t fdbkCh;
		LID_ADC_Channel_t currCh;

		LID_GPIO_t led_o;

		uint16_t targ_pos;
		uint16_t fdbk_pos;

		ServoRefPos adc_ref = adc0Ref;
		ServoRefPos pwm_ref = pwm0Ref;

		ServoState servo_state;
		bool reqCalib;
};

#endif /*SERVOCHANNEL_H*/
