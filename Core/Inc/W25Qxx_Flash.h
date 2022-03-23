#ifndef W25QXX_FLASH_H
#define W25QXX_FLASH_H

#include <stdint.h>



class W25Qxx_Flash {
	public:
		W25Qxx_Flash(uint8_t size_2n, uint32_t page_size);
		~W25Qxx_Flash();

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

		uint32_t writeCurrentPage(const uint8_t * data, uint32_t n);
		uint32_t read(uint32_t address, uint8_t *data, uint32_t n);
		bool writeEnable();
		bool writeDisable();
		bool sectorErase(uint32_t sector);
		bool chipErase();

		uint32_t getPageCount();
		uint32_t getSectorCount();

	private:
		uint8_t size_2n;
		uint32_t page_size;

		uint32_t pageCount;
		uint32_t sectorCount;

		int waitForSREGFlag(uint8_t flag, bool state, uint16_t tot);
};

#endif /*W25QXX_FLASH_H*/
