#include <foc/as5x47.h>
#include "foc/swdriver.h"
#include "spi.h"
#include "dipswitch.h"
#include "serial.h"
#include "systick.h"

static void spiMode_set(void)
{
	LL_SPI_Disable(SPI1);
	SPI1_Init(LL_SPI_DATAWIDTH_8BIT, LL_SPI_PHASE_2EDGE, LL_SPI_POLARITY_LOW, LL_SPI_NSS_SOFT, LL_SPI_BAUDRATEPRESCALER_DIV32);
	Systick_BusyWait(2);

}

static void spiMode_reset(void)
{
	LL_SPI_Disable(SPI1);
//	SPI1_Init(LL_SPI_DATAWIDTH_24BIT, LL_SPI_PHASE_2EDGE, LL_SPI_POLARITY_HIGH, LL_SPI_NSS_HARD_OUTPUT, LL_SPI_BAUDRATEPRESCALER_DIV32);
	SPI1_Init(LL_SPI_DATAWIDTH_8BIT, LL_SPI_PHASE_2EDGE, LL_SPI_POLARITY_HIGH, LL_SPI_NSS_SOFT, LL_SPI_BAUDRATEPRESCALER_DIV256);
	Systick_BusyWait(2);

}

uint16_t AS5x47_GenEvenParity(uint16_t data)
{
	uint8_t xorCarry = 0;
	for (uint8_t i = 0; i < 15; i++)
	{
		xorCarry = ((data >> i) & 1) ^ xorCarry;
	}
	return (data & 0x7FFF) | (xorCarry << 15);
}

void AS5x47_SendWord(uint8_t enc, uint16_t address, uint16_t data) //returns 11bit value
{
	address &= 0x3FFF;
	address = AS5x47_GenEvenParity(address);
	data &= 0x3FFF;
	data = AS5x47_GenEvenParity(data);

	uint8_t txData[2];
	uint8_t rxData[2];

	spiMode_set();

	txData[0] = address >> 8 & 0xFF;
	txData[1] = address >> 0 & 0xFF;

	swdriver_setCsnEncoder(enc, false);
	SPI_Transmit_Receive(swdriver.spi, txData, rxData, 2); //TODO FIX
	for (volatile uint16_t c = 0; c < 0xFF; c++)
	{
	}
	swdriver_setCsnEncoder(enc, true);
	Systick_BusyWait(2);

	txData[0] = data >> 8 & 0xFF;
	txData[1] = data >> 0 & 0xFF;

	swdriver_setCsnEncoder(enc, false);
	SPI_Transmit_Receive(swdriver.spi, txData, rxData, 2);
	for (volatile uint16_t c = 0; c < 0xFF; c++)
	{
	}
	swdriver_setCsnEncoder(enc, true);

	spiMode_reset();

}

uint16_t AS5x47_ReceiveWord(uint8_t enc, uint16_t address)
{
	address &= 0x3FFF;
	address |= 0x4000;
	address = AS5x47_GenEvenParity(address);

	spiMode_set();
	uint8_t txData[2];
	uint8_t rxData[2] =
	{ 0 };
	txData[0] = (address >> 8) & 0xFF;
	txData[1] = (address >> 0) & 0xFF;
	swdriver_setCsnEncoder(enc, false);
	SPI_Transmit_Receive(swdriver.spi, txData, rxData, 2);
	for (volatile uint16_t c = 0; c < 0xFF; c++)
	{
	}
	swdriver_setCsnEncoder(enc, true);
	for (volatile uint16_t c = 0; c < 0xFF; c++)
	{
	}
	txData[0] = 0;
	txData[1] = 0;
	swdriver_setCsnEncoder(enc, false);
	SPI_Transmit_Receive(swdriver.spi, txData, rxData, 2);
	for (volatile uint16_t c = 0; c < 0xFF; c++)
	{
	}
	swdriver_setCsnEncoder(enc, true);

	spiMode_reset();
	Systick_BusyWait(2);

	return (((uint16_t) rxData[0] << 8) | rxData[1]);
}
void AS5x47_Init(uint8_t enc)
{

	if (!(Dipswitch_Read() & 0x80))
	{
		Serial_PutString("REG 0x0018 (0x8001): ");
		Serial_PrintHex(AS5x47_ReceiveWord(enc, 0x0018));
		Serial_PutString("REG 0x0019 (0x0000): ");
		Serial_PrintHex(AS5x47_ReceiveWord(enc, 0x0019));
		AS5x47_SendWord(enc, 0x0018, 1 << 5 | 0 << 2 | 1);
		Serial_PutString("REG 0x0018 (0x0021): ");
		Serial_PrintHex(AS5x47_ReceiveWord(enc, 0x0018));
	}
	else
	{

	}
	/*
	 Serial_PutString("INIT 0x0018: ");
	 while(1)
	 {
	 Serial_PrintHex(AS5x47_ReceiveWord(BLMB_POSITION_ENCODER, 0x0018));
	 Systick_BusyWait(10);
	 }
	 Serial_PutString("INIT 0x0018: ");
	 Serial_PrintHex(AS5x47_ReceiveWord(enc, 0x0018));
	 Serial_PutString("INIT 0x0019: ");
	 Serial_PrintHex(AS5x47_ReceiveWord(enc, 0x0019));
	 */

}

uint16_t AS5x47_GetAngle(uint8_t enc) //returns 11bit value
{
	spiMode_set();

	uint8_t txData[2];
	uint8_t rxData[2];

	txData[0] = (1 << 7) | (1 << 6) | 0x3F; // parity 1, read, address upper 6 bits
	txData[1] = 0xFF; // address lower 6 bits

	swdriver_setCsnEncoder(enc, false);
	SPI_Transmit_Receive(swdriver.spi, txData, rxData, 2); //TODO FIX
	for (volatile uint16_t c = 0; c < 0xFF; c++)
	{
	}
	swdriver_setCsnEncoder(enc, true);

	txData[0] = 0;
	txData[1] = 0;
	for (volatile uint16_t c = 0; c < 0xFF; c++)
	{
	}

	swdriver_setCsnEncoder(enc, false);
	SPI_Transmit_Receive(swdriver.spi, txData, rxData, 2);
	for (volatile uint16_t c = 0; c < 0xFF; c++)
	{
	}
	swdriver_setCsnEncoder(enc, true);

	spiMode_reset();
	return (((((uint16_t) rxData[0]) & 0x3F) << 8) | rxData[1]);
//	return ((((((uint16_t)rxData[0]) & 0x3F) << 8) | rxData[1]) >> 3);
}
