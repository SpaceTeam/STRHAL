#include <string.h>
#include <math.h>

#include "../Inc/LID_QSPI.h"

#define LID_QSPI_PRESCALER 8      // NEED TO DETERMINE RIGHT VALUE
#define LID_QSPI_FIFO_THRESH 8    // NEED TO DETERMINE RIGHT VALUE, when <value> amount of bytes in fifo, start dma transfer

#define LID_QSPI_DMA DMA1
#define LID_QSPI_DMA_CHANNEL LL_DMA_CHANNEL_6
#define LID_QSPI_DMA_PRIORITY LL_DMA_PRIORITY_MEDIUM

//static uint8_t _rx_buf[LID_QSPI_RX_BUF_SIZE];

static uint32_t _state = LID_QSPI_STATE_IDLE;

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
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

	LL_DMA_InitTypeDef DMA_InitStruct = {0};

	DMA_InitStruct.Direction = LL_DMA_DIRECTION_MEMORY_TO_PERIPH;
	DMA_InitStruct.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_BYTE;
	DMA_InitStruct.MemoryOrM2MDstIncMode = LL_DMA_MEMORY_INCREMENT;
	DMA_InitStruct.Mode = LL_DMA_MODE_NORMAL;
	DMA_InitStruct.PeriphOrM2MSrcAddress = (uint32_t)&QUADSPI->DR;
	DMA_InitStruct.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_BYTE;
	DMA_InitStruct.PeriphOrM2MSrcIncMode = LL_DMA_MEMORY_NOINCREMENT;
	DMA_InitStruct.PeriphRequest = LL_DMAMUX_REQ_QSPI;
	DMA_InitStruct.Priority = LID_QSPI_DMA_PRIORITY;

	LL_DMA_Init(LID_QSPI_DMA, LID_QSPI_DMA_CHANNEL, &DMA_InitStruct);
	LL_DMA_DisableChannel(LID_QSPI_DMA, LID_QSPI_DMA_CHANNEL);

	LL_DMA_EnableIT_TC(LID_QSPI_DMA, LID_QSPI_DMA_CHANNEL);
	LL_DMA_EnableIT_TE(LID_QSPI_DMA, LID_QSPI_DMA_CHANNEL);

	NVIC_SetPriority(DMA1_Channel6_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 5, 2));
	NVIC_EnableIRQ(DMA1_Channel6_IRQn);
}

void LID_QSPI_Init(uint8_t flash_size, uint8_t ncs_high_time, uint8_t clk_level) {
	LL_AHB3_GRP1_EnableClock(LL_AHB3_GRP1_PERIPH_QSPI);

	LID_QSPI_InitGPIO();

	// QSPI config
	QUADSPI->CR |= LID_QSPI_PRESCALER << QUADSPI_CR_PRESCALER_Pos;
	QUADSPI->CR |= QUADSPI_CR_DMAEN;  // Enable use of DMA
	QUADSPI->CR |= LID_QSPI_FIFO_THRESH << QUADSPI_CR_FTHRES_Pos;
	QUADSPI->CR |= QUADSPI_CR_EN;  // Enable QSPI

	// QSPI device config
	QUADSPI->DCR |= flash_size << QUADSPI_DCR_FSIZE_Pos;
	QUADSPI->DCR |= ncs_high_time << QUADSPI_DCR_CSHT_Pos;
	QUADSPI->DCR |= clk_level << QUADSPI_DCR_CKMODE_Pos;

	LID_QSPI_InitDMA();
}

uint32_t LID_QSPI_Write(LID_QSPI_Command_t cmd) {
	if (_state & (LID_QSPI_STATE_TX | LID_QSPI_STATE_RX))
		return 0;

	LL_DMA_DisableChannel(LID_QSPI_DMA, LID_QSPI_DMA_CHANNEL);
	while (LL_DMA_IsEnabledChannel(LID_QSPI_DMA, LID_QSPI_DMA_CHANNEL));

	QUADSPI->CCR = 0;

	// Specify which data need to be sent
	// All parameters are transmitted on one line only, except for data
	uint32_t ccr = 0;
	ccr |= cmd.ReadWrite ? 0: QUADSPI_CCR_FMODE_0;  // Set read write
	ccr |= QUADSPI_CCR_IMODE_0;
	ccr |= cmd.SendAddress ? QUADSPI_CCR_ADMODE_0 : 0;
	ccr |= cmd.SendAlternate ? QUADSPI_CCR_ABMODE_0 : 0;
	ccr |= cmd.DummyLength ? (uint8_t)fmin(cmd.DummyLength, 0x1F) << QUADSPI_CCR_DCYC_Pos : 0;  // Clamp dummy length to largest possible value if larger value supplied
	ccr |= cmd.SendData ? (QUADSPI_CCR_DMODE_1 | QUADSPI_CCR_DMODE_0) : 0;

	ccr |= (cmd.Instruction & 0xFF) << QUADSPI_CCR_INSTRUCTION_Pos;

	QUADSPI->CCR = ccr;

	if (cmd.SendAddress) {
		QUADSPI->CCR |= QUADSPI_CCR_ADSIZE_0 | QUADSPI_CCR_ADSIZE_1;  // 32-bit address
		QUADSPI->AR = cmd.Address;
	}

	if (cmd.SendAlternate) {
		if (cmd.AlternateLength > 3)  // If input value too large, clamp to largest value
			cmd.AlternateLength = 3;  // TODO: Notify
		QUADSPI->CCR |= cmd.AlternateLength << QUADSPI_CCR_ABSIZE_Pos;
		QUADSPI->ABR |= cmd.Alternate << QUADSPI_ABR_ALTERNATE_Pos;
	}

	if (cmd.ReadWrite) {
		if (cmd.SendData) {
			QUADSPI->DLR |= cmd.DataLength << QUADSPI_DLR_DL_Pos;

			LL_DMA_SetMemoryAddress(LID_QSPI_DMA, LID_QSPI_DMA_CHANNEL, (uint32_t)cmd.Data);
			LL_DMA_SetDataLength(LID_QSPI_DMA, LID_QSPI_DMA_CHANNEL, cmd.DataLength + 1);

			LL_DMA_EnableChannel(LID_QSPI_DMA, LID_QSPI_DMA_CHANNEL);
			while (!LL_DMA_IsEnabledChannel(LID_QSPI_DMA, LID_QSPI_DMA_CHANNEL));

			_state |= LID_QSPI_STATE_TX;

			return cmd.DataLength;
		}
	} else {
		// TODO: Read data
	}

	return 0;  // TODO: return real bytes written/read
}

void DMA1_Channel6_IRQHandler(void) {
	if (LL_DMA_IsActiveFlag_TC6(LID_QSPI_DMA)) {  // Transfer complete
		LL_DMA_ClearFlag_TC6(LID_QSPI_DMA);
		_state &= ~(LID_QSPI_STATE_TX);
		_state |= LID_QSPI_STATE_TC;
	}

	if (LL_DMA_IsActiveFlag_TE6(LID_QSPI_DMA)) {  // Transfer error
		LL_DMA_ClearFlag_TE6(LID_QSPI_DMA);
		_state &= ~(LID_QSPI_STATE_TX);
		_state |= LID_QSPI_STATE_TE;  // TODO: Retry
	}
}
