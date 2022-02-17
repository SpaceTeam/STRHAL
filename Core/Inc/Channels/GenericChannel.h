#ifndef GENERICCHANNEL_H
#define GENERICCHANNEL_H

#include "AbstractChannel.h"
#include "CANCOM.h"
#include <can_houbolt/channels/generic_channel_def.h>
#include <LID.h>

class GenericChannel : public AbstractChannel {
	public:
		GenericChannel(uint32_t node_id, uint32_t fw_version);

		int init();
		int reset();
		int exec();

		int prcMsg(uint8_t cmd_id, uint8_t variable_id, uint32_t data, uint32_t &ret);
		int prcMsg(uint8_t cmd_id, uint8_t variable_id, uint32_t data, uint32_t &ret, uint8_t ch_id);
		int getSensorData(uint8_t *data, uint8_t &n);

	protected:
		int setVar(uint8_t variable_id, uint32_t data) override;
		int getVar(uint8_t variable_id, uint32_t &data) const override;
		int addChannel(AbstractChannel &channel);

		AbstractChannel *channels[MAX_CHANNELS] = {nullptr};
		CANCOM cancom;
		uint8_t n_sens;

	private:
		 uint32_t node_id;
		 uint32_t fw_version;
};

#endif /*GENERICCHANNEL_H*/
