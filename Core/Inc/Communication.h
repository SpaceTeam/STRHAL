#ifndef COMMUNICATION_H
#define COMMUNICATION_H

class GenericChannel;
// Forward declaration to avoid cyclic dependency

#include <AbstractCom.h>
#include "Channels/AbstractChannel.h"
#include "STRHAL.h"
#include <Can.h>
#include <Radio.h>
#include "Modules/LoRa1276F30_Radio.h"

class Communication
{
	public:
		Communication(const Communication &other) = delete;
		Communication& operator=(const Communication &other) = delete;
		Communication(const Communication &&other) = delete;
		Communication& operator=(const Communication &&other) = delete;

		static Communication* instance(GenericChannel *genericChannel = nullptr, LoRa1276F30_Radio *lora = nullptr);

		int init();
		int init(COMMode canMode);
		int exec();

		static void heartbeatLora();
		static void receptor(uint32_t id, uint8_t *data, uint32_t n);

		static constexpr uint32_t MSG_SIZE = 118;
		static constexpr uint8_t ECU_START_ADDR = 0;
		static constexpr uint8_t ECU_MSG_SIZE = 52;
		static constexpr uint8_t PMU_START_ADDR = ECU_START_ADDR + ECU_MSG_SIZE;
		static constexpr uint8_t PMU_MSG_SIZE = 36;
		static constexpr uint8_t RCU_START_ADDR = PMU_START_ADDR + PMU_MSG_SIZE;
		static constexpr uint8_t RCU_MSG_SIZE = 30;

		static Can *can;
		static Radio *radio;
		static GenericChannel *genericChannel;
	private:
		Communication(GenericChannel *genericChannel, LoRa1276F30_Radio *lora);

		//static void receptor(uint32_t id, uint8_t *data, uint32_t n);
		static void receptorLora(uint32_t id, uint8_t *data, uint32_t n);
		static void heartbeatCan();
		//static void heartbeatLora();

		static Communication *com;

		static uint8_t radioArray[MSG_SIZE];
		static bool loraActive;

};

#endif /*COMMUNICATION_H*/
