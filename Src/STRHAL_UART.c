#include <stm32g4xx_ll_bus.h>
#include <stm32g4xx_ll_cortex.h>
#include <stm32g4xx_ll_dma.h>
#include <stm32g4xx_ll_dmamux.h>
#include <stm32g4xx_ll_gpio.h>
#include <stm32g4xx_ll_rcc.h>
#include <stm32g4xx_ll_usart.h>
#include <STRHAL_Oof.h>
#include <STRHAL_UART.h>
#include <STRHAL_SysTick.h>

typedef struct {
	USART_TypeDef *uart;
	STRHAL_UART_Tx_Buf_t tx_buf;
	STRHAL_UART_Rx_Buf_t rx_buf;
	STRHAL_UART_State_t state;
	uint32_t dma_rx_channel;
	uint32_t dma_tx_channel;
	uint32_t dma_rx_request;
	uint32_t dma_tx_request;
	uint32_t it_rx_handler;
	uint32_t it_tx_handler;
	uint32_t baud_rate;
	uint8_t swap; // swap tx/rx pins
} STRHAL_UART_Handle_t;

static STRHAL_UART_Handle_t _uarts[2] = {
	[STRHAL_UART1] =
	{
			.uart = USART1,
			.dma_rx_channel = LL_DMA_CHANNEL_1,
			.dma_tx_channel = LL_DMA_CHANNEL_2,
			.dma_rx_request = LL_DMAMUX_REQ_USART1_RX,
			.dma_tx_request = LL_DMAMUX_REQ_USART1_TX,
			.it_rx_handler = DMA1_Channel1_IRQn,
			.it_tx_handler = DMA1_Channel2_IRQn,
			.baud_rate = 9600,
			.swap = 1
	},
	[STRHAL_UART2] =
	{
			.uart = USART2,
			.dma_rx_channel = LL_DMA_CHANNEL_3,
			.dma_tx_channel = LL_DMA_CHANNEL_4,
			.dma_rx_request = LL_DMAMUX_REQ_USART2_RX,
			.dma_tx_request = LL_DMAMUX_REQ_USART2_TX,
			.it_rx_handler = DMA1_Channel3_IRQn,
			.it_tx_handler = DMA1_Channel4_IRQn,
			.baud_rate = 115200,
			.swap = 0
	}
};

void STRHAL_UART_Init() {
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMAMUX1);
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = LL_GPIO_PIN_9 | LL_GPIO_PIN_10;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LL_GPIO_PIN_3 | LL_GPIO_PIN_4;
	LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

