#include "../Inc/ECU.h"

ECU::ECU(uint32_t node_id) : AbstractBoard(node_id) {

}

BoardState ECU::init() {
	state = BoardState::SBY;
	return state;
}

BoardState ECU::exec() {
	state = BoardState::RUN;
	while(1) {

	}
	return state;
}
