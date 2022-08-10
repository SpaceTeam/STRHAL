#ifndef SERVOCHANNEL_H
#define SERVOCHANNEL_H

#include "AbstractChannel.h"
#include "ADCChannel.h"

#include <can_houbolt/channels/servo_channel_def.h>
#include <STRHAL.h>
#include "../Modules/W25Qxx_Flash.h"

struct ServoRefPos
{
		uint16_t start;
		uint16_t end;
};

enum class ServoState : int
{
	IDLE = 0,
	READY,
	FAULT,
	MOVIN,
	CALIB,
};

class ServoChannel: public AbstractChannel
{
	public:
		ServoChannel(uint8_t id, uint8_t servoId, const STRHAL_TIM_TimerId_t &pwmTimer, const STRHAL_TIM_ChannelId_t &control, const STRHAL_ADC_Channel_t &feedbackChannel, const STRHAL_ADC_Channel_t &currentChannel, const STRHAL_GPIO_t &led, uint32_t refreshDivider);

		int init() override;
		int reset() override;
		int exec() override;

		int processMessage(uint8_t commandId, uint8_t *returnData, uint8_t &n) override;
		int getSensorData(uint8_t *data, uint8_t &n) override;

		void setTargetPos(uint16_t pos);
		void moveToPosInInterval(uint16_t position, uint16_t interval);
		uint16_t getTargetPos() const;

		uint16_t getPos() const;
		uint16_t getFeedbackMeasurement() const;
		uint16_t getCurrentMeasurement() const;

		static constexpr uint16_t PWM_FREQ = 50;
		static constexpr uint16_t PWM_RES = (1000 / PWM_FREQ) * 1800; //36.000
		static constexpr uint16_t PWM_PSC = STRHAL_SYSCLK_FREQ / PWM_RES / PWM_FREQ;

		static constexpr uint16_t POS_DEV = (UINT16_MAX / 180);
		static constexpr uint8_t TARG_HIT_MIN = 20;
		static constexpr uint8_t CALIB_HIT_MIN = 200;

		static constexpr uint64_t EXEC_SAMPLE_TICKS = 5;

		//static constexpr ServoRefPos pwm0Ref = {1800, 3600};
		static constexpr ServoRefPos pwm0Ref =
		{ 900, 4500 };
		static constexpr ServoRefPos com0Ref =
		{ 0, UINT16_MAX };
		static constexpr ServoRefPos adc0Ref =
		{ 0, 0xFFF };

		static uint16_t tPosFromCanonic(uint16_t pos, const ServoRefPos &frame);
		static uint16_t tPosToCanonic(uint16_t pos, const ServoRefPos &frame);
		static uint16_t distPos(uint16_t pos1, uint16_t pos2);

	protected:
		int setVariable(uint8_t variableId, int32_t data) override;
		int getVariable(uint8_t variableId, int32_t &data) const override;

	private:
		uint8_t servoId;
		STRHAL_TIM_TimerId_t pwmTimer;
		STRHAL_TIM_ChannelId_t ctrlChannelId;
		STRHAL_TIM_PWM_Channel_t pwmChannel;

		STRHAL_ADC_Data_t *feedbackMeasurement = nullptr;
		STRHAL_ADC_Data_t *currentMeasurement = nullptr;

		STRHAL_ADC_Channel_t feedbackChannel;
		STRHAL_ADC_Channel_t currentChannel;

		STRHAL_GPIO_t led;

		uint16_t targetPosition = 0;
		uint16_t feedbackPosition = 0;
		uint16_t finalPosition = 0;
		int16_t step = 0;

		ServoRefPos adcRef = adc0Ref;
		ServoRefPos pwmRef = pwm0Ref;

		W25Qxx_Flash &flash;

		ServoState servoState;
		bool reqCalib;

		uint16_t targetPositionLast = 0, feedbackPositionLast = 0;
		uint16_t targetHitCount = 0;
		uint64_t timeLastSample = 0, timeLastCommand = 0;
};

#endif /*SERVOCHANNEL_H*/
