#ifndef LID_QSPI_H
#define LID_QSPI_H

#include <stm32g4xx_ll_bus.h>
#include <stm32g4xx_ll_gpio.h>
#include <stm32g4xx_ll_dma.h>
#include <stm32g4xx_ll_dmamux.h>

typedef struct {
		uint8_t Instruction;
		uint8_t SendAddress;
		uint32_t Address;
		uint8_t SendAlternate;
		uint8_t AlternateLength;
		uint32_t Alternate;
		uint8_t DummyLength;
		uint8_t SendData;
		uint8_t DataLength;
		uint8_t *Data;
} LID_QSPI_Command;


typedef enum {
	LID_QSPI_STATE_IDLE	= 0 << 0,

	LID_QSPI_STATE_TX	= 1 << 1,
	LID_QSPI_STATE_TC	= 1 << 2,
	LID_QSPI_STATE_TE   = 1 << 3,

	LID_QSPI_STATE_RX	= 1 << 4,
} LID_QSPI_State_t;

/* @brief Init QSPI interface
 * @param flash_size  actual flash size will be 2^(value+1) | e.g. value=24 -> 32 MiB
 * @param ncs_high_time  minimum number of cycles during which nCS must be high between commands
 * @param clk_level  if clk should stay low (0) or high (1) when nCS is high
 */
void LID_QSPI_Init(uint8_t flash_size, uint8_t ncs_high_time, uint8_t clk_level);

void LID_QSPI_WriteInstruction(uint8_t instruction);

uint32_t LID_QSPI_Write(LID_QSPI_Command cmd);

#endif
