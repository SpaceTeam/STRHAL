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

		static Radio& instance(uint32_t nodeId, LoRa1276F30_Radio &lora);

		int init(Com_Receptor_t receptor, Com_Heartbeat_t heartbeat) override;
		int exec() override;

		int send(uint32_t id, uint8_t *data, uint8_t n) override;

		static constexpr uint32_t MSG_SIZE = 118;
		static constexpr uint8_t ECU_START_ADDR = 0;
		static constexpr uint8_t ECU_MSG_SIZE = 52;
		static constexpr uint8_t PMU_START_ADDR = ECU_START_ADDR + ECU_MSG_SIZE;
		static constexpr uint8_t PMU_MSG_SIZE = 36;
		static constexpr uint8_t RCU_START_ADDR = PMU_START_ADDR + PMU_MSG_SIZE;
		static constexpr uint8_t RCU_MSG_SIZE = 30;
		static uint8_t msgArray[MSG_SIZE];

	private:
		static LoRa1276F30_Radio *lora;
		Radio(uint32_t nodeId, LoRa1276F30_Radio &lora);
};

#endif /*RADIO_H*/
