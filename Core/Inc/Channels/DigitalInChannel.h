#ifndef DIGITALINCHANNEL_H
#define DIGITALINCHANNEL_H

#include "./Channels/AbstractChannel.h"
#include <can_houbolt/channels/adc16_channel_def.h>
#include <LID.h>

class DigitalInChannel : public AbstractChannel {
	public:
		DigitalInChannel(uint8_t channel_id, const LID_GPIO_t &digin_pin, uint32_t refresh_divider);

		DigitalInChannel(const DigitalInChannel &other) = delete;
		DigitalInChannel& operator=(const DigitalInChannel &other) = delete;
		DigitalInChannel(const DigitalInChannel &&other) = delete;
		DigitalInChannel& operator=(const DigitalInChannel &&other) = delete;

		int init() override;
		int reset() override;
		int exec() override;
		int getSensorData(uint8_t *data, uint8_t &n) override;

		uint16_t getMeas() const;

		int prcMsg(uint8_t cmd_id, uint8_t *ret_data, uint8_t &ret_n) override;

	protected:

		int setVar(uint8_t variable_id, int32_t data) override;
		int getVar(uint8_t variable_id, int32_t &data) const override;

	private:
		LID_GPIO_t digin_pin;

};

#endif /*DIGITALINCHANNEL_H*/
