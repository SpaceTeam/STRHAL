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
		uint8_t instruction;
		uint8_t instruction_size;
		uint32_t addr;
		uint32_t addr_size;
		uint8_t alt_size;
		uint32_t alt;
		uint8_t dummy_size;
} LID_QSPI_Command_t;


typedef struct {
	uint8_t psc : 5;
	uint8_t flash_size : 4;
	uint8_t ncs_high_time : 3;
	uint8_t clk_level : 1;
} LID_QSPI_Config_t;


void LID_QSPI_Init();
void LID_QSPI_Run();
void LID_QSPI_Stop();
void LID_QSPI_Reset();

int LID_QSPI_Flash_Init(const LID_QSPI_Config_t *config);

uint32_t LID_QSPI_Indirect_Write(const LID_QSPI_Command_t *cmd, const uint8_t *data, uint32_t n, uint16_t tot);
uint32_t LID_QSPI_Indirect_Read(const LID_QSPI_Command_t *cmd, uint8_t *data, uint32_t n, uint16_t tot);

#ifdef __cplusplus
}
#endif

#endif
