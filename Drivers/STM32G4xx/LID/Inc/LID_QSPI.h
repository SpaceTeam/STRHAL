#ifndef LID_QSPI_H
#define LID_QSPI_H

#include <stm32g4xx_ll_bus.h>
#include <stm32g4xx_ll_gpio.h>
#include <stm32g4xx_ll_dma.h>
#include <stm32g4xx_ll_dmamux.h>


#ifdef __cplusplus
extern "C" {
#endif

#define LID_QSPI_RX_BUF_SIZE 1024

/*
 * @brief QSPI command frame structure
 *
 * The only field required is Instruction.
 * SendAddress, SendAlternate and SendData are boolean values that determine whether to send the respective data.
 * If they are set to 0, the respective data are ignored.
 * DummyLength directly specifies the number of dummy cycles to send. If set to 0, no dummy cycles are sent.
 */
typedef struct {
		uint8_t ReadWrite;  // 0 = read, 1 = write
		uint8_t Instruction;
		uint8_t SendAddress;
		uint32_t Address;
		uint8_t SendAlternate;
		uint8_t AlternateLength;  // If SendAlternate is 1, AlternateLength+1 alternate bytes will be sent
		uint32_t Alternate;
		uint8_t DummyLength;
		uint8_t SendData;
		uint8_t DataLength;  // If SendData is 1, DataLength+1 bytes at Data will be sent
		uint8_t *Data;
} LID_QSPI_Command_t;


typedef enum {
	LID_QSPI_STATE_IDLE	= 0 << 0,

	LID_QSPI_STATE_TX	= 1 << 1,
	LID_QSPI_STATE_TC	= 1 << 2,
	LID_QSPI_STATE_TE   = 1 << 3,

	LID_QSPI_STATE_RX	= 1 << 4,
} LID_QSPI_State_t;

/* @brief Initialize QSPI interface
 * @param flash_size  actual flash size will be 2^(value+1) | e.g. value=24 -> 32 MiB
 * @param ncs_high_time  minimum number of cycles during which nCS must be high between commands
 * @param clk_level  if clk should stay low (0) or high (1) when nCS is high
 */
void LID_QSPI_Init(uint8_t flash_size, uint8_t ncs_high_time, uint8_t clk_level);

/*
 * @brief Write commands over QSPI
 * @param cmd  QSPI command frame structure
 * @return TODO: bytes written
 */
uint32_t LID_QSPI_Write(LID_QSPI_Command_t cmd);


#ifdef __cplusplus
}
#endif

#endif
