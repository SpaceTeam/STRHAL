#include "../Inc/LID_UART.h"
#include "../Inc/LID_Container.h"


static char _rx_mem[LID_UART_BUF_SIZE];
static char _tx_mem[LID_UART_BUF_SIZE];

static LID_LinearBuffer_t _rx_buf;
static LID_LinearBuffer_t _tx_buf;

static LID_UART_State_t _state;

void LID_UART_Init() {
	LID_LinearBuffer_Init(&_rx_buf, (uint8_t*) _rx_mem, LID_UART_BUF_SIZE);
	LID_LinearBuffer_Init(&_tx_buf, (uint8_t*) _tx_mem, LID_UART_BUF_SIZE);


	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = LL_GPIO_PIN_3 | LL_GPIO_PIN_4;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
	LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*
	NVIC_SetPriority(USART2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
	NVIC_EnableIRQ(USART2_IRQn);
	*/

	LL_USART_InitTypeDef UART_InitStruct = {0};
	UART_InitStruct.BaudRate = LID_UART_BAUDRATE;
	UART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
	UART_InitStruct.StopBits = LL_USART_STOPBITS_1;
	UART_InitStruct.Parity = LL_USART_PARITY_NONE;
	UART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
	UART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
	UART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
	LL_USART_Init(USART2, &USART_InitStruct);
	LL_USART_ConfigAsyncMode(USART2);

	LL_USART_EnableDirectionTx(USART2);
	LL_USART_EnableDirectionRx(USART2);
}

uint32_t LID_UART_Write(const char *data, uint32_t n) {
	return n;
}
uint32_t LID_UART_Read(char *data, uint32_t n) {
	return n;
}

uint32_t LID_UART_AsyncWrite(const char *data, uint32_t n) {
	return n;
}

uint32_t LID_UART_AsyncRead(char *data, uint32_t n) {
	return n;
}

LID_UART_State_t LID_UART_GetState() {
	return _state;
}
