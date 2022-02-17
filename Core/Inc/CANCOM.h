#ifndef CANCOM_H
#define CANCOM_H

class GenericChannel; // Forward declaration to avoid cyclic dependency

#include "AbstractCOM.h"
#include "Channels/AbstractChannel.h"
#include "LID.h"

class CANCOM : public AbstractCOM {
	public:
		CANCOM(const CANCOM &other) = delete;
		CANCOM& operator=(const CANCOM &other) = delete;
		CANCOM(const CANCOM &&other) = delete;
		CANCOM& operator=(const CANCOM &&other) = delete;
		~CANCOM();

		static CANCOM* instance(GenericChannel *gc = nullptr);

		COMState init() override;
		COMState exec() override;

		COMState subscribe2Node(uint8_t nodeId, AbstractChannel & channel);

	private:
		CANCOM(GenericChannel *gc);
		CANCOM(GenericChannel &gc);

		static GenericChannel *generic_ch;
		static void mainReceptor(uint32_t id, uint8_t *data, uint32_t n);
		static void burner();

		static CANCOM *cancom;

};

#endif /*CANCOM_H*/
