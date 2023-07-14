#ifndef GENERICCHANNEL_H
#define GENERICCHANNEL_H

#include "AbstractChannel.h"
#include "AbstractControlInputChannel.h"

#include <can_houbolt/channels/generic_channel_def.h>
#include <STRHAL.h>
#include <Radio.h>
#include "../Modules/W25Qxx_Flash.h"
#include <Can.h>

class GenericChannel: public AbstractChannel
{
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
		void printLog();
		void detectReadoutMode();
		AbstractControlInputChannel* getControlInputChannel(uint8_t id);


		uint32_t getNodeId() const;

		static constexpr uint8_t MAX_MODULES = 8;

	protected:
		int setVariable(uint8_t variableId, int32_t data) override;
		int getVariable(uint8_t variableId, int32_t &data) const override;
		void registerChannel(AbstractChannel *channel);
		void registerChannels(AbstractChannel **channels, uint8_t n);
		void registerModule(AbstractModule *module);

		AbstractChannel *channels[MAX_CHANNELS] =
		{ nullptr };
		AbstractModule *modules[MAX_MODULES] =
		{ nullptr };

		static GenericChannel* gcPtr;
		static Radio* radioPtr;
		static bool loraActive;
		static void setLoraActive(bool enable);
		static void heartbeatCan();
		static void heartbeatLora();
		static void receptor(uint32_t id, uint8_t *data, uint32_t n);
		static void receptorLora(uint32_t id, uint8_t *data, uint32_t n);

		Can& can;
		W25Qxx_Flash& flash;

	private:
		uint32_t nodeId;
		uint32_t firmwareVersion;
		uint32_t channelMask;
		uint8_t loggingEnabled = 0;
		uint8_t moduleIndex = 0;
};

#endif /*GENERICCHANNEL_H*/