int STRHAL_UART_Instance_Init(STRHAL_UART_Id_t uart_id) {
	if(uart_id < 0 || uart_id >= STRHAL_N_UART)
		return -1;

	STRHAL_UART_Handle_t *_uart = &_uarts[uart_id];

	LL_USART_Disable(_uart->uart);

	LL_USART_InitTypeDef UART_InitStruct = {0};
	UART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
	UART_InitStruct.StopBits = LL_USART_STOPBITS_1;
	UART_InitStruct.Parity = LL_USART_PARITY_NONE;
	UART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
	UART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
	UART_InitStruct.BaudRate = _uart->baud_rate;
	UART_InitStruct.PrescalerValue = LL_USART_PRESCALER_DIV1;
	UART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
	LL_USART_Init(_uart->uart, &UART_InitStruct);


	LL_USART_SetTXFIFOThreshold(_uart->uart, LL_USART_FIFOTHRESHOLD_1_8);
	LL_USART_SetRXFIFOThreshold(_uart->uart, LL_USART_FIFOTHRESHOLD_1_8);
	LL_USART_DisableFIFO(_uart->uart);
	LL_USART_ConfigAsyncMode(_uart->uart);
	if(_uart->swap)
		LL_USART_SetTXRXSwap(_uart->uart,LL_USART_TXRX_SWAPPED);

	LL_DMA_InitTypeDef DMA_InitStruct = {0};

	DMA_InitStruct.Direction = LL_DMA_DIRECTION_PERIPH_TO_MEMORY;
	DMA_InitStruct.MemoryOrM2MDstAddress = (uint32_t) _uart->rx_buf.data;
	DMA_InitStruct.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_BYTE;
	DMA_InitStruct.MemoryOrM2MDstIncMode = LL_DMA_MEMORY_INCREMENT;
	DMA_InitStruct.Mode = LL_DMA_MODE_CIRCULAR;
	DMA_InitStruct.NbData = STRHAL_UART_BUF_SIZE;
	DMA_InitStruct.PeriphOrM2MSrcAddress = (uint32_t) &(_uart->uart->RDR);
	DMA_InitStruct.PeriphOrM2MSrcDataSize = (uint32_t) LL_DMA_PDATAALIGN_BYTE;
	DMA_InitStruct.PeriphOrM2MSrcIncMode = LL_DMA_MEMORY_NOINCREMENT;
	DMA_InitStruct.PeriphRequest = _uart->dma_rx_request;
	DMA_InitStruct.Priority = LL_DMA_PRIORITY_HIGH;

	LL_DMA_Init(DMA1, _uart->dma_rx_channel, &DMA_InitStruct);

	LL_USART_EnableDMAReq_RX(_uart->uart);

	LL_DMA_EnableIT_TC(DMA1, _uart->dma_rx_channel);
	LL_DMA_EnableIT_TE(DMA1, _uart->dma_rx_channel);

    NVIC_SetPriority(_uart->it_rx_handler, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 1, 1));
    NVIC_EnableIRQ(_uart->it_rx_handler);


	/*DMA configuration for TX */
	DMA_InitStruct.Direction = LL_DMA_DIRECTION_MEMORY_TO_PERIPH;
	DMA_InitStruct.MemoryOrM2MDstAddress = (uint32_t) _uart->tx_buf.data;
	DMA_InitStruct.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_BYTE;
	DMA_InitStruct.MemoryOrM2MDstIncMode = LL_DMA_MEMORY_INCREMENT;
	DMA_InitStruct.Mode = LL_DMA_MODE_NORMAL;
	DMA_InitStruct.NbData = STRHAL_UART_BUF_SIZE;
	DMA_InitStruct.PeriphOrM2MSrcAddress = (uint32_t) &(_uart->uart->TDR);
	DMA_InitStruct.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_BYTE;
	DMA_InitStruct.PeriphOrM2MSrcIncMode = LL_DMA_MEMORY_NOINCREMENT;
	DMA_InitStruct.PeriphRequest = _uart->dma_tx_request;
	DMA_InitStruct.Priority = LL_DMA_PRIORITY_HIGH;
	LL_DMA_Init(DMA1, _uart->dma_tx_channel, &DMA_InitStruct);

	LL_USART_EnableDMAReq_TX(_uart->uart);

	LL_DMA_EnableIT_TC(DMA1, _uart->dma_tx_channel);
	LL_DMA_EnableIT_TE(DMA1, _uart->dma_tx_channel);

    NVIC_SetPriority(_uart->it_tx_handler, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 1, 2));
    NVIC_EnableIRQ(_uart->it_tx_handler);

    LL_USART_EnableDirectionTx(_uart->uart);
	LL_USART_EnableDirectionRx(_uart->uart);
    LL_USART_Enable(_uart->uart);

    _uart->state = STRHAL_UART_STATE_00;
    _uart->rx_buf.h = _uart->rx_buf.n = _uart->tx_buf.n = 0;
    _uart->rx_buf.n_dma = STRHAL_UART_BUF_SIZE;
    _uart->tx_buf.n = 0;
    return 0;
}

int32_t STRHAL_UART_Debug_Write_DMA(const char *data, uint32_t n) {
	return STRHAL_UART_Write_DMA(STRHAL_UART_DEBUG, data, n);
}

int32_t STRHAL_UART_Debug_Write_Blocking(const char *data, uint32_t n, uint16_t timeout) {
	return STRHAL_UART_Write_Blocking(STRHAL_UART_DEBUG, data, n, timeout);
}

int32_t STRHAL_UART_Write_DMA(STRHAL_UART_Id_t uart_id, const char *data, uint32_t n) {
	if(uart_id < 0 || uart_id >= STRHAL_N_UART)
		return -1;

	STRHAL_UART_Handle_t *_uart = &_uarts[uart_id];

	if(_uart->state & (STRHAL_UART_STATE_TX | STRHAL_UART_STATE_TE) || n > STRHAL_UART_BUF_SIZE)
		return 0;

	uint32_t i;
	for(i = 0; i < n; ++i)
		_uart->tx_buf.data[i] = data[i];

	_uart->tx_buf.n = n;

	LL_DMA_DisableChannel(DMA1, _uart->dma_tx_channel);
	while(LL_DMA_IsEnabledChannel(DMA1, _uart->dma_tx_channel))
		;

	if(_uart->dma_tx_channel == 1U) {
		LL_DMA_ClearFlag_TC2(DMA1);
		LL_DMA_ClearFlag_TE2(DMA1);
	} else if(_uart->dma_tx_channel == 3U) {
		LL_DMA_ClearFlag_TC4(DMA1);
		LL_DMA_ClearFlag_TE4(DMA1);
	}

    _uart->state &= ~STRHAL_UART_STATE_TC;
    _uart->state |= STRHAL_UART_STATE_TX;

	LL_DMA_SetMemoryAddress(DMA1, _uart->dma_tx_channel, (uint32_t) _uart->tx_buf.data);
	LL_DMA_SetDataLength(DMA1, _uart->dma_tx_channel, n);

    LL_DMA_EnableChannel(DMA1, _uart->dma_tx_channel);
    while(!LL_DMA_IsEnabledChannel(DMA1, _uart->dma_tx_channel))
    	;

	return n;
}

