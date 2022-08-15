#include <stddef.h>
#include <STRHAL_I2C.h>
#include <STRHAL_SysTick.h>

typedef struct
{
	I2C_TypeDef *i2c;
	STRHAL_I2C_Rx_Buf_t rx_buf;
	uint32_t dma_rx_channel;
	uint32_t dma_rx_request;
	uint32_t it_rx_handler;
} STRHAL_I2C_Handle_t;

static STRHAL_I2C_Handle_t _i2cs[1] =
{ [STRHAL_I2C3] =
	{ .i2c = I2C3, .dma_rx_channel = STRHAL_I2C3_DMA_CHANNEL, .dma_rx_request = LL_DMAMUX_REQ_I2C3_RX, .it_rx_handler = STRHAL_I2C3_DMA_CHANNEL_IRQ }
};

static inline int _wait_for_busy(I2C_TypeDef *i2c, uint16_t tot);
static inline int _wait_for_txis(I2C_TypeDef *i2c, uint16_t tot);
static inline int _wait_for_stop(I2C_TypeDef *i2c, uint16_t tot);
static inline int _wait_for_rxne(I2C_TypeDef *i2c, uint16_t tot);

/*
static void STRHAL_I2C_DmaInit(DMA_TypeDef *DMAx, uint32_t dmaChannel, uint32_t dest, uint32_t src, uint32_t periph)
{
	LL_DMA_InitTypeDef DMA_InitStruct =
	{ 0 };

	DMA_InitStruct.Direction = LL_DMA_DIRECTION_PERIPH_TO_MEMORY;
	DMA_InitStruct.MemoryOrM2MDstAddress = dest;
	DMA_InitStruct.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_BYTE;
	DMA_InitStruct.MemoryOrM2MDstIncMode = LL_DMA_MEMORY_INCREMENT;
	DMA_InitStruct.Mode = LL_DMA_MODE_CIRCULAR;
	DMA_InitStruct.NbData = STRHAL_I2C_BUF_SIZE;
	DMA_InitStruct.PeriphOrM2MSrcAddress = src;
	DMA_InitStruct.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_BYTE;
	DMA_InitStruct.PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT;
	DMA_InitStruct.PeriphRequest = periph;
	DMA_InitStruct.Priority = STRHAL_I2C_DMA_PRIORITY;

	LL_DMA_Init(DMAx, dmaChannel, &DMA_InitStruct);
}*/

void STRHAL_I2C_Init()
{
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
	/**I2C3 GPIO Configuration
	PC9   ------> I2C3_SDA
	PA8   ------> I2C3_SCL
	*/
	GPIO_InitStruct.Pin = LL_GPIO_PIN_9;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_8;
	LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LL_GPIO_PIN_8;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_2;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

int STRHAL_I2C_Instance_Init(STRHAL_I2C_Id_t i2c_id)
{
	if (i2c_id < 0 || i2c_id >= STRHAL_N_I2C)
		return -1;

	STRHAL_I2C_Handle_t *_i2c = &_i2cs[i2c_id];

	if (STRHAL_I2C_DMA == DMA1)
	{
		LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
	}
	else if (STRHAL_I2C_DMA == DMA2)
	{
		LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA2);
	}

	if (i2c_id == STRHAL_I2C3)
	{
		LL_RCC_SetI2CClockSource(LL_RCC_I2C3_CLKSOURCE_PCLK1);
		LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C3);
	}
	//else if () TODO Add other I2Cs

	LL_I2C_InitTypeDef I2C_InitStruct = {0};

	I2C_InitStruct.PeripheralMode = LL_I2C_MODE_I2C;
	I2C_InitStruct.Timing = 0x00303D5B;
	I2C_InitStruct.AnalogFilter = LL_I2C_ANALOGFILTER_ENABLE;
	I2C_InitStruct.DigitalFilter = 0;
	I2C_InitStruct.OwnAddress1 = 0;
	I2C_InitStruct.TypeAcknowledge = LL_I2C_ACK;
	I2C_InitStruct.OwnAddrSize = LL_I2C_OWNADDRESS1_7BIT;
	LL_I2C_Init(_i2c->i2c, &I2C_InitStruct);
	LL_I2C_EnableAutoEndMode(_i2c->i2c);
	LL_I2C_SetOwnAddress2(_i2c->i2c, 0, LL_I2C_OWNADDRESS2_NOMASK);
	LL_I2C_DisableOwnAddress2(_i2c->i2c);
	LL_I2C_DisableGeneralCall(_i2c->i2c);
	LL_I2C_EnableClockStretching(_i2c->i2c);

	//Init DMA for I2C
	/*STRHAL_I2C_DmaInit(STRHAL_I2C_DMA, _i2c->dma_rx_channel, (uint32_t) _i2c->rx_buf.data, LL_I2C_DMA_GetRegAddr(_i2c->i2c, LL_I2C_DMA_REG_DATA_RECEIVE), _i2c->dma_rx_request);

	LL_I2C_EnableDMAReq_RX(_i2c->i2c);
	LL_DMA_EnableIT_TC(STRHAL_I2C_DMA, _i2c->dma_rx_channel);
	NVIC_SetPriority(_i2c->it_rx_handler, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 1, 1));
	NVIC_EnableIRQ(_i2c->it_rx_handler);*/

	//LL_I2C_Enable(_i2c->i2c);
	//_i2c->rx_buf.h = _i2c->rx_buf.n = 0;
	//_i2c->rx_buf.n_dma = STRHAL_I2C_BUF_SIZE;

	return 0;
}

