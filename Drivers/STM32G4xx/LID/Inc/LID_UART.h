#ifndef LID_UART_H
#define LID_UART_H

#define LID_UART_BUF_SIZE 256

#include <stdint.h>

#define LID_UART_BAUDRATE 115200

typedef enum {
	LID_UART_STATE_00 = 0x00,
	LID_UART_STATE_TX = (1<<1),
	LID_UART_STATE_RX = (1<<2)
} LID_UART_State_t;

void LID_UART_Init();

uint32_t LID_UART_Write(const char *data, uint32_t n);
uint32_t LID_UART_Read(char *data, uint32_t n);

uint32_t LID_UART_AsyncWrite(const char *data, uint32_t n);
uint32_t LID_UART_AsyncRead(char *data, uint32_t n);

LID_UART_State_t LID_UART_GetState();

#endif /*LID_UART_H*/
