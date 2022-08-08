#ifndef DIGITALOUTCHANNEL_H
#define DIGITALOUTCHANNEL_H

#include "./Channels/AbstractChannel.h"
#include <can_houbolt/channels/digital_out_channel_def.h>
#include <STRHAL.h>

class DigitalOutChannel: public AbstractChannel
{
	public:
		DigitalOutChannel(uint8_t id, const STRHAL_ADC_Channel_t &adcChannel, const STRHAL_GPIO_t &cntrlPin, STRHAL_ADC_InType_t adcInType, uint32_t refreshDivider);
		DigitalOutChannel(uint8_t id, const STRHAL_GPIO_t &cntrlPin, uint32_t refreshDivider);

		DigitalOutChannel(const DigitalOutChannel &other) = delete;
		DigitalOutChannel& operator=(const DigitalOutChannel &other) = delete;
		DigitalOutChannel(const DigitalOutChannel &&other) = delete;
		DigitalOutChannel& operator=(const DigitalOutChannel &&other) = delete;

		int init() override;
		int reset() override;
		int exec() override;
		int getSensorData(uint8_t *data, uint8_t &n) override;

		uint16_t getMeas() const;

		int processMessage(uint8_t commandI, uint8_t *returnData, uint8_t &n) override;

		uint32_t getState() const;
		int setState(uint32_t state);
	protected:

		int setVariable(uint8_t variableId, int32_t data) override;
		int getVariable(uint8_t variableId, int32_t &data) const override;

	private:
		uint16_t dutyCycle;
		uint16_t frequency;
		STRHAL_ADC_Channel_t adcChannel =
		{ 0 };
		STRHAL_ADC_Data_t *adcMeasurement = nullptr;
		STRHAL_GPIO_t cntrlPin;
		STRHAL_ADC_InType_t adcInType = STRHAL_ADC_INTYPE_REGULAR;
		bool hasFeedback = false;

};

#endif /*DIGITALOUTCHANNEL_H*/
