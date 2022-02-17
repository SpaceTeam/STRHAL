#ifndef TEMPCHANNEL_H
#define TEMPCHANNEL_H

#include "./Channels/AbstractChannel.h"
#include <can_houbolt/channels/adc16_channel_def.h>
#include <LID.h>

/*{
[LID_ADC_TEMP_0] =			{ ADC1, LL_ADC_CHANNEL_6 },
[LID_ADC_TEMP_1] =			{ ADC1, LL_ADC_CHANNEL_7 },
[LID_ADC_TEMP_2] =			{ ADC1, LL_ADC_CHANNEL_8 },
};*/

class TempChannel : public AbstractChannel {
	public:
		TempChannel(uint8_t channel_id, const LID_ADC_Channel_t & adc_ch);

		TempChannel(const TempChannel &other) = delete;
		TempChannel& operator=(const TempChannel &other) = delete;

		int init() override;
		int reset() override;
		int exec() override;
		int getSensorData(uint8_t *data, uint8_t &n) override;

		int prcMsg(uint8_t cmd_id, uint8_t variable_id, uint32_t data, uint32_t &ret) override;

	protected:

		int setVar(uint8_t variable_id, uint32_t data) override;
		int getVar(uint8_t variable_id, uint32_t &data) const override;

	private:
		LID_ADC_Channel_t temp_ch;
		LID_ADC_Data_t *temp_meas = nullptr;

};

#endif /*TEMPCHANNEL_H*/
