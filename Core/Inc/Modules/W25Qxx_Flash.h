#ifndef W25QXX_FLASH_H
#define W25QXX_FLASH_H

#include <stdint.h>
#include <Can.h>
#include "./Modules/AbstractModule.h"

#define PAGE_SIZE			256 //in bytes
#define SECTOR_COUNT		8192 //in bytes
/*!< W25Qxx memory map */
#define CONFIG_BASE			0x00000000						//1st sector
#define LOGGING_BASE		(CONFIG_BASE + 0x00001000UL)	//2nd sector
#define SERVOCONFIG_OFFSET	0								//Offset from Config Base
#define SERVOCONFIG_N_EACH	4								//Number of registers per servo

typedef union
{
		uint32_t reg[PAGE_SIZE / 4];
		uint8_t bytes[PAGE_SIZE];
} ConfigData_t;

enum class Config : int
{
	SERVO0_ADC_START = SERVOCONFIG_OFFSET,
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

enum class FlashState : int
{
	IDLE = 0,
	CLEARING,
	READY,
	LOGGING,
	FULL,
};

class W25Qxx_Flash: public AbstractModule
{
	public:
		W25Qxx_Flash(W25Qxx_Flash const&);
		void operator=(W25Qxx_Flash const&);

		static W25Qxx_Flash& instance();

		int init() override;
		int exec() override;
		int reset() override;

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

		uint32_t writeNextPage(const uint8_t *data, uint32_t n);
		uint32_t write(uint32_t address, const uint8_t *data, uint32_t n);
		uint32_t read(uint32_t address, uint8_t *data, uint32_t n);
		bool writeConfigRegsFromAddr(uint32_t startAddress, uint32_t *val, uint16_t n);
		bool writeConfigRegs(Config *reg, uint32_t *val, uint16_t n);
		bool writeConfigReg(Config reg, uint32_t val);
		bool writeConfig();
		uint32_t readConfigReg(Config reg);
		uint32_t readConfigReg(uint32_t regAddr);
		bool readConfig();
		bool writeEnable();
		bool writeDisable();

		bool configErase();
		bool configReset();
		bool sectorErase(uint32_t sector);
		bool chipErase();

		uint32_t getPageCount();
		uint32_t getSectorCount();

		void setState(FlashState nextState);
		FlashState getState();
		bool sendClearDone();
		bool sendFull();
		void addLog(uint8_t *data, uint8_t n);

		bool lock = false;

		static constexpr uint16_t EXEC_SAMPLE_TICKS = 10;
		static constexpr uint8_t SIZE_2N = 0x1F;
	private:
		FlashState state;

		uint32_t pageCount;
		uint32_t sectorCount;

		ConfigData_t config;

		uint64_t timeLastSample = 0;
		uint64_t timeLastTransition = 0;
		FlashState internalNextState = FlashState::IDLE;
		FlashState externalNextState = FlashState::IDLE;

		uint8_t loggingBuffer[256];
		uint8_t loggingIndex = 0;

		W25Qxx_Flash();

		int waitForSREGFlag(uint8_t flag, bool state, uint16_t tot);

		FlashState currentStateLogic(uint64_t time);
		void nextStateLogic(FlashState nextState, uint64_t time);

		Can& can;
};

#endif /*W25QXX_FLASH_H*/
