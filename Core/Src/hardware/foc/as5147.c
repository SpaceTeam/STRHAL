#include "foc/as5147.h"
#include "foc/swdriver.h"
#include "spi.h"
#include "serial.h"
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

void AS5x47_SendWord(uint8_t enc, uint16_t address, uint16_t data) //returns 11bit value
{
	spiMode_set();

	uint8_t txData[2];
	uint8_t rxData[2];

	txData[0] = address>>8 & 0xFF;
	txData[1] = address>>0 & 0xFF;

	swdriver_setCsnEncoder(enc, false);
	SPI_Transmit_Receive(swdriver.spi, txData, rxData, 2); //TODO FIX
	swdriver_setCsnEncoder(enc, true);
	Systick_BusyWait(2);

	txData[0] = data>>8 & 0xFF;
	txData[1] = data>>0 & 0xFF;

	swdriver_setCsnEncoder(enc, false);
	SPI_Transmit_Receive(swdriver.spi, txData, rxData, 2);
	swdriver_setCsnEncoder(enc, true);
Serial_PutString("SendWord");
Serial_PrintHex(rxData[0] << 8 | rxData[1]);
	spiMode_reset();


}

uint16_t AS5x47_ReceiveWord(uint8_t enc, uint16_t address)
{
	spiMode_set();
	uint8_t txData[2];
	uint8_t rxData[2] = {0};
	txData[0] = ( address >> 8 ) & 0xFF;
	txData[1] = ( address >> 0 ) & 0xFF;
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
	return (((uint16_t)rxData[0] << 8) | rxData[1]);
}
void AS5x47_Init(uint8_t enc)
{
//	AS5x47_SendWord(enc, 0x0018, 1<<2);	//Set Dir Bit
	AS5x47_SendWord(enc, 0x0018, 1<<5 | 0<<2);	//Set ABIBIN and Clear DIR
	Serial_PutString("INIT AS5x47: ");
	Serial_PrintHex(AS5x47_ReceiveWord(enc, 0xC018));
	Serial_PutString("TEST AS5x47: ");
	Serial_PrintHex(AS5x47_ReceiveWord(enc, 0x000));
	Serial_PrintHex(AS5x47_ReceiveWord(enc, 0x0001));


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
	swdriver_setCsnEncoder(enc, true);
	Systick_BusyWait(2);

	txData[0] = 0;
	txData[1] = 0;

	swdriver_setCsnEncoder(enc, false);
	SPI_Transmit_Receive(swdriver.spi, txData, rxData, 2);
	swdriver_setCsnEncoder(enc, true);

	spiMode_reset();
	return (((((uint16_t)rxData[0]) & 0x3F) << 8) | rxData[1]);
//	return ((((((uint16_t)rxData[0]) & 0x3F) << 8) | rxData[1]) >> 3);
}
