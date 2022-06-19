#ifndef LORA1276F30_RADIO_H
#define LORA1276F30_RADIO_H

#include <STRHAL.h>

enum class LoraAddr : uint8_t {
	FIFO                 = 0x00,
	OP_MODE              = 0x01,
	FRF_MSB              = 0x06,
	FRF_MID              = 0x07,
	FRF_LSB              = 0x08,
	PA_CONFIG            = 0x09,
	LNA                  = 0x0c,
	FIFO_ADDR_PTR        = 0x0d,
	FIFO_TX_BASE_ADDR    = 0x0e,
	FIFO_RX_BASE_ADDR    = 0x0f,
	FIFO_RX_CURRENT_ADDR = 0x10,
	IRQ_FLAGS            = 0x12,
	RX_NB_BYTES          = 0x13,
	PKT_RSSI_VALUE       = 0x1a,
	PKT_SNR_VALUE        = 0x1b,
	MODEM_CONFIG_1       = 0x1d,
	MODEM_CONFIG_2       = 0x1e,
	PREAMBLE_MSB         = 0x20,
	PREAMBLE_LSB         = 0x21,
	PAYLOAD_LENGTH       = 0x22,
	RSSI_WIDEBAND        = 0x2c,
	DETECTION_OPTIMIZE   = 0x31,
	DETECTION_THRESHOLD  = 0x37,
	ERRATA1				 = 0x36,
	ERRATA2              = 0x3A,
	SYNC_WORD            = 0x39,
	DIO_MAPPING_1        = 0x40,
	VERSION              = 0x42,
	TEMP				 = 0x3C,
	OCP					 = 0x0B,
	LR_RegHopPeriod      = 0x24,
};

enum class LoraMode : uint8_t {
	LONG_RANGE_MODE     = 0x80,
	SLEEP               = 0x00,
	STDBY               = 0x01,
	TX                  = 0x03,
	RX_CONTINUOUS       = 0x05,
	RX_SINGLE           = 0x06,
};

enum class LoraIRQ : uint8_t {
	TX_DONE_MASK           = 0x08,
	PAYLOAD_CRC_ERROR_MASK = 0x21,
	RX_DONE_MASK           = 0x40,
};

enum class LoraState : uint8_t {
	SENDING,
	RECEIVING,
	IDLE,
	DISCONNECTED,
	SLEEPING,
};

class LoRa1276F30_Radio {
	public:
		LoRa1276F30_Radio(const STRHAL_SPI_Id_t &spiId, const STRHAL_SPI_Config_t &spiConf, const STRHAL_GPIO_t &dio1, const STRHAL_GPIO_t &dio3);
		//LPS25HB_Baro(const LPS25HB_Baro &other) = delete;
		//LPS25HB_Baro& operator=(const LPS25HB_Baro &other) = delete;

		int init();
		int reset();

		uint8_t whoAmI();
		uint8_t getMode();
		LoraState getState();
		bool setState(LoraState state);
		bool ready();

		int sendBytes(uint8_t* buffer, uint8_t length);
		int16_t read();
		uint16_t readBytes(uint8_t *data, uint16_t length);
		uint8_t available();

		static constexpr uint32_t MAX_PKT_LENGTH = 255;
		static constexpr uint32_t PKT_LENGTH = 105; // 3 + 35 + 25 + 47
		static constexpr uint32_t FREQUENCY = 433E6;
		static constexpr uint16_t PREAMBLE_LENGTH = 8;
		static constexpr uint8_t SPREADING_FACTOR = 10;
		static constexpr uint32_t SIGNAL_BANDWIDTH = 500E3;
		static constexpr uint8_t TX_POWER = 17;
		static constexpr uint8_t SYNC_WORD = 0xE4;
		static constexpr uint8_t CODING_RATE = 6;
		static constexpr uint8_t WRITE_BIT = 0x80;
	private:
		bool readReg(const LoraAddr &address, uint8_t *reg, uint8_t n = 0);
		uint8_t getReg(const LoraAddr &address);
		bool writeReg(const LoraAddr &address, uint8_t reg, uint16_t delay = 0);
		bool writeRegSafe(const LoraAddr &address, uint8_t reg, uint16_t delay);
		bool writeFifo(const LoraAddr &address, uint8_t *data, uint8_t n);

		bool setTxPower(uint8_t level);
		bool setFrequency(uint32_t frequency);
		bool setSpreadingFactor(uint8_t sf);
		bool setSignalBandwidth(uint32_t sbw);
		bool setCodingRate4(uint8_t denominator);
		bool setPreambleLength(uint16_t length);
		bool setSyncWord(uint8_t sw);
		bool enableCRC();
		bool disableCRC();
		bool explicitHeaderMode();
		bool implicitHeaderMode();

		STRHAL_SPI_Id_t spiId;
		STRHAL_SPI_Config_t spiConf;
		const STRHAL_GPIO_t dio1, dio3;
		uint16_t _packetIndex = 0;

};

#endif /*LORA1276F30_RADIO*/