int32_t STRHAL_UART_Write_Blocking(STRHAL_UART_Id_t uart_id, const char *data, uint32_t n, uint16_t timeout) {
	if(uart_id < 0 || uart_id >= STRHAL_N_UART)
		return -1;

	STRHAL_UART_Handle_t *_uart = &_uarts[uart_id];

	if(_uart->state & (STRHAL_UART_STATE_TX | STRHAL_UART_STATE_TE) || n > STRHAL_UART_BUF_SIZE)
		return 0;

	uint32_t i;
	for(i = 0; i < n; ++i)
		_uart->tx_buf.data[i] = data[i];

	_uart->tx_buf.n = n;

    _uart->state &= ~STRHAL_UART_STATE_TC;
    _uart->state |= STRHAL_UART_STATE_TX;

    uint8_t *tx_data = (uint8_t *) _uart->tx_buf.data;

    uint64_t tx_starttime = STRHAL_Systick_GetTick();

	while (_uart->tx_buf.n > 0) {
		// wait until byte transmitted, or timeout
		while(!LL_USART_IsActiveFlag_TXE_TXFNF(_uart->uart)) {
			if(STRHAL_Systick_GetTick() - tx_starttime > timeout)
				return -1;
		}
		_uart->uart->TDR  = (uint8_t)(*tx_data & 0xFFU);
		tx_data++;
		_uart->tx_buf.n--;
	}

	// wait until transmission complete, or timeout
	while(!LL_USART_IsActiveFlag_TC(_uart->uart)) {
		if(STRHAL_Systick_GetTick() - tx_starttime > timeout)
			return -1;
	}

    _uart->state &= ~STRHAL_UART_STATE_TX;
    _uart->state |= STRHAL_UART_STATE_TC;

	return n;
}

int32_t STRHAL_UART_Read(STRHAL_UART_Id_t uart_id, char *data, uint32_t n) {
	if(uart_id < 0 || uart_id >= STRHAL_N_UART)
		return -1;

	STRHAL_UART_Handle_t *_uart = &_uarts[uart_id];

	if(_uart->state & STRHAL_UART_STATE_RE)
		return -1;

	if(!(_uart->state & (STRHAL_UART_STATE_RX | STRHAL_UART_STATE_RC)))
		return 0;

	LL_DMA_DisableIT_TC(DMA1, _uart->dma_rx_channel);
	LL_DMA_DisableIT_TE(DMA1, _uart->dma_rx_channel);

	if(_uart->rx_buf.n == 0) {
		uint32_t res = LL_DMA_GetDataLength(DMA1, _uart->dma_rx_channel);
		uint32_t ava =  _uart->rx_buf.n_dma - res;

		if(ava > STRHAL_UART_BUF_SIZE) {
			_uart->state |= STRHAL_UART_STATE_RO;
			_uart->rx_buf.h = 0;
			_uart->rx_buf.n = 0;
			_uart->rx_buf.n_dma = res;
			return -1;
		}

		if(ava == 0)
			return 0;

		_uart->rx_buf.n += ava;
		_uart->rx_buf.n_dma = res;
	}

	if(n > _uart->rx_buf.n)
		n = _uart->rx_buf.n;

	uint32_t i;
	uint32_t j = (n > (STRHAL_UART_BUF_SIZE - _uart->rx_buf.h)) ? (STRHAL_UART_BUF_SIZE - _uart->rx_buf.h) : n;

	for(i = 0; i < j; ++i)
		data[i] = _uart->rx_buf.data[_uart->rx_buf.h+i];

	for(; i < n; ++i)
		data[i] = _uart->rx_buf.data[i-j];

	_uart->rx_buf.h += n;
	_uart->rx_buf.h %= STRHAL_UART_BUF_SIZE;
	_uart->rx_buf.n -= n;

	LL_DMA_EnableIT_TC(DMA1, _uart->dma_rx_channel);
	LL_DMA_EnableIT_TE(DMA1, _uart->dma_rx_channel);

	return n;
}

