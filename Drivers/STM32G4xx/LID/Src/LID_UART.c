#include "../Inc/LID_Oof.h"
#include "../Inc/LID_UART.h"
#include <stm32g4xx_ll_rcc.h>
#include <stm32g4xx_ll_bus.h>
#include <stm32g4xx_ll_cortex.h>
#include <stm32g4xx_ll_gpio.h>
#include <stm32g4xx_ll_usart.h>
#include <stm32g4xx_ll_dma.h>
#include <stm32g4xx_ll_dmamux.h>

static struct {
	struct {
		char data[LID_UART_BUF_SIZE];
		uint32_t n;
	} tx_buf;

	struct {
		char data[LID_UART_BUF_SIZE];
		uint32_t h;
		uint32_t n;
		uint32_t n_dma;
	} rx_buf;

	LID_UART_State_t state;
}_uart;

void LID_UART_Init() {
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMAMUX1);
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = LL_GPIO_PIN_3 | LL_GPIO_PIN_4;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
	LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	LL_USART_InitTypeDef UART_InitStruct = {0};
	UART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
	UART_InitStruct.StopBits = LL_USART_STOPBITS_1;
	UART_InitStruct.Parity = LL_USART_PARITY_NONE;
	UART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
	UART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
	UART_InitStruct.BaudRate = LID_UART_BAUDRATE;
	UART_InitStruct.PrescalerValue = LL_USART_PRESCALER_DIV1;
	UART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
	LL_USART_Init(USART2, &UART_InitStruct);


	LL_USART_SetTXFIFOThreshold(USART2, LL_USART_FIFOTHRESHOLD_1_8);
	LL_USART_SetRXFIFOThreshold(USART2, LL_USART_FIFOTHRESHOLD_1_8);
	LL_USART_DisableFIFO(USART2);
	LL_USART_ConfigAsyncMode(USART2);

	LL_DMA_InitTypeDef DMA_InitStruct = {0};

	DMA_InitStruct.Direction = LL_DMA_DIRECTION_PERIPH_TO_MEMORY;
	DMA_InitStruct.MemoryOrM2MDstAddress = (uint32_t) _uart.rx_buf.data;
	DMA_InitStruct.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_BYTE;
	DMA_InitStruct.MemoryOrM2MDstIncMode = LL_DMA_MEMORY_INCREMENT;
	DMA_InitStruct.Mode = LL_DMA_MODE_CIRCULAR;
	DMA_InitStruct.NbData = LID_UART_BUF_SIZE;
	DMA_InitStruct.PeriphOrM2MSrcAddress = (uint32_t) &USART2->RDR;
	DMA_InitStruct.PeriphOrM2MSrcDataSize = (uint32_t) LL_DMA_PDATAALIGN_BYTE;
	DMA_InitStruct.PeriphOrM2MSrcIncMode = LL_DMA_MEMORY_NOINCREMENT;
	DMA_InitStruct.PeriphRequest = LL_DMAMUX_REQ_USART2_RX;
	DMA_InitStruct.Priority = LL_DMA_PRIORITY_HIGH;

	LL_DMA_Init(DMA1, LL_DMA_CHANNEL_1, &DMA_InitStruct);

	LL_USART_EnableDMAReq_RX(USART2);

	LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_1);
	LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_1);

    NVIC_SetPriority(DMA1_Channel1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 1, 1));
    NVIC_EnableIRQ(DMA1_Channel1_IRQn);


	/*DMA configuration for TX */
	DMA_InitStruct.Direction = LL_DMA_DIRECTION_MEMORY_TO_PERIPH;
	DMA_InitStruct.MemoryOrM2MDstAddress = (uint32_t) _uart.tx_buf.data;
	DMA_InitStruct.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_BYTE;
	DMA_InitStruct.MemoryOrM2MDstIncMode = LL_DMA_MEMORY_INCREMENT;
	DMA_InitStruct.Mode = LL_DMA_MODE_NORMAL;
	DMA_InitStruct.NbData = LID_UART_BUF_SIZE;
	DMA_InitStruct.PeriphOrM2MSrcAddress = (uint32_t) &USART2->TDR;
	DMA_InitStruct.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_BYTE;
	DMA_InitStruct.PeriphOrM2MSrcIncMode = LL_DMA_MEMORY_NOINCREMENT;
	DMA_InitStruct.PeriphRequest = LL_DMAMUX_REQ_USART2_TX;
	DMA_InitStruct.Priority = LL_DMA_PRIORITY_HIGH;
	LL_DMA_Init(DMA1, LL_DMA_CHANNEL_2, &DMA_InitStruct);

	LL_USART_EnableDMAReq_TX(USART2);

	LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_2);
	LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_2);

    NVIC_SetPriority(DMA1_Channel2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 1, 2));
    NVIC_EnableIRQ(DMA1_Channel2_IRQn);

    LL_USART_EnableDirectionTx(USART2);
	LL_USART_EnableDirectionRx(USART2);
    LL_USART_Enable(USART2);


    _uart.state = LID_UART_STATE_00;
    _uart.rx_buf.h = _uart.rx_buf.n = _uart.tx_buf.n = 0;
    _uart.rx_buf.n_dma = LID_UART_BUF_SIZE;
    _uart.tx_buf.n = 0;
}

