#include <AbstractCom.h>

AbstractCom::AbstractCom(uint32_t nodeId) : nodeId(nodeId), state(COMState::INI) {
}

COMState AbstractCom::getState() const {
	return state;
}
