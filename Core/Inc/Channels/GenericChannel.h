#ifndef GENERICCHANNEL_H
#define GENERICCHANNEL_H

#include "AbstractChannel.h"
#include <can_houbolt/channels/generic_channel_def.h>
#include <Communication.h>
#include <STRHAL.h>
#include "../Modules/W25Qxx_Flash.h"

class GenericChannel : public AbstractChannel {
	public:
		GenericChannel(uint32_t nodeId, uint32_t firmwareVersion, uint32_t refreshDivider);

		int init() override;
		int reset() override;
		int exec() override; //TODO: implement "smart" scheduler for different CPU

		int processMessage(uint8_t commandId, uint8_t *returnData, uint8_t &n);
		int processMessage(uint8_t commandId, uint8_t *returnData, uint8_t &n, uint8_t channelId);
		int getSensorData(uint8_t *data, uint8_t &n);
		int getNodeInfo(uint8_t *data, uint8_t &n);
		int getFlashClearInfo(uint8_t *data, uint8_t &n);
		int flashClear(uint8_t *data, uint8_t &n);

		uint32_t getNodeId() const;

	protected:
		int setVariable(uint8_t variableId, int32_t data) override;
		int getVariable(uint8_t variableId, int32_t &data) const override;
		void registerChannel(AbstractChannel *channel);
		void registerChannels(AbstractChannel **channels, uint8_t n);

		AbstractChannel *channels[MAX_CHANNELS] = {nullptr};

	private:
		 uint32_t nodeId;
		 uint32_t firmwareVersion;
		 uint32_t channelMask;
		 uint8_t loggingEnabled = 0;

		 W25Qxx_Flash *flash;
};

#endif /*GENERICCHANNEL_H*/
