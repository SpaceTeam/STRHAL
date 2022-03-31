#ifndef DIGITALOUTCHANNEL_H
#define DIGITALOUTCHANNEL_H

#include "./Channels/AbstractChannel.h"
#include <can_houbolt/channels/digital_out_channel_def.h>
#include <LID.h>

class DigitalOutChannel : public AbstractChannel {
	public:
		DigitalOutChannel(uint8_t channel_id, const LID_ADC_Channel_t &adc_ch, const LID_GPIO_t &cntrl_pin, uint32_t refresh_divider);

		DigitalOutChannel(const DigitalOutChannel &other) = delete;
		DigitalOutChannel& operator=(const DigitalOutChannel &other) = delete;
		DigitalOutChannel(const DigitalOutChannel &&other) = delete;
		DigitalOutChannel& operator=(const DigitalOutChannel &&other) = delete;

		int init() override;
		int reset() override;
		int exec() override;
		int getSensorData(uint8_t *data, uint8_t &n) override;

		uint16_t getMeas() const;

		int prcMsg(uint8_t cmd_id, uint8_t *ret_data, uint8_t &ret_n) override;

		uint32_t getState() const;
		int setState(uint32_t state);
	protected:

		int setVar(uint8_t variable_id, int32_t data) override;
		int getVar(uint8_t variable_id, int32_t &data) const override;

	private:
		uint16_t duty_cycle;
		uint16_t frequency;
		LID_ADC_Channel_t adc_ch;
		LID_ADC_Data_t *adc_meas = nullptr;
		LID_GPIO_t cntrl_pin;

};

#endif /*DIGITALOUTCHANNEL_H*/