int32_t LID_UART_Write(const char *data, uint32_t n) {
	if(_uart.state & (LID_UART_STATE_TX | LID_UART_STATE_TE) || n > LID_UART_BUF_SIZE)
		return 0;

	uint32_t i;
	for(i = 0; i < n; ++i)
		_uart.tx_buf.data[i] = data[i];

	_uart.tx_buf.n = n;

	LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_2);
	while(LL_DMA_IsEnabledChannel(DMA1, LL_DMA_CHANNEL_2))
		;


    LL_DMA_ClearFlag_TC2(DMA1);
    LL_DMA_ClearFlag_TE2(DMA1);

    _uart.state &= ~LID_UART_STATE_TC;
    _uart.state |= LID_UART_STATE_TX;

	LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_2, (uint32_t) _uart.tx_buf.data);
	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_2, n);

    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_2);
    while(!LL_DMA_IsEnabledChannel(DMA1, LL_DMA_CHANNEL_2))
    	;

	return n;
}

int32_t LID_UART_Read(char *data, uint32_t n) {
	if(_uart.state & LID_UART_STATE_RE)
		return -1;

	if(!(_uart.state & (LID_UART_STATE_RX | LID_UART_STATE_RC)))
		return 0;

	LL_DMA_DisableIT_TC(DMA1, LL_DMA_CHANNEL_1);
	LL_DMA_DisableIT_TE(DMA1, LL_DMA_CHANNEL_1);

	if(_uart.rx_buf.n == 0) {
		uint32_t res = LL_DMA_GetDataLength(DMA1, LL_DMA_CHANNEL_1);
		uint32_t ava =  _uart.rx_buf.n_dma - res;

		if(ava > LID_UART_BUF_SIZE) {
			_uart.state |= LID_UART_STATE_RO;
			_uart.rx_buf.h = 0;
			_uart.rx_buf.n = 0;
			_uart.rx_buf.n_dma = res;
			return -1;
		}

		if(ava == 0)
			return 0;

		_uart.rx_buf.n += ava;
		_uart.rx_buf.n_dma = res;
	}

	if(n > _uart.rx_buf.n)
		n = _uart.rx_buf.n;

	uint32_t i;
	uint32_t j = (n > (LID_UART_BUF_SIZE - _uart.rx_buf.h)) ? (LID_UART_BUF_SIZE - _uart.rx_buf.h) : n;

	for(i = 0; i < j; ++i)
		data[i] = _uart.rx_buf.data[_uart.rx_buf.h+i];

	for(; i < n; ++i)
		data[i] = _uart.rx_buf.data[i-j];

	_uart.rx_buf.h += n;
	_uart.rx_buf.h %= LID_UART_BUF_SIZE;
	_uart.rx_buf.n -= n;

	LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_1);
	LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_1);

	return n;
}

