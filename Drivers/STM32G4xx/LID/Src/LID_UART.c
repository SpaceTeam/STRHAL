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
