#ifndef ABSTRACTCOM_H
#define ABSTRACTCOM_H

#include <cstdint>

enum class COMState : int {
	INI,
	SBY,
	RUN,
	ERR,
};

class AbstractCom {
	public:
		AbstractCom(uint32_t nodeId);
		virtual ~AbstractCom() {}
		virtual COMState init() = 0;
		virtual COMState exec() = 0;

		COMState getState() const;

	protected:
		uint32_t nodeId;
		COMState state;
};

#endif /*ABSTRACTCOM_H*/
