#ifndef ADCCHANNEL_H
#define ADCCHANNEL_H

#include "./Channels/AbstractChannel.h"
#include <can_houbolt/channels/adc16_channel_def.h>
#include <LID.h>

class ADCChannel : public AbstractChannel {
	public:
		ADCChannel(uint8_t channel_id, const LID_ADC_Channel_t &adc_ch);

		ADCChannel(const ADCChannel &other) = delete;
		ADCChannel& operator=(const ADCChannel &other) = delete;
		ADCChannel(const ADCChannel &&other) = delete;
		ADCChannel& operator=(const ADCChannel &&other) = delete;

		int init() override;
		int reset() override;
		int exec() override;
		int getSensorData(uint8_t *data, uint8_t &n) override;

		uint16_t getMeas() const;

		int prcMsg(uint8_t cmd_id, uint8_t *ret_data, uint8_t &ret_n) override;

	protected:

		int setVar(uint8_t variable_id, int32_t data) override;
		int getVar(uint8_t variable_id, int32_t *data) const override;

	private:
		LID_ADC_Channel_t adc_ch;
		LID_ADC_Data_t *adc_meas = nullptr;

};

#endif /*ADCCHANNEL_H*/
