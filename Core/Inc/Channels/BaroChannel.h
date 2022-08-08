#ifndef BAROCHANNEL_H
#define BAROCHANNEL_H

#include "AbstractChannel.h"
#include <can_houbolt/channels/adc24_channel_def.h>
#include "../Modules/LPS25HB_Baro.h"
#include <STRHAL.h>

class BaroChannel: public AbstractChannel
{
	public:
		BaroChannel(uint8_t id, LPS25HB_Baro &baro, uint32_t refreshDivider);

		BaroChannel(const BaroChannel &other) = delete;
		BaroChannel& operator=(const BaroChannel &other) = delete;
		BaroChannel(const BaroChannel &&other) = delete;
		BaroChannel& operator=(const BaroChannel &&other) = delete;

		~BaroChannel();

		int init() override;
		int reset() override;
		int exec() override;

		int getSensorData(uint8_t *data, uint8_t &n) override;

		int processMessage(uint8_t commandId, uint8_t *returnData, uint8_t &n) override;

	protected:
		int setVariable(uint8_t variableId, int32_t data) override;
		int getVariable(uint8_t variableId, int32_t &data) const override;

	private:
		LPS25HB_Baro baro;
};

#endif /*BAROCHANNEL_H*/
