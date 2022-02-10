#ifndef ABSTRACTBOARD_H
#define ABSTRACTBOARD_H

#include <cstdint>

enum class BoardState : int {
	INI,
	SBY,
	RUN,
	ERR,
};

class AbstractBoard {
	public:
		AbstractBoard(uint32_t node_id);
		virtual ~AbstractBoard();
		virtual BoardState init() = 0;
		virtual BoardState exec() = 0;

		BoardState getState() const;

	protected:
		uint32_t node_id;
		BoardState state;
};

#endif /*ABSTRACTBOARD_H*/
