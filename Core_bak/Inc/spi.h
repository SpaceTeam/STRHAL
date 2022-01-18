#ifndef __SPI_H__
#define __SPI_H__


#include "main.h"
#include "cmds.h"


void SPI1_Init(uint32_t datawidth, uint32_t cpha, uint32_t cpol, uint32_t nss, uint32_t baudrate);


Result_t SPI_Transmit_Receive(SPI_TypeDef * SPI, uint8_t *txData, uint8_t *rxData, uint32_t count);
Result_t SPI_Transmit_Receive_HardNSS(SPI_TypeDef *SPI, int32_t txData[], int32_t rxData[], uint32_t count);

#endif
