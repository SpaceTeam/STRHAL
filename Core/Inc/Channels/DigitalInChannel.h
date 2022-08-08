#ifndef DIGITALINCHANNEL_H
#define DIGITALINCHANNEL_H

#include "./Channels/AbstractChannel.h"
#include <can_houbolt/channels/adc16_channel_def.h>
#include <STRHAL.h>

class DigitalInChannel: public AbstractChannel
{
	public:
		DigitalInChannel(uint8_t id, const STRHAL_GPIO_t &digitalInPin, uint32_t refreshDivider);

		DigitalInChannel(const DigitalInChannel &other) = delete;
		DigitalInChannel& operator=(const DigitalInChannel &other) = delete;
		DigitalInChannel(const DigitalInChannel &&other) = delete;
		DigitalInChannel& operator=(const DigitalInChannel &&other) = delete;

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
		STRHAL_GPIO_t digitalInPin;

};

#endif /*DIGITALINCHANNEL_H*/
