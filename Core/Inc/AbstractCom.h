#ifndef ABSTRACTCOM_H
#define ABSTRACTCOM_H

#include <cstdint>

enum class COMMode : int
{
	STANDARD_COM_MODE,
	BRIDGE_COM_MODE,
	LISTENER_COM_MODE
};

typedef void (*Com_Receptor_t)(uint32_t id, uint8_t *data, uint32_t n);
typedef void (*Com_Heartbeat_t)();

class AbstractCom
{
	public:
		AbstractCom(uint32_t nodeId);
		virtual int init(Com_Receptor_t receptor, Com_Heartbeat_t heartbeat) = 0;
		virtual int exec() = 0;
		virtual int send(uint32_t id, uint8_t *data, uint8_t n) = 0;

	protected:
		uint32_t nodeId;
};

#endif /*ABSTRACTCOM_H*/
