#ifndef LCB_H
#define LCB_H

#include <Channels/ADCChannel.h>
#include <Channels/DigitalOutChannel.h>
#include <Channels/DigitalInChannel.h>
#include <Channels/GenericChannel.h>
#include <Can.h>
#include <Speaker.h>

#include <STRHAL.h>

class LCB: public GenericChannel
{
	public:
		LCB(uint32_t node_id, uint32_t fw_version, uint32_t refresh_divider);
		LCB(const LCB &other) = delete;
		LCB& operator=(const LCB &other) = delete;

		int init() override;
		//int reset() override;
		int exec() override;

	private:
		W25Qxx_Flash &flash;

		STRHAL_GPIO_t ledRed, ledGreen;
		Can& can;
		Speaker speaker;
};

#endif /*LCB_H*/
