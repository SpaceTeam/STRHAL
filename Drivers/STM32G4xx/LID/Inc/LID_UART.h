#ifndef LID_UART_H
#define LID_UART_H

#define LID_UART_BUF_SIZE 256

#include <stdint.h>

#define LID_UART_BAUDRATE 115200

typedef enum {
	LID_UART_STATE_00 = 0,

	LID_UART_STATE_RX = (1<<1),
	LID_UART_STATE_RC = (1<<2),
	LID_UART_STATE_RO = (1<<3),
	LID_UART_STATE_RE = (1<<4),

	LID_UART_STATE_TX = (1<<5),
	LID_UART_STATE_TC = (1<<6),
	LID_UART_STATE_TE = (1<<7),
} LID_UART_State_t;

void LID_UART_Init();

int32_t LID_UART_Write(const char *data, uint32_t n);
int32_t LID_UART_Read(char *data, uint32_t n);

void LID_UART_FlushReception();

LID_UART_State_t LID_UART_Listen();
LID_UART_State_t LID_UART_GetState();



#endif /*LID_UART_H*/
