#include "../Inc/AbstractBoard.h"

AbstractBoard::AbstractBoard(uint32_t node_id) : node_id(node_id), state(BoardState::INI) {
}

BoardState AbstractBoard::getState() const {
	return state;
}
