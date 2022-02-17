#ifndef ABSTRACTCOM_H
#define ABSTRACTCOM_H

#include <cstdint>

enum class COMState : int {
	INI,
	SBY,
	RUN,
	ERR,
};

class AbstractCOM {
	public:
		AbstractCOM(uint32_t node_id);
		virtual ~AbstractCOM() {}
		virtual COMState init() = 0;
		virtual COMState exec() = 0;

		COMState getState() const;

	protected:
		uint32_t node_id;
		COMState state;
};

#endif /*ABSTRACTCOM_H*/
