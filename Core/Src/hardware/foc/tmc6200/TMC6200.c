#include "foc/tmc6200/TMC6200.h"
#include "foc/swdriver.h"
#include "spi.h"
#include "foc/tmc_helpers/Macros.h"
#include "foc/tmc6200/TMC6200_Constants.h"
#include "serial.h"


uint32_t tmc6200_readRegister(uint8_t address)
{
	uint8_t txData[5];
	txData[0] = TMC_ADDRESS(address);
	txData[1] = 0;
	txData[2] = 0;
	txData[3] = 0;
	txData[4] = 0;

	uint8_t rxData[5];

	swdriver_setCsnDriver(false);
	if(SPI_Transmit_Receive(swdriver.spi, txData, rxData, 5) != NOICE) Serial_PrintString("fack");
	swdriver_setCsnDriver(true);

	return (int)((rxData[1] << 24) | (rxData[2] << 16) | (rxData[3] << 8) | (rxData[4] << 0));
}


void tmc6200_writeRegister(uint8_t address, uint32_t value)
{
	uint8_t data[5];
	data[0] = address | TMC6200_WRITE_BIT;
	data[1] = 0xFF & (value>>24);
	data[2] = 0xFF & (value>>16);
	data[3] = 0xFF & (value>>8);
	data[4] = 0xFF & (value>>0);

	swdriver_setCsnDriver(false);
	if(SPI_Transmit_Receive(swdriver.spi, data, 0, 5) != NOICE) Serial_PrintString("fack");
	swdriver_setCsnDriver(true);
}
