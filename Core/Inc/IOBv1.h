#ifndef IOBv1_H
#define IOBv1_H

#include <Channels/ADCChannel.h>
#include <Channels/DigitalOutChannel.h>
#include <Channels/DigitalInChannel.h>
#include <Channels/GenericChannel.h>
#include <Can.h>
#include <Speaker.h>

#include <STRHAL.h>

class IOBv1: public GenericChannel
{
	public:
		IOBv1(uint32_t node_id, uint32_t fw_version, uint32_t refresh_divider);
		IOBv1(const IOBv1 &other) = delete;
		IOBv1& operator=(const IOBv1 &other) = delete;

		int init() override;
		//int reset() override;
		int exec() override;

	private:
		STRHAL_GPIO_t ledRed, ledGreen;

		Speaker speaker;
};

#endif /*IOB_H*/
