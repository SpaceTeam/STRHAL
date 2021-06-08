#include "foc/as5147.h"
#include "foc/swdriver.h"
#include "spi.h"


static void spiMode_set(void)
{
/*
	//TODO FIX
	swdriver.SPI->Init.CLKPolarity = SPI_POLARITY_LOW;
	swdriver.SPI->Init.CLKPhase = SPI_PHASE_2EDGE;

	if (HAL_SPI_Init(swdriver[drv].SPI) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}
	HAL_Delay(2);*/
}

static void spiMode_reset(void)
{
	/*
	swdriver.SPI->Init.CLKPolarity = SPI_POLARITY_HIGH;
	swdriver.SPI->Init.CLKPhase = SPI_PHASE_2EDGE;
	if (HAL_SPI_Init(swdriver.SPI) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}
	HAL_Delay(2);*/
}

uint16_t as5147_getAngle(uint8_t enc) //returns 11bit value
{
	spiMode_set();

	uint8_t txData[2];
	txData[0] = (1 << 7) | (1 << 6) | 0x3F; // parity 1, read, address upper 6 bits
	txData[1] = 0xFF; // address lower 6 bits

	swdriver_setCsnEncoder(enc, false);
	//TODO FIX
//	HAL_SPI_Transmit(swdriver.SPI, txData, 2, HAL_MAX_DELAY); //TODO FIX
	swdriver_setCsnEncoder(enc, true);
	Systick_BusyWait(2);

	txData[0] = 0;
	txData[1] = 0;
	uint8_t rxData[2];

	swdriver_setCsnEncoder(enc, false);
//TODO FIX
	//HAL_SPI_TransmitReceive(swdriver.SPI, txData, rxData, 2, HAL_MAX_DELAY);
	swdriver_setCsnEncoder(enc, true);

	spiMode_reset();

	return ((((((uint16_t)rxData[0]) & 0x3F) << 8) | rxData[1]) >> 3);
}
