#ifndef ECU_H
#define ECU_H

#include "AbstractBoard.h"

class ECU : public AbstractBoard {
	public:
		ECU(uint32_t node_id);
		ECU(const ECU &other) = delete;
		ECU& operator=(const ECU &other) = delete;


		BoardState init() override;
		BoardState exec() override;

	private:

};

#endif /*ECU_H*/
