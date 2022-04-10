#ifndef STRHAL_UART_H
#define STRHAL_UART_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define STRHAL_UART_BUF_SIZE 256
#define STRHAL_UART_BAUDRATE 115200

typedef enum {
	STRHAL_UART_STATE_00 = 0,

	STRHAL_UART_STATE_RX = (1<<1),
	STRHAL_UART_STATE_RC = (1<<2),
	STRHAL_UART_STATE_RO = (1<<3),
	STRHAL_UART_STATE_RE = (1<<4),

	STRHAL_UART_STATE_TX = (1<<5),
	STRHAL_UART_STATE_TC = (1<<6),
	STRHAL_UART_STATE_TE = (1<<7),
} STRHAL_UART_State_t;

void STRHAL_UART_Init();

int32_t STRHAL_UART_Write(const char *data, uint32_t n);
int32_t STRHAL_UART_Read(char *data, uint32_t n);

void STRHAL_UART_FlushReception();

STRHAL_UART_State_t STRHAL_UART_Listen();
STRHAL_UART_State_t STRHAL_UART_GetState();


#ifdef __cplusplus
}
#endif

#endif /*STRHAL_UART_H*/