STRHAL_UART_State_t STRHAL_UART_Listen(STRHAL_UART_Id_t uart_id) {
	if(uart_id < 0 || uart_id >= STRHAL_N_UART)
		return 0; // TODO this error is not handled correctly

	STRHAL_UART_Handle_t *_uart = &_uarts[uart_id];

	if(!(_uart->state & STRHAL_UART_STATE_RE) && !(_uart->state & STRHAL_UART_STATE_RX)) {
        LL_DMA_DisableChannel(DMA1, _uart->dma_rx_channel);

	    while(LL_DMA_IsEnabledChannel(DMA1, _uart->dma_rx_channel))
		;

	    LL_DMA_SetMemoryAddress(DMA1, _uart->dma_rx_channel, (uint32_t) _uart->rx_buf.data);
	    LL_DMA_SetDataLength(DMA1, _uart->dma_rx_channel, STRHAL_UART_BUF_SIZE);

		if(_uart->dma_rx_channel == 0U) {
			LL_DMA_ClearFlag_TC1(DMA1);
			LL_DMA_ClearFlag_TE1(DMA1);
		} else if(_uart->dma_rx_channel == 2U) {
			LL_DMA_ClearFlag_TC3(DMA1);
			LL_DMA_ClearFlag_TE3(DMA1);
		}

	    LL_DMA_EnableChannel(DMA1, _uart->dma_rx_channel);

	    while(!LL_DMA_IsEnabledChannel(DMA1, _uart->dma_rx_channel));

	    _uart->state &= ~(STRHAL_UART_STATE_RC | STRHAL_UART_STATE_RE);
	    _uart->state |= STRHAL_UART_STATE_RX;
	}

	return _uart->state;
}

int STRHAL_UART_FlushReception(STRHAL_UART_Id_t uart_id) {
	if(uart_id < 0 || uart_id >= STRHAL_N_UART)
		return -1;

	STRHAL_UART_Handle_t *_uart = &_uarts[uart_id];

	LL_DMA_DisableChannel(DMA1, _uart->dma_rx_channel);

	while(LL_DMA_IsEnabledChannel(DMA1, _uart->dma_rx_channel))
	;

	_uart->rx_buf.n = 0;
	_uart->rx_buf.h = 0;
	_uart->rx_buf.n_dma = STRHAL_UART_BUF_SIZE;

	_uart->state &= ~(STRHAL_UART_STATE_RX | STRHAL_UART_STATE_RO | STRHAL_UART_STATE_RC);

    LL_DMA_SetMemoryAddress(DMA1, _uart->dma_rx_channel, (uint32_t) _uart->rx_buf.data);
    LL_DMA_SetDataLength(DMA1, _uart->dma_rx_channel, STRHAL_UART_BUF_SIZE);

	if(_uart->dma_rx_channel == 0U) {
		LL_DMA_ClearFlag_TC1(DMA1);
		LL_DMA_ClearFlag_TE1(DMA1);
	} else if(_uart->dma_rx_channel == 2U) {
		LL_DMA_ClearFlag_TC3(DMA1);
		LL_DMA_ClearFlag_TE3(DMA1);
	}

    LL_DMA_EnableChannel(DMA1, _uart->dma_rx_channel);

    while(!LL_DMA_IsEnabledChannel(DMA1, _uart->dma_rx_channel))
    	;

    return 0;
}

STRHAL_UART_State_t STRHAL_UART_GetState(STRHAL_UART_Id_t uart_id) {
	return _uarts[uart_id].state;
}

