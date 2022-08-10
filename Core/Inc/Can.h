#ifndef CAN_H
#define CAN_H

#include "Channels/AbstractChannel.h"
#include "STRHAL.h"
#include <AbstractCom.h>
//#include <Communication.h>

class Can: public AbstractCom
{
	public:
		Can(const Can &other) = delete;
		Can& operator=(const Can &other) = delete;
		Can(const Can &&other) = delete;
		Can& operator=(const Can &&other) = delete;

		static Can& instance(uint32_t nodeId = 0);

		int init(Com_Receptor_t receptor, Com_Heartbeat_t heartbeat) override;
		int init(Com_Receptor_t receptor, Com_Heartbeat_t heartbeat, COMMode mode);
		int exec() override;

		int send(uint32_t id, uint8_t *data, uint8_t n) override;
		void sendAsMaster(uint8_t receiverNodeId, uint8_t receiverChannelId, uint8_t commandId, uint8_t *data, uint8_t n);

	private:
		Can(uint32_t nodeId);

		static void bridgeReceptor(STRHAL_FDCAN_Id_t bus_id, uint32_t id, uint8_t *data, uint32_t n);
		static void internalReceptor(uint32_t id, uint8_t *data, uint32_t n);
		static void externalReceptor(uint32_t id, uint8_t *data, uint32_t n);

		static Com_Receptor_t standardReceptor;
		static uint32_t _nodeId;

};

#endif /*CAN_H*/
