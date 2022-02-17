#ifndef PRESSURECHANNEL_H
#define PRESSURECHANNEL_H

#include "./Channels/AbstractChannel.h"
#include <can_houbolt/channels/adc16_channel_def.h>
#include <LID.h>

/*{
[LID_ADC_PRESS_0] =			{ ADC2, LL_ADC_CHANNEL_15 },
[LID_ADC_PRESS_1] =			{ ADC3, LL_ADC_CHANNEL_5 },
[LID_ADC_PRESS_2] =			{ ADC1, LL_ADC_CHANNEL_14 },
[LID_ADC_PRESS_3] =			{ ADC3, LL_ADC_CHANNEL_7 },
[LID_ADC_PRESS_4] =			{ ADC1, LL_ADC_CHANNEL_5 },
[LID_ADC_PRESS_5] =			{ ADC1, LL_ADC_CHANNEL_11 },
};*/

class PressureChannel : public AbstractChannel {
	public:
		PressureChannel(uint8_t channel_id, const LID_ADC_Channel_t & adc_ch);

		PressureChannel(const PressureChannel &other) = delete;
		PressureChannel& operator=(const PressureChannel &other) = delete;

		int init() override;
		int reset() override;
		int exec() override;
		int getSensorData(uint8_t *data, uint8_t &n) override;

		int prcMsg(uint8_t cmd_id, uint8_t variable_id, uint32_t data, uint32_t &ret) override;

	protected:

		int setVar(uint8_t variable_id, uint32_t data) override;
		int getVar(uint8_t variable_id, uint32_t &data) const override;

	private:
		LID_ADC_Channel_t press_ch;
		LID_ADC_Data_t *press_meas = nullptr;

};

#endif /*PRESSURECHANNEL_H*/
