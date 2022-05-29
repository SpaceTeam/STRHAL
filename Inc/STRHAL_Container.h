
#ifndef STM32H7XX_STRHAL_INC_STRHAL_CONTAINER_H_
#define STM32H7XX_STRHAL_INC_STRHAL_CONTAINER_H_

#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	STRHAL_BUFFER_STATE_0 = 0,
	STRHAL_BUFFER_STATE_W = (1<<0),
	STRHAL_BUFFER_STATE_R = (1<<1)
} STRHAL_Buffer_State_t;

typedef struct {
	uint8_t *data;
	STRHAL_Buffer_State_t state;
	uint32_t size;
	uint32_t n;
} STRHAL_LinearBuffer_t;

typedef struct {
	uint8_t *data;
	STRHAL_Buffer_State_t state;
	uint32_t size;
	uint32_t t;
	uint32_t n;
} STRHAL_CircularBuffer_t;

void STRHAL_LinearBuffer_Init(STRHAL_LinearBuffer_t *buf, uint8_t *mem, uint32_t size);
uint32_t STRHAL_LinearBuffer_Write(STRHAL_LinearBuffer_t *buf, uint8_t *data, uint32_t n);
uint32_t STRHAL_LinearBuffer_Read(STRHAL_LinearBuffer_t *buf, uint8_t *data, uint32_t n);

void STRHAL_CircularBuffer_Init(STRHAL_CircularBuffer_t *buf, uint8_t *mem, uint32_t size);
uint32_t STRHAL_CircularBuffer_VWrite(STRHAL_CircularBuffer_t *buf, uint32_t n);
uint32_t STRHAL_CircularBuffer_VRead(STRHAL_CircularBuffer_t *buf, uint32_t n);
uint32_t STRHAL_CircularBuffer_Write(STRHAL_CircularBuffer_t *buf, const uint8_t *data, uint32_t n);
uint32_t STRHAL_CircularBuffer_Read(STRHAL_CircularBuffer_t *buf, uint8_t *data, uint32_t n);
uint32_t STRHAL_CircularBuffer_GetFreeSpace(STRHAL_CircularBuffer_t *buf);
void STRHAL_CircularBuffer_Flush(STRHAL_CircularBuffer_t *buf);


#ifdef __cplusplus
}
#endif



#endif /* STM32H7XX_STRHAL_INC_STRHAL_CONTAINER_H_ */
