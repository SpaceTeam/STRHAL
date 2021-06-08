#include "foc/tmc6200/TMC6200.h"
#include "foc/swdriver.h"
#include "spi.h"


// spi access
int tmc6200_readInt(uint8_t address)
{
	uint8_t txData[5];
	txData[0] = TMC_ADDRESS(address);
	txData[1] = 0;
	txData[2] = 0;
	txData[3] = 0;
	txData[4] = 0;

	uint8_t rxData[5];

	swdriver_setCsnDriver(false);
	//TODO FIX HAL_SPI_TransmitReceive(swdriver.SPI, txData, rxData, 5, HAL_MAX_DELAY);
	swdriver_setCsnDriver(true);

	return (int)((rxData[1] << 24) | (rxData[2] << 16) | (rxData[3] << 8) | (rxData[4] << 0));
}


void tmc6200_writeInt(uint8_t address, int value)
{
	uint8_t data[5];
	data[0] = address | TMC6200_WRITE_BIT;
	data[1] = 0xFF & (value>>24);
	data[2] = 0xFF & (value>>16);
	data[3] = 0xFF & (value>>8);
	data[4] = 0xFF & (value>>0);

	swdriver_setCsnDriver(false);
	//TODO FIX
	//HAL_SPI_Transmit(swdriver.SPI, data, 5, HAL_MAX_DELAY);
	swdriver_setCsnDriver(true);
}
