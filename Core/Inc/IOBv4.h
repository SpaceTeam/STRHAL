#ifndef IOBv4_H
#define IOBv4_H

#include <Channels/ADCChannel.h>
#include <Channels/DigitalOutChannel.h>
#include <Channels/ServoChannel.h>
#include <Channels/DigitalInChannel.h>
#include <Channels/GenericChannel.h>
#include <Can.h>
#include <Speaker.h>

#include <STRHAL.h>

class IOBv4: public GenericChannel
{
	public:
		IOBv4(uint32_t node_id, uint32_t fw_version, uint32_t refresh_divider);
		IOBv4(const IOBv4 &other) = delete;
		IOBv4& operator=(const IOBv4 &other) = delete;

		int init() override;
		//int reset() override;
		int exec() override;

	private:
		STRHAL_GPIO_t led1, led2, led_debug;
		ServoChannel /*servo0,*/ servo1, servo2;
		DigitalOutChannel channel0, /*channel1, channel2,*/ channel3, channel4, channel5, channel6, channel7, channel8, channel9, channel10, channel11;//, channel12;
		Speaker speaker;
};

#endif /*IOB_H*/
