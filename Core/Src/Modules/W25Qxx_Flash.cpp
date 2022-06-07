#include "../../Inc/Modules/W25Qxx_Flash.h"

#include <STRHAL_QSPI.h>
#include <STRHAL_SysTick.h>
#include <STRHAL.h>

W25Qxx_Flash* W25Qxx_Flash::flash = nullptr;

W25Qxx_Flash::W25Qxx_Flash(uint8_t size_2n) : size_2n(size_2n & 0x3F), pageCount(0), sectorCount(0) {
}

W25Qxx_Flash* W25Qxx_Flash::instance(uint8_t size_2n) {
	if(W25Qxx_Flash::flash == nullptr) {
		W25Qxx_Flash::flash = new W25Qxx_Flash(size_2n);
	}

	return W25Qxx_Flash::flash;
}

int W25Qxx_Flash::init() {
	STRHAL_QSPI_Config_t qspi_conf;
	qspi_conf.clk_level = 0x0;
	qspi_conf.flash_size = size_2n;
	qspi_conf.ncs_high_time = 0x7;
	qspi_conf.psc = 19;

	if(STRHAL_QSPI_Flash_Init(&qspi_conf) < 0)
		return -1;

	STRHAL_QSPI_Run();

	if(!enter4ByteAddrMode()) {
		return -1;
	}

	if(!writeEnable()) {
		return -1;
	}

	if(!disableWPS()) {
		return -1;
	}

	LL_mDelay(10);

	return 0;
}

bool W25Qxx_Flash::readSREG1(uint8_t &sreg1) const {
	STRHAL_QSPI_Command_t cmd;
	cmd.instruction_size = 1;
	cmd.addr_size = 0;
	cmd.alt_size = 0;
	cmd.dummy_size = 0;

	cmd.instruction = 0x05;
	if(STRHAL_QSPI_Indirect_Read(&cmd, &sreg1, 1, 100) != 1)
		return false;

	return true;
}

bool W25Qxx_Flash::readSREG2(uint8_t &sreg2) const {
	STRHAL_QSPI_Command_t cmd;
	cmd.instruction_size = 1;
	cmd.addr_size = 0;
	cmd.alt_size = 0;
	cmd.dummy_size = 0;

	cmd.instruction = 0x35;
	if(STRHAL_QSPI_Indirect_Read(&cmd, &sreg2, 1, 100) != 1)
		return false;

	return true;
}

bool W25Qxx_Flash::readSREG3(uint8_t &sreg3) const {
	STRHAL_QSPI_Command_t cmd;
	cmd.instruction_size = 1;
	cmd.addr_size = 0;
	cmd.alt_size = 0;
	cmd.dummy_size = 0;

	cmd.instruction = 0x15;
	if(STRHAL_QSPI_Indirect_Read(&cmd, &sreg3, 1, 100) != 1)
		return false;

	return true;
}

bool W25Qxx_Flash::readSREGs(uint8_t &sreg1, uint8_t &sreg2, uint8_t &sreg3) const {
	return readSREG1(sreg1) && readSREG2(sreg2) && readSREG3(sreg3);
}

bool W25Qxx_Flash::writeEnable() {
STRHAL_QSPI_Command_t cmd;
	cmd.instruction = 0x06;
	cmd.instruction_size = 1;
	cmd.addr_size = 0;
	cmd.alt_size = 0;
	cmd.dummy_size = 0;

	if(waitForSREGFlag(0x01, false, 100) < 0)
		return false;

	return STRHAL_QSPI_Indirect_Write(&cmd, nullptr, 0, 100) == 0;
}

bool W25Qxx_Flash::writeDisable() {
STRHAL_QSPI_Command_t cmd;
	cmd.instruction = 0x04;
	cmd.instruction_size = 1;
	cmd.addr_size = 0;
	cmd.alt_size = 0;
	cmd.dummy_size = 0;

	return STRHAL_QSPI_Indirect_Write(&cmd, nullptr, 0, 100) == 0;
}

uint8_t  W25Qxx_Flash::readDeviceId() const {
	STRHAL_QSPI_Command_t cmd;
	cmd.instruction = 0xAB;
	cmd.instruction_size = 1;
	cmd.addr_size = 0;
	cmd.alt_size = 0;
	cmd.dummy_size = 3;

	uint8_t device_id;
	if(STRHAL_QSPI_Indirect_Read(&cmd, &device_id, 1, 100) != 1)
		return 0;

	return device_id;
}

