#ifndef W25QXX_FLASH_H
#define W25QXX_FLASH_H

#include <stdint.h>

#define PAGE_SIZE			256 //in bytes
/*!< W25Qxx memory map */
#define CONFIG_BASE			0x00000000						//1st sector
#define LOGGING_BASE		(CONFIG_BASE + 0x00001000UL)	//2nd sector

typedef union {
	uint32_t reg[PAGE_SIZE/4];
	uint8_t bytes[PAGE_SIZE];
} ConfigData_t;

enum class Config : int {
	SERVO0_ADC_START = 0,
	SERVO0_ADC_END,
	SERVO0_PWM_START,
	SERVO0_PWM_END,
	SERVO1_ADC_START,
	SERVO1_ADC_END,
	SERVO1_PWM_START,
	SERVO1_PWM_END,
	SERVO2_ADC_START,
	SERVO2_ADC_END,
	SERVO2_PWM_START,
	SERVO2_PWM_END,
};

class W25Qxx_Flash {
	public:
		W25Qxx_Flash(const W25Qxx_Flash &other) = delete;
		W25Qxx_Flash& operator=(const W25Qxx_Flash &other) = delete;
		W25Qxx_Flash(const W25Qxx_Flash &&other) = delete;
		W25Qxx_Flash& operator=(const W25Qxx_Flash &&other) = delete;
		~W25Qxx_Flash();

		static W25Qxx_Flash* instance(uint8_t size_2n);

		int init();

		bool readSREGs(uint8_t &sreg1, uint8_t &sreg2, uint8_t &sreg3) const;
		bool readSREG1(uint8_t &sreg1) const;
		bool readSREG2(uint8_t &sreg2) const;
		bool readSREG3(uint8_t &sreg3) const;

		bool readManufacturerDeviceId(uint8_t &manufacturer, uint8_t &device_id) const;
		uint8_t readDeviceId() const;
		uint64_t readUniqueId() const;

		bool enter4ByteAddrMode();
		bool exit4ByteAddrMode();
		bool disableWPS();

		uint32_t writeNextPage(const uint8_t * data, uint32_t n);
		uint32_t write(uint32_t address, const uint8_t *data, uint32_t n);
		uint32_t read(uint32_t address, uint8_t *data, uint32_t n);
		bool writeConfigRegs(Config *reg, uint32_t *val, uint16_t n);
		bool writeConfigReg(Config reg, uint32_t val);
		uint32_t readConfigReg(Config reg);
		bool readConfig();
		bool writeEnable();
		bool writeDisable();

		bool configErase();
		bool sectorErase(uint32_t sector);
		bool chipErase();

		uint32_t getPageCount();
		uint32_t getSectorCount();

	private:
		uint8_t size_2n;

		uint32_t pageCount;
		uint32_t sectorCount;

		ConfigData_t config;

		W25Qxx_Flash(uint8_t size_2n);

		int waitForSREGFlag(uint8_t flag, bool state, uint16_t tot);

		static W25Qxx_Flash *flash;
};

#endif /*W25QXX_FLASH_H*/
