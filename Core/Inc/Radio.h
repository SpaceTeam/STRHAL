#ifndef RADIO_H
#define RADIO_H

#include "Channels/AbstractChannel.h"
#include "Modules/LoRa1276F30_Radio.h"
#include <AbstractCom.h>
#include "STRHAL.h"

class Radio: public AbstractCom
{
	public:
		Radio(const Radio &other) = delete;
		Radio& operator=(const Radio &other) = delete;
		Radio(const Radio &&other) = delete;
		Radio& operator=(const Radio &&other) = delete;

		static Radio* instance(uint32_t nodeId, LoRa1276F30_Radio *lora);

		int init(Com_Receptor_t receptor, Com_Heartbeat_t heartbeat) override;
		int exec() override;

		int send(uint32_t id, uint8_t *data, uint8_t n) override;

	private:
		static LoRa1276F30_Radio *lora;
		Radio(uint32_t nodeId, LoRa1276F30_Radio *lora);

		static Radio *radio;
};

#endif /*RADIO_H*/
