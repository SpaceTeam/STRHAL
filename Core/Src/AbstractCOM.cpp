#include "../Inc/AbstractCOM.h"

AbstractCOM::AbstractCOM(uint32_t nodeId) : nodeId(nodeId), state(COMState::INI) {
}

COMState AbstractCOM::getState() const {
	return state;
}
