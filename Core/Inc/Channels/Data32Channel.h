#ifndef DATA32CHANNEL_H
#define DATA32CHANNEL_H

#include "./Channels/AbstractChannel.h"
#include <can_houbolt/channels/data32_channel_def.h>
#include <STRHAL.h>

class Data32Channel: public AbstractChannel
{
	public:
		Data32Channel(uint8_t id, int32_t *dataPtr, uint32_t refreshDivider);

		Data32Channel(const Data32Channel &other) = delete;
		Data32Channel& operator=(const Data32Channel &other) = delete;
		Data32Channel(const Data32Channel &&other) = delete;
		Data32Channel& operator=(const Data32Channel &&other) = delete;

		int init() override;
		int reset() override;
		int exec() override;
		int getSensorData(uint8_t *data, uint8_t &n) override;

		int32_t getMeasurement() const;

		int processMessage(uint8_t commandId, uint8_t *returnData, uint8_t &n) override;

	protected:

		int setVariable(uint8_t variableId, int32_t data) override;
		int getVariable(uint8_t variableId, int32_t &data) const override;

	private:
		int32_t *dataPtr = nullptr;

};

#endif /*DATA32CHANNEL_H*/
