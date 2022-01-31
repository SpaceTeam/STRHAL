#include "../Inc/LID_QSPI.h"

static struct {
		uint8_t data[LID_QSPI_WRITE_BUF_SIZE];
		uint32_t n;
} _write_buf;

static void LID_QSPI_InitGPIO() {
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOE);

	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin = LL_GPIO_PIN_10 | LL_GPIO_PIN_11 | LL_GPIO_PIN_12| LL_GPIO_PIN_13 | LL_GPIO_PIN_14 | LL_GPIO_PIN_15;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_10;
	LL_GPIO_Init(GPIOE, &GPIO_InitStruct);
}

static void LID_QSPI_InitDMA() {
	LL_DMA_InitTypeDef DMA_InitStruct = {0};

	DMA_InitStruct.Direction = LL_DMA_DIRECTION_MEMORY_TO_PERIPH;
	DMA_InitStruct.MemoryOrM2MDstAddress = (uint32_t)_write_buf;
	DMA_InitStruct.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_BYTE;
	DMA_InitStruct.MemoryOrM2MDstIncMode = LL_DMA_MEMORY_INCREMENT;
	DMA_InitStruct.Mode = LL_DMA_MODE_CIRCULAR;
	DMA_InitStruct.NbData = LID_QSPI_WRITE_BUF_SIZE;
	DMA_InitStruct.PeriphOrM2MSrcAddress = (uint32_t)&QUADSPI->DR;
	DMA_InitStruct.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_HALFWORD;
	DMA_InitStruct.PeriphOrM2MSrcIncMode = LL_DMA_MEMORY_NOINCREMENT;
	DMA_InitStruct.PeriphRequest = LL_DMAMUX_REQ_QSPI;
	DMA_InitStruct.Priority = LID_QSPI_DMA_PRIORITY;

	LL_DMA_Init(LID_QSPI_DMA, LID_QSPI_DMA_CHANNEL, &DMA_InitStruct);
}

void LID_QSPI_Init() {
	LL_AHB3_GRP1_EnableClock(LL_AHB3_GRP1_PERIPH_QSPI);

	LID_QSPI_InitGPIO();

	// QSPI config
	QUADSPI->CR |= LID_QSPI_PRESCALER << QUADSPI_CR_PRESCALER_Pos;
	QUADSPI->CR |= QUADSPI_CR_DMAEN;  // Enable use of DMA
	QUADSPI->CR |= LID_QSPI_FIFO_THRESH << QUADSPI_CR_FTHRES_Pos;
	QUADSPI->CR |= QUADSPI_CR_EN;  // Enable QSPI

	// QSPI device config
	QUADSPI->DCR |= LID_QSPI_FLASH_SIZE << QUADSPI_DCR_FSIZE_Pos;
	QUADSPI->DCR |= LID_QSPI_NCS_HIGH_TIME << QUADSPI_DCR_CSHT_Pos;
	QUADSPI->DCR |= LID_QSPI_CLK_LEVEL << QUADSPI_DCR_CKMODE_Pos;

	LID_QSPI_InitDMA();
}

void LID_QSPI_WriteInstruction(uint8_t instruction) {
	QUADSPI->CCR = instruction | QUADSPI_CCR_IMODE_0;
	QUADSPI->FCR |= (QUADSPI_FCR_CTCF);
}
