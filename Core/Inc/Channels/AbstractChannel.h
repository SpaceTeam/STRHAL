#ifndef ABSTRACTCHANNEL_H
#define ABSTRACTCHANNEL_H

#include <can_houbolt/can_cmds.h>
#include <can_houbolt/cmds.h>

class AbstractChannel
{
	public:
		AbstractChannel(CHANNEL_TYPE type, uint8_t id, uint32_t refreshDivider);
		virtual ~AbstractChannel();

		CHANNEL_TYPE getChannelType() const;
		CHANNEL_STATUS getChannelStatus() const;
		uint8_t getChannelId() const;

		bool IsChannelType(CHANNEL_TYPE t) const;
		bool IsChannelId(uint8_t id) const;
		bool IsRefreshed();

		virtual int init() = 0;
		virtual int reset() = 0;
		virtual int exec() = 0;

		virtual int processMessage(uint8_t commandId, uint8_t *returnData, uint8_t &n);
		virtual int getSensorData(uint8_t *data, uint8_t &n) = 0;

	protected:
		uint32_t refreshDivider, refreshCounter;

		virtual int setVariable(uint8_t variableId, int32_t data) = 0;
		virtual int getVariable(uint8_t variableId, int32_t &data) const = 0;

	private:
		const CHANNEL_TYPE channelType;
		const uint8_t channelId;
		CHANNEL_STATUS channelStatus;
};

#endif /*ABSTRACTCHANNEL_H*/
