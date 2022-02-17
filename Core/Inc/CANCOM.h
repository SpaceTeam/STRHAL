#ifndef CANCOM_H
#define CANCOM_H

class GenericChannel; // Forward declaration to avoid cyclic dependency

#include "AbstractCOM.h"
#include "Channels/AbstractChannel.h"
#include "LID.h"

class CANCOM : public AbstractCOM {
	public:
		CANCOM(uint32_t node_id, GenericChannel & generic_ch);
		CANCOM(const CANCOM &other) = delete;
		CANCOM& operator=(const CANCOM &other) = delete;

		COMState init() override;
		COMState exec() override;

		COMState subscribe2Node(uint8_t nodeId, AbstractChannel & channel);

	private:
		static GenericChannel *generic_ch;
		static uint32_t _node_id;
		static void mainReceptor(uint32_t id, uint8_t *data, uint32_t n);
		static void burner();

};

#endif /*CANCOM_H*/
