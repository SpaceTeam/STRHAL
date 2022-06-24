#ifndef STRHAL_UART_H
#define STRHAL_UART_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define STRHAL_UART_BUF_SIZE 256
//#define STRHAL_UART_BAUDRATE 115200 // not used since it's set inside the uart struct

#define STRHAL_UART_DEBUG STRHAL_UART2

typedef enum {
	STRHAL_UART1,
	STRHAL_UART2,

	STRHAL_N_UART
} STRHAL_UART_Id_t;

typedef struct {
	char data[STRHAL_UART_BUF_SIZE];
	uint32_t n;
} STRHAL_UART_Tx_Buf_t;

typedef struct {
	char data[STRHAL_UART_BUF_SIZE];
	uint32_t h;
	uint32_t n;
	uint32_t n_dma;
} STRHAL_UART_Rx_Buf_t;

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
int STRHAL_UART_Instance_Init(STRHAL_UART_Id_t uart_id);

// non-blocking IO
int32_t STRHAL_UART_Debug_Write_DMA(const char *data, uint32_t n);
int32_t STRHAL_UART_Write_DMA(STRHAL_UART_Id_t uart_id, const char *data, uint32_t n);
int32_t STRHAL_UART_Read(STRHAL_UART_Id_t uart_id, char *data, uint32_t n);
int32_t STRHAL_UART_Read_Blocking(STRHAL_UART_Id_t uart_id, char *data, uint32_t n, uint16_t timeout);

// blocking IO
int32_t STRHAL_UART_Debug_Write_Blocking(const char *data, uint32_t n, uint16_t timeout);
int32_t STRHAL_UART_Write_Blocking(STRHAL_UART_Id_t uart_id, const char *data, uint32_t n, uint16_t timeout);

int STRHAL_UART_FlushReception(STRHAL_UART_Id_t uart_id);

STRHAL_UART_State_t STRHAL_UART_Listen(STRHAL_UART_Id_t uart_id);
STRHAL_UART_State_t STRHAL_UART_GetState(STRHAL_UART_Id_t uart_id);

#ifdef __cplusplus
}
#endif

#endif /*STRHAL_UART_H*/
