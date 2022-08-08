#ifndef PYROCHANNEL_H
#define PYROCHANNEL_H

#include "./Channels/AbstractChannel.h"
#include "./Channels/DigitalInChannel.h"
#include <can_houbolt/channels/digital_out_channel_def.h>
#include <STRHAL.h>

class PyroChannel: public AbstractChannel
{
	public:
		PyroChannel(uint8_t id, const STRHAL_ADC_Channel_t &adcChannel, const STRHAL_GPIO_t &cntrlPin, const DigitalInChannel &continuityChannel, uint32_t refreshDivider);

		PyroChannel(const PyroChannel &other) = delete;
		PyroChannel& operator=(const PyroChannel &other) = delete;
		PyroChannel(const PyroChannel &&other) = delete;
		PyroChannel& operator=(const PyroChannel &&other) = delete;

		int init() override;
		int reset() override;
		int exec() override;
		int getSensorData(uint8_t *data, uint8_t &n) override;

		uint16_t getMeasurement() const;
		uint16_t getContinuity() const;

		uint32_t getState() const;
		int setState(uint32_t state);

		int processMessage(uint8_t commandId, uint8_t *returnData, uint8_t &n) override;

	protected:

		int setVariable(uint8_t variableId, int32_t data) override;
		int getVariable(uint8_t variableId, int32_t &data) const override;

	private:
		uint16_t dutyCycle;
		uint16_t frequency;
		STRHAL_ADC_Channel_t adcChannel;
		STRHAL_ADC_Data_t *adcMeasurement = nullptr;
		STRHAL_GPIO_t cntrlPin;
		const DigitalInChannel &continuityChannel;

};

#endif /*PYROCHANNEL_H*/
