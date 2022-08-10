#ifndef IOB_H
#define IOB_H

#include <Channels/ADCChannel.h>
#include <Channels/DigitalOutChannel.h>
#include <Channels/DigitalInChannel.h>
#include <Channels/GenericChannel.h>
#include <Can.h>
#include <Speaker.h>

#include <STRHAL.h>

class IOB: public GenericChannel
{
	public:
		IOB(uint32_t node_id, uint32_t fw_version, uint32_t refresh_divider);
		IOB(const IOB &other) = delete;
		IOB& operator=(const IOB &other) = delete;

		int init() override;
		//int reset() override;
		int exec() override;

	private:
		W25Qxx_Flash &flash;

		STRHAL_GPIO_t ledRed, ledGreen;
		Can& can;

		Speaker speaker;
};

#endif /*IOB_H*/
