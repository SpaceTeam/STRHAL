#ifndef GENERICCHANNEL_H
#define GENERICCHANNEL_H

#include "AbstractChannel.h"
#include "CANCOM.h"
#include <can_houbolt/channels/generic_channel_def.h>
#include <LID.h>

class GenericChannel : public AbstractChannel {
	public:
		GenericChannel(uint32_t node_id, uint32_t fw_version, uint32_t refresh_divider);

		int init() override;
		int reset() override;
		int exec() override; //TODO: implement "smart" scheduler for different CPU

		int prcMsg(uint8_t cmd_id, uint8_t *ret_data, uint8_t &ret_n);
		int prcMsg(uint8_t cmd_id, uint8_t *ret_data, uint8_t &ret_n, uint8_t ch_id);
		int getSensorData(uint8_t *data, uint8_t &n);
		int getNodeInfo(uint8_t *data, uint8_t &n);

		uint32_t getNodeId() const;

	protected:
		int setVar(uint8_t variable_id, int32_t data) override;
		int getVar(uint8_t variable_id, int32_t &data) const override;
		void registerChannel(AbstractChannel *channel);
		void registerChannels(AbstractChannel **channels, uint8_t n);

		AbstractChannel *channels[MAX_CHANNELS] = {nullptr};
		uint8_t n_sens = 0;

	private:
		 uint32_t node_id;
		 uint32_t fw_version;
		 uint32_t channel_msk;
};

#endif /*GENERICCHANNEL_H*/
