#include "../Inc/Channels/GenericChannel.h"
#include <Radio.h>
#include <cstring>
#include <cstdio>

LoRa1276F30_Radio *Radio::lora = nullptr;
uint8_t Radio::msgArray[MSG_SIZE] =
{ 0 };

Radio::Radio(uint32_t nodeId, LoRa1276F30_Radio& lora) :
		AbstractCom(nodeId)
{
	Radio::lora = &lora;
}

Radio& Radio::instance(uint32_t nodeId, LoRa1276F30_Radio& lora)
{
	static Radio radio(nodeId, lora);

	return radio;
}

int Radio::init(Com_Receptor_t receptor, Com_Heartbeat_t heartbeat)
{
	if (heartbeat)
	{
		if (STRHAL_TIM_Heartbeat_Init(STRHAL_TIM_TIM6, 8000, 10000) != 2)
			return -1;

		if (STRHAL_TIM_Heartbeat_Subscribe(STRHAL_TIM_TIM6, heartbeat) != 0)
			return -1;
	}

	return 0;
}

int Radio::exec()
{
	if (STRHAL_TIM_Heartbeat_StartHeartbeat(STRHAL_TIM_TIM6) != 0)
		return -1;

	return 0;
}

int Radio::send(uint32_t id, uint8_t *data, uint8_t n)
{
	return (lora->sendBytes(data, n)) ? 0 : -1;
}