uint64_t W25Qxx_Flash::readUniqueId() const {
	STRHAL_QSPI_Command_t cmd;
	cmd.instruction = 0x4B;
	cmd.instruction_size = 1;
	cmd.addr_size = 0;
	cmd.alt_size = 0;
	cmd.dummy_size = 5;

	uint64_t id;
	if(STRHAL_QSPI_Indirect_Read(&cmd, (uint8_t *) &id, 8, 100) != 8)
		return 0;

	return id;
}

bool W25Qxx_Flash::readManufacturerDeviceId(uint8_t &manufacturer, uint8_t &device_id) const {
	STRHAL_QSPI_Command_t cmd;
	cmd.instruction = 0x90;
	cmd.instruction_size = 1;
	cmd.addr_size = 3;
	cmd.addr = 0x000000;
	cmd.alt_size = 0;
	cmd.dummy_size = 0;

	uint8_t tmp[2];
	if(STRHAL_QSPI_Indirect_Read(&cmd, tmp, 2, 100) != 2)
		return false;

	manufacturer = tmp[0];
	device_id = tmp[1];

	return true;
}

bool W25Qxx_Flash::disableWPS() {
	STRHAL_QSPI_Command_t cmd;
	cmd.instruction = 0x11;
	//cmd.instruction = 0x98;
	cmd.instruction_size = 1;
	cmd.addr = 0;
	cmd.addr_size = 0;
	cmd.alt_size = 0;
	cmd.dummy_size = 0;

	uint8_t value = 0xFB;

	if(waitForSREGFlag(0x01, false, 10) < 0)
		return false;

	if(!writeEnable())
		return false;

	if (STRHAL_QSPI_Indirect_Write(&cmd, &value, 1, 100) != 1)
		return false;

	return true;
}

bool W25Qxx_Flash::enter4ByteAddrMode() {
	STRHAL_QSPI_Command_t cmd;
	cmd.instruction = 0xB7;
	cmd.instruction_size = 1;
	cmd.addr_size = 0;
	cmd.addr = 0x000000;
	cmd.alt_size = 0;
	cmd.dummy_size = 0;

	if(STRHAL_QSPI_Indirect_Write(&cmd, nullptr, 0, 100) != 0)
		return false;

	return true;
}

bool W25Qxx_Flash::exit4ByteAddrMode() {
	STRHAL_QSPI_Command_t cmd;
	cmd.instruction = 0xE9;
	cmd.instruction_size = 1;
	cmd.addr_size = 0;
	cmd.addr = 0x000000;
	cmd.alt_size = 0;
	cmd.dummy_size = 0;

	if(STRHAL_QSPI_Indirect_Write(&cmd, nullptr, 0, 100) != 0)
		return false;

	return true;
}

uint32_t W25Qxx_Flash::writeNextPage(const uint8_t * data, uint32_t n) {
	if(sectorCount == 8192-1) {
		return 0;
	}

	if(pageCount == 0) {
		if(!writeEnable())
			return 0;
		if(!sectorErase(sectorCount))
			return 0;
	}

	uint32_t numWritten = write((pageCount << 8) | (sectorCount << 12),data,n);

	if(numWritten == n)
		return 0;

	if(pageCount == 15) {
		pageCount = 0;
		sectorCount++;
	} else {
		pageCount++;
	}

	return n;
}

uint32_t W25Qxx_Flash::write(uint32_t address, const uint8_t *data, uint32_t n) {
	STRHAL_QSPI_Command_t cmd;
	cmd.instruction = 0x12;
	cmd.instruction_size = 1;
	cmd.addr = address;
	cmd.addr_size = 4;
	cmd.alt_size = 0;
	cmd.dummy_size = 0;

	if(waitForSREGFlag(0x01, false, 100) < 0)
		return 0;

	if(!writeEnable())
		return 0;

	if(n > PAGE_SIZE)
		n = PAGE_SIZE;

	if (STRHAL_QSPI_Indirect_Write(&cmd, data, n, 100) != n)
		return 0;

	return n;
}

uint32_t W25Qxx_Flash::read(uint32_t address, uint8_t *data, uint32_t n) {
	STRHAL_QSPI_Command_t cmd;
	cmd.instruction = 0x13;
	cmd.instruction_size = 1;
	cmd.addr = address;
	cmd.addr_size = 4;
	cmd.alt_size = 0;
	cmd.dummy_size = 0;

	if((uint64_t) address + n > (uint64_t) (1<<size_2n))
		n = 0xFFFFFFFF-address;

	if(waitForSREGFlag(0x01, false, 100) < 0)
		return 0;

	if (STRHAL_QSPI_Indirect_Read(&cmd, data, n, 1000) != n)
		return 0;

	return n;
}

