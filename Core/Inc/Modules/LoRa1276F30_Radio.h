#ifndef LORA1276F30_RADIO_H
#define LORA1276F30_RADIO_H

#include <STRHAL.h>

enum class LoraOpcode : uint8_t
{
	SET_SLEEP = 0x84,
	SET_STANDBY = 0x80,
	SET_FS = 0xC1,
	SET_TX = 0x83,
	SET_RX = 0x82,
	STOP_TIMER_ON_PREAMBE = 0x9F,
	SET_RX_DUTY_CYCLE = 0x94,
	SET_CAD = 0x0d,
	SET_TX_CONT_WAVE = 0xD1,
	SET_TX_INF_PREAMBLE = 0xD2,
	SET_REGULATOR_MODE = 0x96,
	CALIBRATE = 0x89,
	CALIBRATE_IMAGE = 0x98,
	SET_PA_CONFIG = 0x95,
	SET_RX_TX_FALLBACK_MODE = 0x93,
	WRITE_REGISTER = 0x0D,
	READ_REGISTER = 0x1D,
	WRITE_BUFFER = 0x0E,
	READ_BUFFER = 0x1E,
	SET_DIO_IRQ_PARAM = 0x08,
	GET_IRQ_STATUS = 0x12,
	CLEAR_IRQ_STATUS = 0x02,
	SET_DIO2_AS_RF_SWITCH = 0x9D,
	SET_DIO3_AS_TCX0 = 0x97,
	SET_FREQUENCY = 0x86,
	SET_PACKET_TYPE = 0x8A,
	GET_PACKET_TYPE = 0x11,
	SET_TX_PARAMS = 0x8E,
	SET_MODULATION_PARAM = 0x8B,
	SET_PACKET_PARAM = 0x8C,
	SET_CAD_PARAM = 0x88,
	SET_BUFFER_BASE_ADDR = 0x8F,
	SET_LORA_SYMB_NUM_TO = 0xA0,
	GET_STATUS = 0xC0,
	GET_RSSI_INST = 0x15,
	GET_RX_BUF_STATUS = 0x13,
	GET_PACKET_STATUS = 0x14,
	GET_DEVICE_ERRORS = 0x17,
	CLEAR_DEVICE_ERRORS = 0x07,
	GET_STATS = 0x10,
	RESET_STATS = 0x00,
};

enum class LoraAddr : uint16_t
{
	HOPPING_ENABLE = 0x0385,
	PACKET_LENGTH = 0x0386,
	NUM_HOPPING_BLOCKS = 0x0387,
	NUM_SYMBOLS_0 = 0x0388,
	FREQ_0 = 0x038A,
	NUM_SYMBOLS_15 = 0x03E2,
	FREQ_15 = 0x03E4,
	DIOX_OUTPUT_ENA = 0x0580,
	DIOX_INPUT_ENA = 0x0583,
	DIOX_PULL_UP_CNTRL = 0x0584,
	DIOX_PULL_DOWN_CNTRL = 0x0585,
	WHITENING_INIT_MSB = 0x06B8,
	WHITENING_INIT_LSB = 0x06B9,
	CRC_INIT_MSB = 0x06BC,
	CRC_INIT_LSB = 0x06BD,
	CRC_POLY_MSB = 0x06BE,
	CRC_POLY_LSB = 0x06BF,
	SYNC_WORD_MSB = 0x0740,
	SYNC_WORD_LSB = 0x0741,
	TX_MODULATION = 0x0889,
	RX_GAIN = 0x08AC,
	TX_CLAMP_CONFIG = 0x08D8,
	RTC_CONTROL = 0x0902,
	XTA_TRIM = 0x0911,
	XTB_TRIM = 0x0912,
	DIO3_OUTPUT_VOLT_CNTRL = 0x0920,
	EVENT_MASK = 0x0944,
};

class LoRa1276F30_Radio
{
	public:
		LoRa1276F30_Radio(const STRHAL_SPI_Id_t &spiId, const STRHAL_SPI_Config_t &spiConf, const STRHAL_GPIO_t &dio1, const STRHAL_GPIO_t &dio3, const STRHAL_GPIO_t &busyPin);
		//LPS25HB_Baro(const LPS25HB_Baro &other) = delete;
		//LPS25HB_Baro& operator=(const LPS25HB_Baro &other) = delete;

		int init();
		int reset();

		bool isBusy();

		bool readReg(const LoraAddr &address, uint8_t *reg, uint8_t n = 0);
		uint8_t getReg(const LoraAddr &address);
		uint8_t getStatus();
		uint8_t getErrors();
		bool writeReg(const LoraAddr &address, uint8_t reg, uint16_t delay = 0);
		bool writeCommand(const LoraOpcode &opcode, uint8_t *parameter, uint8_t n, uint16_t delay = 0);

		bool sendBytes(uint8_t *buffer, uint8_t n);

		static constexpr uint32_t MAX_PKT_LENGTH = 255;
		static constexpr uint32_t PKT_LENGTH = 118; // 3 + 31+4(PMU) + 25+4(RCU) + 47+4(ECU)
		static constexpr float FREQUENCY = 433.0;
		static constexpr uint16_t PREAMBLE_LENGTH = 8;
		static constexpr uint8_t SPREADING_FACTOR = 7;
		static constexpr uint32_t SIGNAL_BANDWIDTH = 250E3;
		static constexpr uint8_t TX_POWER = 17;
		static constexpr uint8_t SYNC_WORD = 0xE4;
		static constexpr uint8_t CODING_RATE = 6;
		static constexpr uint8_t CRC_ENABLED = 0x01;
	private:
		bool setPacketType();
		bool setRFFrequency();
		bool setPAConfig();
		bool SetTxParams();
		bool SetBufferBaseAddress();
		bool setModulationParams();
		bool setPacketParams();
		bool setDioIrqParams();
		bool calibrateImage();
		void waitForBusy();
		bool setTx();

		STRHAL_SPI_Id_t spiId;
		STRHAL_SPI_Config_t spiConf;
		const STRHAL_GPIO_t dio1, dio3, busyPin;

};

#endif /*LORA1276F30_RADIO*/
