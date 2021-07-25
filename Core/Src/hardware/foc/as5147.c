#include "foc/as5147.h"
#include "foc/swdriver.h"
#include "spi.h"
#include "systick.h"


static void spiMode_set(void)
{
	SPI1_Init(LL_SPI_DATAWIDTH_24BIT, LL_SPI_PHASE_2EDGE, LL_SPI_POLARITY_LOW, LL_SPI_NSS_HARD_OUTPUT, LL_SPI_BAUDRATEPRESCALER_DIV32);
	Systick_BusyWait(2);

}

static void spiMode_reset(void)
{
	SPI1_Init(LL_SPI_DATAWIDTH_24BIT, LL_SPI_PHASE_2EDGE, LL_SPI_POLARITY_HIGH, LL_SPI_NSS_HARD_OUTPUT, LL_SPI_BAUDRATEPRESCALER_DIV32);
	Systick_BusyWait(2);
}

uint16_t as5147_getAngle(uint8_t enc) //returns 11bit value
{
	spiMode_set();

	uint8_t txData[2];
	uint8_t rxData[2];

	txData[0] = (1 << 7) | (1 << 6) | 0x3F; // parity 1, read, address upper 6 bits
	txData[1] = 0xFF; // address lower 6 bits

	swdriver_setCsnEncoder(enc, false);
	SPI_Transmit_Receive(swdriver.spi, txData, rxData, 2); //TODO FIX
	swdriver_setCsnEncoder(enc, true);
	Systick_BusyWait(2);

	txData[0] = 0;
	txData[1] = 0;

	swdriver_setCsnEncoder(enc, false);
	SPI_Transmit_Receive(swdriver.spi, txData, rxData, 2);
	swdriver_setCsnEncoder(enc, true);

	spiMode_reset();

	return ((((((uint16_t)rxData[0]) & 0x3F) << 8) | rxData[1]) >> 3);
}
