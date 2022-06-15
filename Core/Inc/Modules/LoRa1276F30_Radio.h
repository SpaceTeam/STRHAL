#ifndef LORA1276F30_RADIO_H
#define LORA1276F30_RADIO_H

#include <STRHAL.h>

class LoRa1276F30_Radio {
	public:
		LoRa1276F30_Radio(const STRHAL_SPI_Id_t &spiId, const STRHAL_SPI_Config_t &spiConf, const STRHAL_GPIO_t &dio1, const STRHAL_GPIO_t &dio3);
		//LPS25HB_Baro(const LPS25HB_Baro &other) = delete;
		//LPS25HB_Baro& operator=(const LPS25HB_Baro &other) = delete;

		int init();
		int reset();

		static constexpr uint32_t BUF_DATA_SIZE = 64;
		static constexpr uint8_t READ_BIT = 0x80;
	private:
		//bool readReg(const BaroAddr &address, uint8_t *reg, uint8_t n = 0);
		//bool writeReg(const BaroAddr &address, uint8_t reg, uint16_t delay = 0);

		STRHAL_SPI_Id_t spiId;
		STRHAL_SPI_Config_t spiConf;
		const STRHAL_GPIO_t dio1, dio3;

};

#endif /*LORA1276F30_RADIO*/
