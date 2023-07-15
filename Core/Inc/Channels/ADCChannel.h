#ifndef ADCCHANNEL_H
#define ADCCHANNEL_H

#include "./Channels/AbstractChannel.h"
#include <can_houbolt/channels/adc16_channel_def.h>
#include <STRHAL.h>

class ADCChannel: public AbstractChannel
{
	public:
		ADCChannel(uint8_t id, const STRHAL_ADC_Channel_t adcChannel, uint32_t refreshDivider);
		ADCChannel(uint8_t id, STRHAL_ADC_Data_t *adcPtr, uint32_t refreshDivider);

		ADCChannel(const ADCChannel &other) = delete;
		ADCChannel& operator=(const ADCChannel &other) = delete;
		ADCChannel(const ADCChannel &&other) = delete;
		ADCChannel& operator=(const ADCChannel &&other) = delete;

		int init() override;
		int reset() override;
		int exec() override;
		int getSensorData(uint8_t *data, uint8_t &n) override;

		uint16_t getMeasurement() const;

		int processMessage(uint8_t commandId, uint8_t *returnData, uint8_t &n) override;

	protected:

		int setVariable(uint8_t variableId, int32_t data) override;
		int getVariable(uint8_t variableId, int32_t &data) const override;

	private:
		STRHAL_ADC_Channel_t adcChannel =
		{ nullptr, STRHAL_ADC_CHANNEL_0 }; // init with nullptr as ADC to handle non-ADC channels
		STRHAL_ADC_Data_t *adcMeasurement = nullptr;

};

#endif /*ADCCHANNEL_H*/
