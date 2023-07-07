#ifndef STM32G431xx //No QSPI on G431xx
#include <string.h>
#include <math.h>

#include <stm32g4xx_ll_utils.h>
#include <STRHAL_QSPI.h>
#include <STRHAL_SysTick.h>

#define STRHAL_QSPI_FIFO_THRESH 0    // NEED TO DETERMINE RIGHT VALUE, when <value> amount of bytes in fifo, start dma transfer

#define STRHAL_QSPI_DMA DMA1
#define STRHAL_QSPI_DMA_CHANNEL LL_DMA_CHANNEL_6
#define STRHAL_QSPI_DMA_PRIORITY LL_DMA_PRIORITY_MEDIUM

//static uint8_t _rx_buf[STRHAL_QSPI_RX_BUF_SIZE];
static inline int _wait_for_status(uint32_t flag, uint16_t tot);
static inline int _wait_for_status_clear(uint32_t flag, uint16_t);
static inline void _clear_status(uint32_t flags);

static void _init_GPIO()
{
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOE);

	LL_GPIO_InitTypeDef GPIO_InitStruct =
	{ 0 };
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Pin = LL_GPIO_PIN_10 | LL_GPIO_PIN_11 | LL_GPIO_PIN_12 | LL_GPIO_PIN_13 | LL_GPIO_PIN_14 | LL_GPIO_PIN_15;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_10;
	LL_GPIO_Init(GPIOE, &GPIO_InitStruct);
}

void STRHAL_QSPI_Init()
{
	LL_AHB3_GRP1_EnableClock(LL_AHB3_GRP1_PERIPH_QSPI);
}

int STRHAL_QSPI_Flash_Init(const STRHAL_QSPI_Config_t *config)
{
	STRHAL_QSPI_Reset();

	_init_GPIO();

	QUADSPI->CR |= STRHAL_QSPI_FIFO_THRESH << QUADSPI_CR_FTHRES_Pos;

	if (_wait_for_status_clear(QUADSPI_SR_BUSY, 100) < 0)
		return -1;

	QUADSPI->CR |= config->psc << QUADSPI_CR_PRESCALER_Pos;
	QUADSPI->DCR |= config->flash_size << QUADSPI_DCR_FSIZE_Pos;
	QUADSPI->DCR |= config->ncs_high_time << QUADSPI_DCR_CSHT_Pos;
	QUADSPI->DCR |= config->clk_level << QUADSPI_DCR_CKMODE_Pos;

	STRHAL_QSPI_Run();

	return 0;
}

void STRHAL_QSPI_Reset()
{
	if (QUADSPI->CR & QUADSPI_CR_EN)
		QUADSPI->CR &= ~QUADSPI_CR_EN;

	CLEAR_REG(QUADSPI->CR);
	CLEAR_REG(QUADSPI->DCR);
	CLEAR_REG(QUADSPI->CCR);

	QUADSPI->FCR |= (QUADSPI_FCR_CSMF | QUADSPI_FCR_CTCF | QUADSPI_FCR_CTEF | QUADSPI_FCR_CTOF);
}

void STRHAL_QSPI_Run()
{
	QUADSPI->CR |= QUADSPI_CR_EN; // Enable QSPI
}

void STRHAL_QSPI_Stop()
{
	QUADSPI->CR &= ~QUADSPI_CR_EN;  // Enable QSPI
}

uint32_t STRHAL_QSPI_Indirect_Write(const STRHAL_QSPI_Command_t *cmd, const uint8_t *data, uint32_t n, uint16_t tot)
{
	if (_wait_for_status_clear(QUADSPI_SR_BUSY, tot) < 0)
		return -1;

	__IO uint32_t *data_reg = &QUADSPI->DR;
	_clear_status(QUADSPI_FCR_CSMF | QUADSPI_FCR_CTCF | QUADSPI_FCR_CTEF | QUADSPI_FCR_CTOF);

	uint32_t ccr = 0x00000000;

	if (n > 0)
	{
		ccr |= QUADSPI_CCR_DMODE_0;
		WRITE_REG(QUADSPI->DLR, n - 1U);
	}

	if (cmd->alt_size > 0)
	{
		ccr |= QUADSPI_CCR_ABMODE_0;
		ccr |= ((cmd->alt_size - 1) & 0x3) << QUADSPI_CCR_ABSIZE_Pos;
		WRITE_REG(QUADSPI->ABR, cmd->alt);
	}

	ccr |= (cmd->dummy_size & 0x1F) << QUADSPI_CCR_DCYC_Pos;

	if (cmd->instruction_size > 0)
	{
		ccr |= QUADSPI_CCR_IMODE_0;
		ccr |= cmd->instruction << QUADSPI_CCR_INSTRUCTION_Pos;
		if (cmd->addr_size > 0)
		{
			ccr |= QUADSPI_CCR_ADMODE_0;
			ccr |= ((cmd->addr_size - 1) & 0x3) << QUADSPI_CCR_ADSIZE_Pos;
			WRITE_REG(QUADSPI->CCR, ccr);
			WRITE_REG(QUADSPI->AR, cmd->addr);
		}
		else
		{
			WRITE_REG(QUADSPI->CCR, ccr);
		}
	}
	else
	{
		if (cmd->addr_size > 0)
		{
			ccr |= QUADSPI_CCR_ADMODE_0;
			ccr |= ((cmd->addr_size - 1) & 0x3) << QUADSPI_CCR_ADSIZE_Pos;
			WRITE_REG(QUADSPI->CCR, ccr);
			WRITE_REG(QUADSPI->AR, cmd->addr);
		}
		else
		{
			WRITE_REG(QUADSPI->CCR, ccr);
		}
	}

	uint32_t i;
	for (i = 0; i < n; ++i)
	{
		if (_wait_for_status(QUADSPI_SR_FTF, tot) < 0)
			return i;

		*((__IO uint8_t*) data_reg) = data[i];

	}

	if (_wait_for_status(QUADSPI_SR_TCF, tot) == 0)
	{
		SET_BIT(QUADSPI->FCR, QUADSPI_FCR_CTCF);
	}

	return i;
}

