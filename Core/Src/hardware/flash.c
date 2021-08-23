#include "flash.h"
#include "main.h"

void Flash_InitGPIO(void)
{

	LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOE);
	LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOB);
	LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOD);
	LL_GPIO_InitTypeDef GPIO_InitStruct =
	{ 0 };
	GPIO_InitStruct.Pin = LL_GPIO_PIN_2;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_9;
	LL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LL_GPIO_PIN_2 | LL_GPIO_PIN_10;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_9;
	LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LL_GPIO_PIN_11 | LL_GPIO_PIN_12 | LL_GPIO_PIN_13;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_9;
	LL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}

void Flash_Init(void)
{
	LL_AHB3_GRP1_EnableClock(LL_AHB3_GRP1_PERIPH_QSPI);

	Flash_InitGPIO();

	QUADSPI->DCR |= ((1 << 20) | (1 << 19));				// Specifies the size of the memory
	QUADSPI->DCR |= ((1 << 10) | (1 << 9) | (1 << 8));		// Specifies the delay between commands
	QUADSPI->CR |= (0x08 << 24);							// Prescaler

	QUADSPI->CR |= (1 << 0);							// Enables QUADSPI

	Flash_SendInstructionOnly(FLASH_ENTER_4_BYTE_ADDRESS_MODE);
	//Flash_SendCommand(0, FLASH_WRITE_STATUS_REGISTER_2, 0, 0, 1, 0, 0x02, 0, 0, 0, 0);	// Set the Quad Enable bit in Status Register-2
}

// This function sends only an instruction to the flash. No address, alternate bytes, dummy cycles or data will be sent.
void Flash_SendInstructionOnly(uint8_t instruction)
{
	uint32_t ccr = instruction;
	ccr |= (1 << 8);

	QUADSPI->CCR = ccr;

	QUADSPI->FCR |= (QUADSPI_FCR_CTCF);
}

// This function offers full flexibility in sending a command to the flash. If a parameter is not needed, the corresponding variable "is..." must be 0, else anything but 0.
// E.g., a command does not need an address: isAddress = 0. The argument "address" will be ignored in this case.
//
// isRead:			Specifies whether to execute a read command. If 0, a write command will be executed.
// instruction: 	Specifies the instruction to be sent to the flash.
// isAdress:		Specifies whether an address is to be sent to the flash.
// address:			The address to be sent to the flash.
// isAlternateBytes:	Specifies whether or not alternate bytes are to be sent to the flash.
// alternateBytesSize:	Specifies the number of alternate bytes (0: 8 bit, 1: 16 bit, 2: 24 bit, 3: 32 bit)
// alternateBytes:	The alternate bytes to be sent to the flash.
// dummyCycles:		Specifies the number of dummy cycles of the command (as a number of clock cycles).
// isData:			Specifies whether data should be sent to the flash.
// dataLength:		Specifies the number of bytes of data to be sent to the flash.
// data:			The data to be sent to the flash.
//
void Flash_ClearBuffer(void)
{
	while (QUADSPI->SR & QUADSPI_SR_FTF)
	{
		QUADSPI->DR;
	}
}
void Flash_SendCommand(uint8_t isRead, uint8_t instruction, uint8_t isAddress, uint32_t address, uint8_t isAlternateBytes, uint8_t alternateBytesSize, uint32_t alternateBytes, uint8_t dummyCycles,
		uint8_t isData, uint32_t dataLength, uint8_t data)
{
	// The order of the following statements should not be changed in order to avoid sending invalid commands.

	while ((QUADSPI->SR & QUADSPI_SR_BUSY))
		;		// to be improved using a transfer complete interrupt (TCI)

	uint32_t ccr_buffer = 0;

	if (isAddress)
	{
		ccr_buffer |= (1 << 10);			// ADMODE = 01

	}

	QUADSPI->DLR = dataLength;

	ccr_buffer |= (1 << 24);			// DMODE = 01

	if (isRead)
	{
		ccr_buffer |= (1 << 26);			// FMODE = 01
	}

	if (isAlternateBytes)
	{
		ccr_buffer |= (1 << 14);			// ABMODE = 01

		ccr_buffer |= (alternateBytesSize << 16);	// ABSIZE

		QUADSPI->ABR = alternateBytes;
	}

	ccr_buffer |= (dummyCycles << 18);

	ccr_buffer |= instruction;
	ccr_buffer |= (1 << 8);							// IMODE = 01

	while ((QUADSPI->SR & QUADSPI_SR_BUSY))
		;		// to be improved using a transfer complete interrupt (TCI)

	QUADSPI->FCR |= (QUADSPI_FCR_CTCF);

	if (isAddress)
	{
		QUADSPI->CCR = ccr_buffer | ((1 << 13) | (1 << 12));	// Sets the address length to 32 bit
		QUADSPI->AR = address;				// If no data needs to be sent, the command sequence will start here
	}
	else
		QUADSPI->CCR = ccr_buffer;

	if (isData)
	{
		QUADSPI->DR = data;					// A data transmission command sequence (both data and address are sent) will start here
	}

}

uint8_t Flash_GetByteFromAddress(uint32_t address)
{
	Flash_SendCommand(1, FLASH_READ_DATA_WITH_4_BYTE_ADDRESS, 1, address, 0, 0, 0, 0, 0, 0, 0);
	while (!(QUADSPI->SR & QUADSPI_SR_TCF))
		;		// to be improved using a transfer complete interrupt (TCI)
	uint8_t data = QUADSPI->DR;
	Flash_ClearBuffer();
	return data;
}

void Flash_SetByteAtAddress(uint32_t address, uint8_t data)
{
	// Before the write operation:
	//  - check that the FIFO has enough capacity left
	//  - check that the flash is not in BUSY-state
	//  - check if the desired memory location needs to be erased first (?)

	Flash_SendInstructionOnly(FLASH_WRITE_ENABLE);
	Flash_SendCommand(0, FLASH_PAGE_PROGRAM_WITH_4_BYTE_ADDRESS, 1, address, 0, 0, 0, 0, 1, 0, data);

}

void Flash_GetBlockStartingFromAddress(uint32_t address, uint8_t *data, uint32_t length)
{

}

void Flash_SetBlockStartingAtAddress(uint32_t address, uint8_t *data, uint32_t length)
{

}

void Flash_EraseSector(/*????*/)	//maybe block maybe both
{

}

void Flash_ResetDevice()
{

}

