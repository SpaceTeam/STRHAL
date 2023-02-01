#ifndef IOBv3_H
#define IOBv3_H

#include <Channels/ADCChannel.h>
#include <Channels/DigitalOutChannel.h>
#include <Channels/DigitalInChannel.h>
#include <Channels/GenericChannel.h>
#include <Can.h>
#include <Speaker.h>

#include <STRHAL.h>

class IOBv3: public GenericChannel
{
	public:
		IOBv3(uint32_t node_id, uint32_t fw_version, uint32_t refresh_divider);
		IOBv3(const IOBv3 &other) = delete;
		IOBv3& operator=(const IOBv3 &other) = delete;

		int init() override;
		//int reset() override;
		int exec() override;

	private:
		STRHAL_GPIO_t ledRed, ledGreen;
		DigitalOutChannel channel0, channel1, channel2, channel3, channel4, channel5, channel6, channel7, channel8, channel9, channel10, channel11;
		Speaker speaker;
};

#endif /*IOB_H*/
