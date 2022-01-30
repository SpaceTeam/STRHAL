#ifndef LID_QSPI_H
#define LID_QSPI_H

#include <stm32g4xx_ll_bus.h>
#include <stm32g4xx_ll_gpio.h>
#include <stm32g4xx_ll_dma.h>
#include <stm32g4xx_ll_dmamux.h>

#define LID_QSPI_PRESCALER 8      // NEED TO DETERMINE RIGHT VALUE
#define LID_QSPI_FIFO_THRESH 8    // NEED TO DETERMINE RIGHT VALUE, when <value> amount of bytes in fifo, start dma transfer
#define LID_QSPI_FLASH_SIZE 24    // actual flash size will be 2^(value+1) -> 32 MiB
#define LID_QSPI_NCS_HIGH_TIME 7  // minimum number of cycles during which nCS must be high between commands
#define LID_QSPI_CLK_LEVEL 0      // if clk should stay low (0) or high (1) when nCS is high

#define LID_QSPI_DMA DMA1
#define LID_QSPI_DMA_CHANNEL LL_DMA_CHANNEL_6
#define LID_QSPI_DMA_PRIORITY LL_DMA_PRIORITY_HIGH
#define LID_QSPI_WRITE_BUF_SIZE 256

void LID_QSPI_Init(void);

void LID_QSPI_WriteInstruction(uint8_t instruction);

#endif