LID_UART_State_t LID_UART_Listen() {
	if(!(_uart.state & LID_UART_STATE_RE) && !(_uart.state & LID_UART_STATE_RX)) {
        LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_1);

	    while(LL_DMA_IsEnabledChannel(DMA1, LL_DMA_CHANNEL_1))
		;

	    LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_1, (uint32_t) _uart.rx_buf.data);
	    LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_1, LID_UART_BUF_SIZE);

	    LL_DMA_ClearFlag_TC1(DMA1);
	    LL_DMA_ClearFlag_TE1(DMA1);

	    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);

	    while(!LL_DMA_IsEnabledChannel(DMA1, LL_DMA_CHANNEL_1));

	    _uart.state &= ~(LID_UART_STATE_RC | LID_UART_STATE_RE);
	    _uart.state |= LID_UART_STATE_RX;
	}

	return _uart.state;
}

void LID_UART_FlushReception() {
	LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_1);

	while(LL_DMA_IsEnabledChannel(DMA1, LL_DMA_CHANNEL_1))
	;

	_uart.rx_buf.n = 0;
	_uart.rx_buf.h = 0;
	_uart.rx_buf.n_dma = LID_UART_BUF_SIZE;

	_uart.state &= ~(LID_UART_STATE_RX | LID_UART_STATE_RO | LID_UART_STATE_RC);


    LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_1, (uint32_t) _uart.rx_buf.data);
    LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_1, LID_UART_BUF_SIZE);

    LL_DMA_ClearFlag_TC1(DMA1);
    LL_DMA_ClearFlag_TE1(DMA1);

    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);

    while(!LL_DMA_IsEnabledChannel(DMA1, LL_DMA_CHANNEL_1))
    	;
}

LID_UART_State_t LID_UART_GetState() {
	return _uart.state;
}

void DMA1_Channel1_IRQHandler(void) {
	if(LL_DMA_IsEnabledIT_TC(DMA1, LL_DMA_CHANNEL_1) && LL_DMA_IsActiveFlag_TC1(DMA1)) {
	    LL_DMA_ClearFlag_TC1(DMA1);
		_uart.rx_buf.n += _uart.rx_buf.n_dma;
		_uart.rx_buf.n_dma = LID_UART_BUF_SIZE;

		if(_uart.rx_buf.n > LID_UART_BUF_SIZE) {
			_uart.rx_buf.h += _uart.rx_buf.n % LID_UART_BUF_SIZE;
			_uart.rx_buf.n = LID_UART_BUF_SIZE;

			_uart.state |= LID_UART_STATE_RO;
		}
	}

	else if(LL_DMA_IsEnabledIT_TE(DMA1, LL_DMA_CHANNEL_1) && LL_DMA_IsActiveFlag_TE1(DMA1)) {
	    LL_DMA_ClearFlag_TE1(DMA1);
		_uart.rx_buf.n = LID_UART_BUF_SIZE - LL_DMA_GetDataLength(DMA1, LL_DMA_CHANNEL_1);

		_uart.state &= ~LID_UART_STATE_RX;
		_uart.state &= ~LID_UART_STATE_RC;
		_uart.state |= LID_UART_STATE_RE;

		LID_OofHandler(LID_OOF_UART, "DMA FAIL");
	}
}

void DMA1_Channel2_IRQHandler(void) {
	if(LL_DMA_IsEnabledIT_TC(DMA1, LL_DMA_CHANNEL_2) && LL_DMA_IsActiveFlag_TC2(DMA1)) {
		LL_DMA_ClearFlag_TC2(DMA1);
		_uart.state &= ~LID_UART_STATE_TX;
		_uart.state |= LID_UART_STATE_TC;
	}

	else if(LL_DMA_IsEnabledIT_TE(DMA1, LL_DMA_CHANNEL_2) && LL_DMA_IsActiveFlag_TE2(DMA1)) {
		LL_DMA_ClearFlag_TE2(DMA1);
		_uart.state &= ~LID_UART_STATE_TX;
		_uart.state &= ~LID_UART_STATE_TC;
		_uart.state |= LID_UART_STATE_TE;
	}
}