void DMA1_Channel1_IRQHandler(void) {
	STRHAL_UART_Handle_t *_uart = &_uarts[STRHAL_UART1];
	if(LL_DMA_IsEnabledIT_TC(DMA1, LL_DMA_CHANNEL_1) && LL_DMA_IsActiveFlag_TC1(DMA1)) {
	    LL_DMA_ClearFlag_TC1(DMA1);
		_uart->rx_buf.n += _uart->rx_buf.n_dma;
		_uart->rx_buf.n_dma = STRHAL_UART_BUF_SIZE;

		if(_uart->rx_buf.n > STRHAL_UART_BUF_SIZE) {
			_uart->rx_buf.h += _uart->rx_buf.n % STRHAL_UART_BUF_SIZE;
			_uart->rx_buf.n = STRHAL_UART_BUF_SIZE;

			_uart->state |= STRHAL_UART_STATE_RO;
		}
	}

	else if(LL_DMA_IsEnabledIT_TE(DMA1, LL_DMA_CHANNEL_1) && LL_DMA_IsActiveFlag_TE1(DMA1)) {
	    LL_DMA_ClearFlag_TE1(DMA1);
		_uart->rx_buf.n = STRHAL_UART_BUF_SIZE - LL_DMA_GetDataLength(DMA1, LL_DMA_CHANNEL_1);

		_uart->state &= ~STRHAL_UART_STATE_RX;
		_uart->state &= ~STRHAL_UART_STATE_RC;
		_uart->state |= STRHAL_UART_STATE_RE;

		STRHAL_OofHandler(STRHAL_OOF_UART, "DMA FAIL");
	}
}

void DMA1_Channel2_IRQHandler(void) {
	STRHAL_UART_Handle_t *_uart = &_uarts[STRHAL_UART1];
	if(LL_DMA_IsEnabledIT_TC(DMA1, LL_DMA_CHANNEL_2) && LL_DMA_IsActiveFlag_TC2(DMA1)) {
		LL_DMA_ClearFlag_TC2(DMA1);
		_uart->state &= ~STRHAL_UART_STATE_TX;
		_uart->state |= STRHAL_UART_STATE_TC;
	}

	else if(LL_DMA_IsEnabledIT_TE(DMA1, LL_DMA_CHANNEL_2) && LL_DMA_IsActiveFlag_TE2(DMA1)) {
		LL_DMA_ClearFlag_TE2(DMA1);
		_uart->state &= ~STRHAL_UART_STATE_TX;
		_uart->state &= ~STRHAL_UART_STATE_TC;
		_uart->state |= STRHAL_UART_STATE_TE;
	}
}

void DMA1_Channel3_IRQHandler(void) {
	STRHAL_UART_Handle_t *_uart = &_uarts[STRHAL_UART2];
	if(LL_DMA_IsEnabledIT_TC(DMA1, LL_DMA_CHANNEL_3) && LL_DMA_IsActiveFlag_TC3(DMA1)) {
	    LL_DMA_ClearFlag_TC3(DMA1);
		_uart->rx_buf.n += _uart->rx_buf.n_dma;
		_uart->rx_buf.n_dma = STRHAL_UART_BUF_SIZE;

		if(_uart->rx_buf.n > STRHAL_UART_BUF_SIZE) {
			_uart->rx_buf.h += _uart->rx_buf.n % STRHAL_UART_BUF_SIZE;
			_uart->rx_buf.n = STRHAL_UART_BUF_SIZE;

			_uart->state |= STRHAL_UART_STATE_RO;
		}
	}

	else if(LL_DMA_IsEnabledIT_TE(DMA1, LL_DMA_CHANNEL_3) && LL_DMA_IsActiveFlag_TE3(DMA1)) {
	    LL_DMA_ClearFlag_TE3(DMA1);
		_uart->rx_buf.n = STRHAL_UART_BUF_SIZE - LL_DMA_GetDataLength(DMA1, LL_DMA_CHANNEL_3);

		_uart->state &= ~STRHAL_UART_STATE_RX;
		_uart->state &= ~STRHAL_UART_STATE_RC;
		_uart->state |= STRHAL_UART_STATE_RE;

		STRHAL_OofHandler(STRHAL_OOF_UART, "DMA FAIL");
	}
}

void DMA1_Channel4_IRQHandler(void) {
	STRHAL_UART_Handle_t *_uart = &_uarts[STRHAL_UART2];
	if(LL_DMA_IsEnabledIT_TC(DMA1, LL_DMA_CHANNEL_4) && LL_DMA_IsActiveFlag_TC4(DMA1)) {
		LL_DMA_ClearFlag_TC4(DMA1);
		_uart->state &= ~STRHAL_UART_STATE_TX;
		_uart->state |= STRHAL_UART_STATE_TC;
	}

	else if(LL_DMA_IsEnabledIT_TE(DMA1, LL_DMA_CHANNEL_4) && LL_DMA_IsActiveFlag_TE4(DMA1)) {
		LL_DMA_ClearFlag_TE4(DMA1);
		_uart->state &= ~STRHAL_UART_STATE_TX;
		_uart->state &= ~STRHAL_UART_STATE_TC;
		_uart->state |= STRHAL_UART_STATE_TE;
	}
}