bool W25Qxx_Flash::writeConfigReg(Config reg, uint32_t val) {
	return writeConfigRegs(&reg, &val, 1);
}

bool W25Qxx_Flash::writeConfigRegs(Config *reg, uint32_t *val, uint16_t n) {
	if(!readConfig())
		return false;

	for(int i = 0; i < n; i++) {
		config.reg[static_cast<int>(reg[i])] = val[i];
	}

	if(!configErase()) {
		return false;
	}

	return write(CONFIG_BASE, config.bytes, sizeof(config.bytes)) == sizeof(config.bytes);

}

bool W25Qxx_Flash::writeConfigRegsFromAddr(uint32_t startAddress, uint32_t *val, uint16_t n) {
	if(!readConfig())
		return false;

	for(int i = 0; i < n; i++) {
		config.reg[startAddress+i] = val[i];
	}

	if(!configErase()) {
		return false;
	}

	return write(CONFIG_BASE, config.bytes, sizeof(config.bytes)) == sizeof(config.bytes);
}

bool W25Qxx_Flash::readConfig() {
	return read(CONFIG_BASE, config.bytes, sizeof(config.bytes)) == sizeof(config.bytes);
}

// Update Config by calling readConfig() prior to this!
uint32_t W25Qxx_Flash::readConfigReg(Config reg) {
	return config.reg[static_cast<int>(reg)];
}

// Update Config by calling readConfig() prior to this!
uint32_t W25Qxx_Flash::readConfigReg(uint32_t regAddr) {
	return config.reg[regAddr];
}

// Erase config, i.e. sets all config registers to 0xFFFF
bool W25Qxx_Flash::configErase() {
	return sectorErase(CONFIG_BASE >> 12);
}

// Resets config, i.e. sets all config registers to 0x000
bool W25Qxx_Flash::configReset() {
	for(int i = 0; i < PAGE_SIZE/4; i++) {
		config.reg[i] = 0;
	}
	return sectorErase(CONFIG_BASE >> 12) && write(CONFIG_BASE, config.bytes, sizeof(config.bytes)) == sizeof(config.bytes);
}

bool W25Qxx_Flash::sectorErase(uint32_t sector) {
	STRHAL_QSPI_Command_t cmd;
	cmd.instruction = 0x21;
	cmd.instruction_size = 1;
	cmd.addr_size = 4;
	cmd.addr = sector << 12;
	cmd.alt_size = 0;
	cmd.dummy_size = 0;

	if(waitForSREGFlag(0x01, false, 100) < 0)
		return false;

	if(!writeEnable())
		return false;

	if(STRHAL_QSPI_Indirect_Write(&cmd, nullptr, 0, 100) != 0)
		return false;

	if(waitForSREGFlag(0x01, false, 100) < 0)
		return false;

	return true;
}

bool W25Qxx_Flash::chipErase() {
	STRHAL_QSPI_Command_t cmd;
	cmd.instruction = 0xC7;
	cmd.instruction_size = 1;
	cmd.addr_size = 0;
	cmd.addr = 0;
	cmd.alt_size = 0;
	cmd.dummy_size = 0;

	if(waitForSREGFlag(0x01, false, 100) < 0)
		return false;

	if(!writeEnable())
		return false;

	if(STRHAL_QSPI_Indirect_Write(&cmd, nullptr, 0, 100) != 0)
		return false;

	if(waitForSREGFlag(0x01, false, 100) < 0)
		return false;

	return true;
}

int W25Qxx_Flash::waitForSREGFlag(uint8_t flag, bool state, uint16_t tot) {
	uint64_t start = STRHAL_Systick_GetTick();
	uint8_t sreg1;

	if(state) {
		do {
			if(!readSREG1(sreg1))
				return -1;

			if(STRHAL_Systick_GetTick() - start > 100)
				return -1;
		} while(!(sreg1 & flag));
	} else {
		do {
			if(!readSREG1(sreg1))
				return -1;

			if(STRHAL_Systick_GetTick() - start > 100)
				return -1;
		} while(sreg1 & flag);
	}

	return 0;
}

uint32_t W25Qxx_Flash::getPageCount() {
	return pageCount;
}

uint32_t W25Qxx_Flash::getSectorCount() {
	return sectorCount;
}

W25Qxx_Flash::~W25Qxx_Flash() {

}