uint32_t STRHAL_QSPI_Indirect_Read(const STRHAL_QSPI_Command_t *cmd, uint8_t *data, uint32_t n, uint16_t tot)
{
	if (_wait_for_status_clear(QUADSPI_SR_BUSY, tot) < 0)
		return -1;

	__IO uint32_t *data_reg = &QUADSPI->DR;
	_clear_status(QUADSPI_FCR_CSMF | QUADSPI_FCR_CTCF | QUADSPI_FCR_CTEF | QUADSPI_FCR_CTOF);

	uint32_t ccr = 0x00000000;

	ccr |= QUADSPI_CCR_FMODE_0;

	if (n > 0)
	{
		ccr |= QUADSPI_CCR_DMODE_0;
		WRITE_REG(QUADSPI->DLR, n - 1U);
	}
	if (cmd->alt_size > 0)
	{
		ccr |= QUADSPI_CCR_ABMODE_0;
		ccr |= ((cmd->alt_size - 1) & 0x3) << QUADSPI_CCR_ABSIZE_Pos;
		WRITE_REG(QUADSPI->ABR, cmd->alt);
	}

	ccr |= (cmd->dummy_size & 0x1F) << QUADSPI_CCR_DCYC_Pos;

	if (cmd->instruction_size > 0)
	{
		ccr |= QUADSPI_CCR_IMODE_0;
		ccr |= cmd->instruction << QUADSPI_CCR_INSTRUCTION_Pos;
		if (cmd->addr_size > 0)
		{
			ccr |= QUADSPI_CCR_ADMODE_0;
			ccr |= ((cmd->addr_size - 1) & 0x3) << QUADSPI_CCR_ADSIZE_Pos;
			WRITE_REG(QUADSPI->CCR, ccr);
			WRITE_REG(QUADSPI->AR, cmd->addr);
		}
		else
		{
			WRITE_REG(QUADSPI->CCR, ccr);
		}
	}
	else
	{
		if (cmd->addr_size > 0)
		{
			ccr |= QUADSPI_CCR_ADMODE_0;
			ccr |= ((cmd->addr_size - 1) & 0x3) << QUADSPI_CCR_ADSIZE_Pos;
			WRITE_REG(QUADSPI->CCR, ccr);
			WRITE_REG(QUADSPI->AR, cmd->addr);
		}
		else
		{
			WRITE_REG(QUADSPI->CCR, ccr);
		}
	}

	if (_wait_for_status(QUADSPI_SR_BUSY, tot) < 0)
		return 0;

	uint32_t i;
	for (i = 0; i < n; ++i)
	{
		if (_wait_for_status(QUADSPI_SR_FTF | QUADSPI_SR_TCF, tot) < 0)
		{
			_clear_status(QUADSPI_SR_TCF);
			return i;
		}
		data[i] = *((__IO uint8_t*) data_reg);
	}

	if (_wait_for_status(QUADSPI_SR_TCF, tot) == 0)
	{
		_clear_status(QUADSPI_SR_TCF);
	}
	return i;
}

int _wait_for_status(uint32_t flag, uint16_t tot)
{
	uint64_t start = STRHAL_Systick_GetTick();

	while (!(QUADSPI->SR & flag))
	{
		if (STRHAL_Systick_GetTick() - start > tot)
			return -1;
	}
	return 0;
}

int _wait_for_status_clear(uint32_t flag, uint16_t tot)
{
	uint64_t start = STRHAL_Systick_GetTick();

	while (QUADSPI->SR & flag)
	{
		if (STRHAL_Systick_GetTick() - start > tot)
			return -1;
	}
	return 0;
}

void _clear_status(uint32_t flags)
{
	QUADSPI->FCR |= (flags);
}
#endif