int32_t STRHAL_I2C_Transmit(STRHAL_I2C_Id_t i2c_id, uint8_t address, uint8_t *data, uint8_t n)
{
	if (i2c_id < 0 || i2c_id >= STRHAL_N_I2C)
			return -1;

	STRHAL_I2C_Handle_t *_i2c = &_i2cs[i2c_id];

	int32_t ret = n;

	if (_wait_for_busy(_i2c->i2c, 100) != 0)
		return -1;

	uint32_t tmp = ((uint32_t)(((uint32_t) address & I2C_CR2_SADD) | (((uint32_t) n << I2C_CR2_NBYTES_Pos) & I2C_CR2_NBYTES) | (uint32_t) LL_I2C_MODE_AUTOEND | (uint32_t) LL_I2C_GENERATE_START_WRITE) & (~0x80000000U));

	// Update CR2 for AUTOEND and START GENERATION
	MODIFY_REG(_i2c->i2c->CR2, ((I2C_CR2_SADD | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_AUTOEND | (I2C_CR2_RD_WRN & (uint32_t)(LL_I2C_GENERATE_START_WRITE >> (31U - I2C_CR2_RD_WRN_Pos))) | I2C_CR2_START | I2C_CR2_STOP)), tmp);

	while(n > 0)
	{
		if (_wait_for_txis(_i2c->i2c, 100) != 0)
			return -1;

		_i2c->i2c->TXDR = *data;
		data++;
		n--;
	}

	if (_wait_for_stop(_i2c->i2c, 100) != 0)
		return -1;

	LL_I2C_ClearFlag_STOP(_i2c->i2c);

	// Reset CR2
	_i2c->i2c->CR2 &= (uint32_t)~((uint32_t)(I2C_CR2_SADD | I2C_CR2_HEAD10R | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_RD_WRN));

	return (ret-n);
}

int32_t STRHAL_I2C_Receive(STRHAL_I2C_Id_t i2c_id, uint8_t address, uint8_t *data, uint8_t n)
{
	if (i2c_id < 0 || i2c_id >= STRHAL_N_I2C)
			return -1;

	STRHAL_I2C_Handle_t *_i2c = &_i2cs[i2c_id];

	int32_t ret = n;

	if (_wait_for_busy(_i2c->i2c, 100) != 0)
		return -1;

	uint32_t tmp = ((uint32_t)(((uint32_t) address & I2C_CR2_SADD) | (((uint32_t) n << I2C_CR2_NBYTES_Pos) & I2C_CR2_NBYTES) | (uint32_t) LL_I2C_MODE_AUTOEND | (uint32_t) LL_I2C_GENERATE_START_READ) & (~0x80000000U));

	// Update CR2 for AUTOEND and START GENERATION
	MODIFY_REG(_i2c->i2c->CR2, ((I2C_CR2_SADD | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_AUTOEND | (I2C_CR2_RD_WRN & (uint32_t)(LL_I2C_GENERATE_START_READ >> (31U - I2C_CR2_RD_WRN_Pos))) | I2C_CR2_START | I2C_CR2_STOP)), tmp);

	while(n > 0)
	{
		if (_wait_for_rxne(_i2c->i2c, 100) != 0)
			return -1;

		*data = (uint8_t) _i2c->i2c->RXDR;
		data++;
		n--;
	}

	if (_wait_for_stop(_i2c->i2c, 100) != 0)
		return -1;

	LL_I2C_ClearFlag_STOP(_i2c->i2c);

	// Reset CR2
	_i2c->i2c->CR2 &= (uint32_t)~((uint32_t)(I2C_CR2_SADD | I2C_CR2_HEAD10R | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_RD_WRN));

	return (ret-n);
}

int _wait_for_busy(I2C_TypeDef *i2c, uint16_t tot)
{
	uint64_t start = STRHAL_Systick_GetTick();
	while (LL_I2C_IsActiveFlag_BUSY(i2c))
	{
		if (STRHAL_Systick_GetTick() - start > tot)
			return -1;
	}
	return 0;
}

int _wait_for_txis(I2C_TypeDef *i2c, uint16_t tot)
{
	uint64_t start = STRHAL_Systick_GetTick();
	while (!LL_I2C_IsActiveFlag_TXIS(i2c))
	{
		if (STRHAL_Systick_GetTick() - start > tot)
			return -1;
	}
	return 0;
}

int _wait_for_stop(I2C_TypeDef *i2c, uint16_t tot)
{
	uint64_t start = STRHAL_Systick_GetTick();
	while (!LL_I2C_IsActiveFlag_STOP(i2c))
	{
		if (STRHAL_Systick_GetTick() - start > tot)
			return -1;
	}
	return 0;
}

int _wait_for_rxne(I2C_TypeDef *i2c, uint16_t tot)
{
	uint64_t start = STRHAL_Systick_GetTick();
	while (!LL_I2C_IsActiveFlag_RXNE(i2c))
	{
		if (STRHAL_Systick_GetTick() - start > tot)
			return -1;
	}
	return 0;
}

void DMA1_Channel8_IRQHandler(void)
{
	STRHAL_I2C_Handle_t *_i2c = &_i2cs[STRHAL_I2C3];
	if (LL_DMA_IsEnabledIT_TC(DMA1, LL_DMA_CHANNEL_8) && LL_DMA_IsActiveFlag_TC1(DMA1))
	{
		LL_DMA_ClearFlag_TC1(DMA1);
		_i2c->rx_buf.n += _i2c->rx_buf.n_dma;
		_i2c->rx_buf.n_dma = STRHAL_I2C_BUF_SIZE;

		if (_i2c->rx_buf.n > STRHAL_I2C_BUF_SIZE)
		{
			_i2c->rx_buf.h += _i2c->rx_buf.n % STRHAL_I2C_BUF_SIZE;
			_i2c->rx_buf.n = STRHAL_I2C_BUF_SIZE;
		}
	}
}
