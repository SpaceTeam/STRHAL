#ifndef LPS25HB_BARO_H
#define LPS25HB_BARO_H

#include <STRHAL.h>
#include "./Modules/AbstractModule.h"

enum class BaroAddr : uint8_t
{
	REF_P_XL = 0x08,
	REF_P_L = 0x09,
	REF_P_H = 0x0A,
	WHO_AM_I = 0x0F,
	RES_CONF = 0x10,
	CTRL_REG1 = 0x20,
	CTRL_REG2 = 0x21,
	CTRL_REG3 = 0x22,
	CTRL_REG4 = 0x23,
	INTERRUPT_CGF = 0x24,
	STATUS_REG = 0x27,
	PRESS_OUT_XL = 0x28,
	PRESS_OUT_L = 0x29,
	PRESS_OUT_H = 0x2A,
	TEMP_OUT_L = 0x2B,
	TEMP_OUT_H = 0x2C,
	FIFO_CTRL = 0x2E,
};

class LPS25HB_Baro: public AbstractModule
{
	public:
		LPS25HB_Baro(const STRHAL_SPI_Id_t &spiId, const STRHAL_SPI_Config_t &spiConf, const STRHAL_GPIO_t &dataReadyPin);
		//LPS25HB_Baro(const LPS25HB_Baro &other) = delete;
		//LPS25HB_Baro& operator=(const LPS25HB_Baro &other) = delete;

		int init() override;
		int exec() override;
		int reset() override;

		int read();
		bool measurementReady();
		void getMeasurement(int32_t &measurement);
		uint8_t whoAmI() const;

		static constexpr uint32_t BUF_DATA_SIZE = 64;
		static constexpr uint8_t READ_BIT = 0x80;

		static constexpr uint64_t EXEC_SAMPLE_TICKS = 40;

	private:
		bool readReg(const BaroAddr &address, uint8_t *reg, uint8_t n = 0);
		bool writeReg(const BaroAddr &address, uint8_t reg, uint16_t delay = 0);

		STRHAL_SPI_Id_t spiId;
		STRHAL_SPI_Config_t spiConf;
		const STRHAL_GPIO_t dataReadyPin;

		int32_t measData[BUF_DATA_SIZE] =
		{ 0 };
		uint32_t measDataTail = 0;
		uint32_t measDataNum = 0;

		uint64_t timeLastSample = 0;

};

#endif /*LPS25HB_BARO_H*/
