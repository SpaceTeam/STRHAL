#ifndef HARDWARE_FLASH_H_
#define HARDWARE_FLASH_H_

#include <stdint.h>

// W25Q256JV Instruction Set (Standard SPI, 4-Byte Address Mode):
#define FLASH_WRITE_ENABLE					0x06
#define FLASH_VOLATILE_SR_WRITE_ENABLE		0x50
#define FLASH_WRITE_DISABLE					0x04
#define FLASH_RELEASE_POWER_DOWN_ID			0xAB
#define FLASH_MANUFACTURER_DEVICE_ID		0x90
#define FLASH_JEDEC_ID						0x9F
#define FLASH_READ_UNIQUE_ID				0x4B
#define FLASH_READ_DATA						0x03
#define FLASH_READ_DATA_WITH_4_BYTE_ADDRESS			0x13
#define FLASH_FAST_READ								0x0B
#define FLASH_FAST_READ_WITH_4_BYTE_ADDRESS			0x0C
#define FLASH_PAGE_PROGRAM							0x02
#define FLASH_PAGE_PROGRAM_WITH_4_BYTE_ADDRESS		0x12
#define FLASH_SECTOR_ERASE_4_KB						0x20
#define FLASH_SECTOR_ERASE_4_KB_WITH_4_BYTE_ADDRESS	0x21
#define FLASH_BLOCK_ERASE_32_KB						0x52
#define FLASH_BLOCK_ERASE_64_KB						0xD8
#define FLASH_BLOCK_ERASE_64_KB_WITH_4_BYTE_ADDRESS	0xDC
#define FLASH_CHIP_ERASE					0xC7 // /0x60 ?
#define FLASH_READ_STATUS_REGISTER_1		0x05
#define FLASH_WRITE_STATUS_REGISTER_1		0x01
#define FLASH_READ_STATUS_REGISTER_2		0x35
#define FLASH_WRITE_STATUS_REGISTER_2		0x31
#define FLASH_READ_STATUS_REGISTER_3		0x15
#define FLASH_WRITE_STATUS_REGISTER_3		0x11
#define FLASH_READ_EXTENDED_ADDR_REG		0xC8
#define FLASH_WRITE_EXTENDED_ADDR_REG		0xC5
#define FLASH_READ_SFDP_REGISTER			0x5A
#define FLASH_ERASE_SECURITY_REGISTER		0x44
#define FLASH_PROGRAM_SECURITY_REGISTER		0x42
#define FLASH_READ_SECURITY_REGISTER		0x48
#define FLASH_GLOBAL_BLOCK_LOCK				0x7E
#define FLASH_GLOBAL_BLOCK_UNLOCK			0x98
#define FLASH_READ_BLOCK_LOCK				0x3D
#define FLASH_INDIVIDUAL_BLOCK_LOCK			0x36
#define FLASH_INDIVIDUAL_BLOCK_UNLOCK		0x39
#define FLASH_ERASE_PROGRAM_SUSPEND			0x75
#define FLASH_ERASE_PROGRAM_RESUME			0x7A
#define FLASH_POWER_DOWN					0xB9
#define FLASH_ENTER_4_BYTE_ADDRESS_MODE		0xB7
#define FLASH_EXIT_4_BYTE_ADDRESS_MODE		0xE9
#define FLASH_ENABLE_RESET					0x66
#define FLASH_RESET_DEVICE					0x99


// W25Q256JV Instruction Set (Dual-/Quad-SPI, 4-Byte Address Mode):
#define FLASH_FAST_READ_DUAL_OUTPUT						0x3B
#define FLASH_FAST_READ_DUAL_OUTPUT_WITH_4_BYTE_ADDRESS	0x3C
#define FLASH_MFTR_DEVICE_ID_DUAL_IO					0x92
#define FLASH_FAST_READ_DUAL_IO							0xBB
#define FLASH_FAST_READ_DUAL_IO_WITH_4_BYTE_ADDRESS		0xBC
#define FLASH_QUAD_INPUT_PAGE_PROGRAM					0x32
#define FLASH_QUAD_PAGE_PROGRAM_WITH_4_BYTE_ADDRESS		0x34
#define FLASH_FAST_READ_QUAD_OUTPUT						0x6B
#define FLASH_FAST_READ_QUAD_OUTPUT_WITH_4_BYTE_ADDRESS	0x6C
#define FLASH_MFTR_DEVICE_ID_QUAD_IO					0x94
#define FLASH_FAST_READ_QUAD_IO							0xEB
#define FLASH_FAST_READ_QUAD_IO_WITH_4_BYTE_ADDRESS		0xEC
#define FLASH_SET_BURST_WITH_WRAP						0x77


void Flash_Init();

void Flash_SendInstructionOnly(uint8_t);

void Flash_SendCommand(uint8_t, uint8_t, uint8_t, uint32_t, uint8_t, uint8_t, uint32_t, uint8_t, uint8_t, uint32_t, uint8_t);

uint8_t Flash_GetByteFromAddress(uint32_t);

void Flash_SetByteAtAddress(uint32_t, uint8_t);



#endif
