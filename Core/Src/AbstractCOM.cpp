#include "../Inc/AbstractCOM.h"

AbstractCOM::AbstractCOM(uint32_t node_id) : node_id(node_id), state(COMState::INI) {
}

COMState AbstractCOM::getState() const {
	return state;
}
