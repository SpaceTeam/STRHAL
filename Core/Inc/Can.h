#ifndef CANCOM_H
#define CANCOM_H

class GenericChannel; // Forward declaration to avoid cyclic dependency

#include <AbstractCom.h>
#include "Channels/AbstractChannel.h"
#include "STRHAL.h"

class Can : public AbstractCom {
	public:
		Can(const Can &other) = delete;
		Can& operator=(const Can &other) = delete;
		Can(const Can &&other) = delete;
		Can& operator=(const Can &&other) = delete;
		~Can();

		static Can* instance(GenericChannel *genericChannel = nullptr);

		COMState init() override;
		COMState exec() override;

		void sendAsMaster(uint8_t receiverNodeId, uint8_t receiverChannelId, uint8_t commandId, uint8_t *data, uint8_t n);

		COMState subscribe2Node(uint8_t nodeId, AbstractChannel & channel);

	private:
		Can(GenericChannel *genericChannel);

		static GenericChannel *genericChannel;
		static void mainReceptor(uint32_t id, uint8_t *data, uint32_t n);
		static void heartbeat();

		static Can *cancom;

};

#endif /*CANCOM_H*/
