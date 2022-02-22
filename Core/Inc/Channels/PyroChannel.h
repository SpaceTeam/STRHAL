#ifndef PYROCHANNEL_H
#define PYROCHANNEL_H

#include "./Channels/AbstractChannel.h"
#include "./Channels/DigitalInChannel.h"
#include <can_houbolt/channels/digital_out_channel_def.h>
#include <LID.h>

class PyroChannel : public AbstractChannel {
	public:
		PyroChannel(uint8_t channel_id, const LID_ADC_Channel_t &adc_ch, const LID_GPIO_t &cntrl_pin, const DigitalInChannel &cont_ch);

		PyroChannel(const PyroChannel &other) = delete;
		PyroChannel& operator=(const PyroChannel &other) = delete;
		PyroChannel(const PyroChannel &&other) = delete;
		PyroChannel& operator=(const PyroChannel &&other) = delete;

		int init() override;
		int reset() override;
		int exec() override;
		int getSensorData(uint8_t *data, uint8_t &n) override;

		uint16_t getMeas() const;

		int prcMsg(uint8_t cmd_id, uint8_t variable_id, uint32_t data, uint8_t *ret_data, uint8_t &ret_n) override;

	protected:

		int setVar(uint8_t variable_id, uint32_t data) override;
		int getVar(uint8_t variable_id, uint8_t *data) const override;

		uint32_t getState() const;
		int setState(uint32_t state);

	private:
		uint16_t duty_cycle;
		uint16_t frequency;
		LID_ADC_Channel_t adc_ch;
		LID_ADC_Data_t *adc_meas = nullptr;
		LID_GPIO_t cntrl_pin;
		const DigitalInChannel &cont_ch;

};

#endif /*PYROCHANNEL_H*/
